# תשובות וניסויים — תרגיל בית 3

הרצתי את הניסויים על g++ 13.3 עם `make` / `-std=c++17` וגם `-std=c++20`. מה שמופיע פה זה הפלט האמיתי, לא מהזיכרון.

---

## חלק א.2 — למה חייבים `Vehicle::statusLine()`

כתבתי את זה גם בהערה ב-`PoweredVehicle.cpp`.

אם בתוך `PoweredVehicle::statusLine()` הייתי כותב סתם `statusLine()` בלי שם המחלקה, הקריאה הייתה הולכת ל-vtable לפי `this`. כלומר שוב לאותה פונקציה, שוב ושוב, עד שהמחסנית מתפוצצת.  
עם `Vehicle::statusLine()` המהדר יודע כבר בקומפילציה לקרוא לגרסה של האב, בלי דיספאץ' וירטואלי. לכן זה לא נכנס ללופ.

---

## חלק ב'2 — שלושת הניסויים

### ניסוי 1: הסרתי את `virtual` מ-`dailyCostILS()` ב-`Vehicle`

אם רק מוחקים את המילה `virtual` ומשאירים `= 0`, מקבלים קודם:

```text
src/Vehicle.h:24:12: error: initializer specified for non-virtual method 'double Vehicle::dailyCostILS() const'
   24 |     double dailyCostILS() const = 0;
```

כי `= 0` מותר רק על פונקציה וירטואלית. אז שמתי במקום מימוש רגיל `return 0` והשארתי את כל ה-`override` במחלקות היורשות. אז כבר `override` תפס:

```text
src/PoweredVehicle.h:15:12: error: 'double PoweredVehicle::dailyCostILS() const' marked 'override', but does not override
   15 |     double dailyCostILS() const override;
      |            ^~~~~~~~~~~~
src/ElectricBus.h:18:12: error: 'double ElectricBus::dailyCostILS() const' marked 'override', but does not override
   18 |     double dailyCostILS() const override;
      |            ^~~~~~~~~~~~
```

(אותה שגיאה גם על ManualVehicle / GarbageTruck / PatrolBike / WaterTanker)

`override` בודק שיש באב פונקציה וירטואלית עם אותה חתימה. בלי `virtual` אין מה לדרוס, והמהדר עוצר במקום לתת באג שקט.

---

### ניסוי 2: בלי `virtual` ובלי `override` על `dailyCostILS`

הפלט התקין (אחרי `make run`):

```text
--- Fleet Report ---
[DepotA] Bus-1 (ElectricBus) powered | Daily Cost: 111 ILS | 87.5% @d1
[DepotB] Truck-1 (GarbageTruck) powered | Daily Cost: 382.5 ILS | full @d1
[DepotC] Bike-1 (PatrolBike) rides=8 | Daily Cost: 16 ILS
[DepotA] Tanker-1 (WaterTanker) powered | Daily Cost: 116 ILS | 500.0L @d1

Total Daily Cost: 625.5 ILS
Vehicles in service: 4
```

הפלט אחרי שהסרתי גם את `virtual` וגם את `override` מ-`dailyCostILS` (עם `return 0` באב):

```text
--- Fleet Report ---
[DepotA] Bus-1 (ElectricBus) powered | Daily Cost: 0 ILS | 87.5% @d1
[DepotB] Truck-1 (GarbageTruck) powered | Daily Cost: 0 ILS | full @d1
[DepotC] Bike-1 (PatrolBike) rides=8 | Daily Cost: 0 ILS
[DepotA] Tanker-1 (WaterTanker) powered | Daily Cost: 0 ILS | 500.0L @d1

Total Daily Cost: 0 ILS
Vehicles in service: 4
```

הפרש בעלות: 625.5 - 0 = 625.5 ש"ח.

בלי `virtual` המהדר מחליט בזמן קומפילציה לפי הטיפוס הסטטי. `printReport` ו-`totalDailyCostILS` עובדים מול `const Vehicle&`, אז נקראת תמיד הגרסה של `Vehicle` (שהחזרתי בה 0) ולא של האוטובוס/המשאית.  
`kind()` ו-`statusLine()` נשארו וירטואליים אז השמות בדוח עדיין נכונים, רק העלות נשברה.

---

### ניסוי 3: `vehicles_` כ-`std::vector<Vehicle>`

שיניתי ב-`Fleet.h` את `vehicles_` ל-`std::vector<Vehicle>` כמו שכתוב. בהתחלה `add` עדיין עושה `push_back(std::move(v))` על `unique_ptr`, אז קיבלתי:

```text
Fleet.cpp:8:24: error: no matching function for call to 'std::vector<Vehicle>::push_back(std::unique_ptr<PoweredVehicle>)'
note:   no known conversion from 'std::unique_ptr<PoweredVehicle>' to 'const Vehicle&'
```

אז ניסיתי לדחוף לפי ערך (`push_back(*v)`), ופה כבר יוצאת הנקודה של הניסוי — אי אפשר לבנות `Vehicle`:

