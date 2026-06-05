CXX := g++

CXXFLAGS := -Wall -pipe -I. -Iinclude $(shell sdl2-config --cflags)

LDFLAGS := $(shell sdl2-config --libs) -lSDL2_mixer

TARGET := builds/Ethereal_Rift.exe

SRC_DIR := src 

SRCS = src/main.cpp

.PHONY: all clean

all: $(TARGET)

$(SRCS): $(shell find include -name "*.hpp")

$(TARGET) : $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) 

clean:
	rm -f $(TARGET)
