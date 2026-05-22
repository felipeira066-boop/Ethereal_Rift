CXX := g++

CXXFLAGS := -Wall -pipe -I. $(shell sdl2-config --cflags)

LDFLAGS := $(shell sdl2-config --libs) -lSDL2_mixer

TARGET := test

SRC_DIR := src 

SRCS = src/main.cpp library/assets/sounds/background.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET) : $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) $(LDFLAGS)

$(SRCS): $(shell find library -name "*.hpp")

clean:
	rm -f $(TARGET)


 