```text
error: invalid new-expression of abstract class type 'Vehicle'
Vehicle.h:8:7: note:   because the following virtual functions are pure within 'Vehicle':
Vehicle.h:23:25: note:     'virtual std::string Vehicle::kind() const'
Vehicle.h:24:20: note:     'virtual double Vehicle::dailyCostILS() const'
```

אם שמים `Vehicle` כשדה רגיל ב-`Fleet` (לא בתוך vector) מקבלים ישר:

```text
Fleet.h:33:13: error: cannot declare field 'Fleet::cannot_exist_' to be of abstract type 'Vehicle'
Vehicle.h:8:7: note:   because the following virtual functions are pure within 'Vehicle':
```

למה דווקא פה זה נכשל: `Vehicle` אבסטרקטית (`kind` ו-`dailyCostILS` טהורות), ואי אפשר ליצור ממנה אובייקט לפי ערך.

אם האב לא היה אבסטרקטי הקוד היה מתקמפל, ואז היה object slicing בשקט: נכנס אוטובוס, נחתך רק חלק ה-`Vehicle`, והעלות/המדידות של הנגזר נעלמות. בלי שגיאה, סתם תוצאות שגויות. לכן אנחנו שומרים מצביעים ולא `Vehicle` לפי ערך.

---

## חלק ג'2 — גודל `Ledger`

הדפסתי:

```text
sizeof(Ledger<Measurement<double>, 4>)  = 208 bytes
sizeof(Ledger<Measurement<double>, 16>) = 784 bytes
```

`Ledger` מחזיק `std::array<T,N>` על הסטאק, בלי `new`. `N` נכנס לטיפוס בזמן קומפילציה אז הגודל גדל עם N.  
זה לא בדיוק כפול 4 (208*4 = 832, לא 784) כי יש עוד שני `size_t` (`head_` ו-`size_`). החלק של המערך כן גדל פי 4: 4*48=192, 16*48=768, פלוס 16 בתים של המונים.

למה `small = big;` לא מתקמפל:

```text
/tmp/sz.cpp:11:13: error: no match for 'operator=' (operand types are 'Ledger<Measurement<double>, 4>' and 'Ledger<Measurement<double>, 16>')
   11 |     small = big;
In file included from /tmp/sz.cpp:1:
Ledger.h:29:7: note: candidate: 'Ledger<Measurement<double>, 4>& Ledger<Measurement<double>, 4>::operator=(const Ledger<Measurement<double>, 4>&)'
note:   no known conversion for argument 1 from 'Ledger<Measurement<double>, 16>' to 'const Ledger<Measurement<double>, 4>&'
```

`N` הוא חלק מהטיפוס. `Ledger<...,4>` ו-`Ledger<...,16>` הם שני טיפוסים שונים לגמרי, אין ביניהם המרה.

---

## חלק ג'3 — סדר ההתמחויות ב-`Formatter`

הרצתי:

```cpp
fmt(std::vector<Measurement<bool>>{
    makeMeasurement(true, 1, ""),
    makeMeasurement(false, 2, "")
});
```

פלט:

```text
[full @d1, empty @d2]
```

מה קורה לפי הסדר:

1. הטיפוס החיצוני הוא `vector`, אז נכנסים ל-`Formatter<std::vector<U>>` (התמחות חלקית). זה שם את הסוגריים ועובר איבר איבר.
2. כל איבר הוא `Measurement<bool>`, אז `Formatter<Measurement<U>>`. משם קוראים רקורסיבית ל-`Formatter<U>` על ה-value, ואז מדביקים יחידה ו-`@d` + היום.
3. U הוא `bool`, אז ההתמחות המלאה `Formatter<bool>` מחזירה `full` / `empty` ולא 1/0.

המהדר בכל שלב לוקח את ההתמחות הכי ספציפית שמתאימה.

---

## חלק ד'1 — אותה טעות ב-C++17 מול C++20

ניסיתי `Ledger<UnformattableType, 4>` כש-`UnformattableType` זה סתם `struct { int x; }` בלי `operator<<`.

**C++17** (`-std=c++17`), `static_assert`:

```text
Ledger.h: In instantiation of 'class Ledger<UnformattableType, 4>':
bad.cpp:4:34:   required from here
Ledger.h:21:37: error: static assertion failed: T must be formattable via fmt(T)
   21 |     static_assert(IsFormattable<T>::value,
      |                                     ^~~~~
```

**C++20** (`-std=c++20`), concept:

```text
bad.cpp:4:32: error: template constraint failure for 'template<class T, int N>  requires  Formattable<T> class Ledger'
    4 |     Ledger<UnformattableType, 4> bad;
Ledger.h:15:9:   required for the satisfaction of 'Formattable<T>' [with T = UnformattableType]
Ledger.h:15:41: note: the expression 'IsFormattable<U, void>::value [with T = UnformattableType]' evaluated to 'false'
   15 | concept Formattable = IsFormattable<T>::value;
```

ב-20 ההודעה יותר בממשק: ישר כתוב שה-concept `Formattable` לא מתקיים על T.  
ב-17 זה נופל רק אחרי שהתבנית כבר מתחילה להיווצר, על ה-`static_assert` שלנו. בשני המקרים זו הודעה שלנו ולא חומה של `ostringstream`.

