CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET   = everglades_rpg

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
