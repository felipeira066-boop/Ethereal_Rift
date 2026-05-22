CXX := g++

CXXFLAGS := -Wall -pipe -I. $(shell sdl2-config --cflags)

LDFLAGS := $(shell sdl2-config --libs) -lSDL2_mixer

TARGET := test

SRC_DIR := src 

SRCS = src/main.cpp library/assets/sounds/background.cpp

.PHONY: all clean

all: $(TARGET)

$(SRCS): $(shell find library -name "*.hpp")

$(TARGET) : $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