---

## חלק ד'2 — איך שני המנגנונים עובדים ביחד

`lastMeasurementText()` וירטואלית. `Fleet` עובר על `const Vehicle&` (דרך `FleetRegistry`) וקורא לה. בזמן ריצה ה-vtable מגיע לאוטובוס או למשאית לפי מה שבאמת יושב שם.

בתוך הפונקציה של כל מחלקה יש `fmt(ledger_.latest())`. `fmt` זו תבנית, אז את זה המהדר סגר כבר בקומפילציה: באוטובוס T הוא `Measurement<double>` (87.5 עם ספרה אחת), במשאית `Measurement<bool>` (`full`/`empty`).

בקיצור: האובייקט בוחר את המחלקה בריצה, המהדר בוחר את העיצוב בקומפילציה.

---

## חלק ה' — שאלות עיוניות

### 1. למה מחלקת בסיס פולימורפית חייבת הורס וירטואלי?

`delete` על מצביע לבסיס (`Vehicle*`) מחפש הורס לפי הטיפוס הסטטי של המצביע, אלא אם ההורס וירטואלי. בלי `virtual` רץ רק הורס של `Vehicle`, ההורס של האוטובוס לא נקרא, השדות שלו לא משוחררים, וההתנהגות לא מוגדרת (UB / דליפה).  
למה זה שורד בדיקות: `kind()` ו-`dailyCostILS()` ממשיכות לעבוד רגיל דרך ה-vtable, אז טסטים פונקציונליים עוברים. הבאג יושב רק בשחרור.

### 2. Object slicing

Slicing זה כשמעתיקים אובייקט נגזר לתוך משתנה/מכולה של האב לפי ערך, למשל `Vehicle v = bus` או `vector<Vehicle>`. מועתק רק חלק האב, והשדות של האוטובוס נחתכים. הקוד מתקמפל כי אוטובוס *הוא* Vehicle (IS-A), ואין פה שגיאת טיפוס.  
אם האב אבסטרקטי אי אפשר בכלל ליצור `Vehicle` לפי ערך, אז אי אפשר להגיע למצב הזה. לכן אנחנו שומרים `unique_ptr<Vehicle>`.

### 3. למה תבניות חייבות לשבת בכותרת?

תבנית זה מתכון, לא קוד מוכן. המהדר צריך לראות את כל המימוש ביחידת התרגום שבה הוא מייצר מופע (`Ledger<Measurement<double>,6>` וכו'). אם נשים את זה ב-`.cpp` נפרד, הקומפילציה של `ElectricBus.cpp` תעבור, אבל בלינקר נקבל `undefined reference` כי אף יחידה לא ייצרה את המופע הספציפי. זו שגיאת קישור ולא שגיאת קומפילציה.

### 4. מה המהדר מייצר למופעי תבניות?

עבור `Ledger<Measurement<double>,4>` ועבור `Ledger<Measurement<bool>,4>` הוא מייצר שתי מחלקות נפרדות לגמרי בקוד.  
בגלל זה הבינארי גדל (code bloat), זמן ההידור מתארך (כל שילוב מעובד מחדש), והודעות השגיאה יוצאות ארוכות כי הוא מדפיס את כל שרשרת ההרחבה עד הנקודה שנשברה.

### 5. למה שרשרת `dynamic_cast` זה ריח תכנוני?

כל סוג חדש מחייב לערוך את השרשרת בעוד מקום — שובר פתוח/סגור. בנוסף הסדר חשוב: אם בודקים קודם `PoweredVehicle` ורק אחר כך `ElectricBus`, האוטובוס נתפס מוקדם מדי.  
בפתרון וירטואלי אין if על טיפוסים. מוסיפים מחלקה חדשה, מממשים את החוזה, ו-`Fleet` לא משתנה. זו גם הסיבה שבבונוס לא נגענו ב-`Fleet`.

---

## חלק ו' — בונוס (`WaterTanker`)

הוספתי רק את `src/WaterTanker.h` וב-`main.cpp` include + שורת `add`. לא נגעתי ב-`Fleet` / `Vehicle` בשביל הסוג החדש.

```text
$ git diff --cached --stat
 src/WaterTanker.h | 38 ++++++++++++++++++++++++++++++++++++++
 src/main.cpp      |  5 ++++-
 2 files changed, 42 insertions(+), 1 deletion(-)
```

```text
$ git diff --cached -- src/main.cpp
+#include "WaterTanker.h" // עבור הבונוס
+        // הוספת הבונוס
+        fleet.add(std::make_unique<WaterTanker>(4, "Tanker-1", "DepotA", 120.0, 1000.0));
```

העיקרון: פתוח/סגור — אפשר להרחיב בלי לשנות קוד ישן.  
המילה שקנתה את זה: `virtual`. `Fleet` מדבר רק עם `Vehicle`/`PoweredVehicle`, וה-vtable מגיע לבד לטנקר.
