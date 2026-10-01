CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -Isrc
SRC := $(wildcard src/*.cpp)
TARGET := cityfleet

.PHONY: all run clean cpp20

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -std=c++17 $(SRC) -o $@

run: $(TARGET)
	./$(TARGET)

cpp20: $(SRC)
	$(CXX) $(CXXFLAGS) -std=c++20 $(SRC) -o $(TARGET)-cpp20
	./$(TARGET)-cpp20

clean:
	rm -f $(TARGET) $(TARGET)-cpp20 src/*.o
