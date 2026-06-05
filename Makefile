CXX := x86_64-w64-mingw32-g++

CXXFLAGS := -Wall -pipe -I. -Iinclude $(shell sdl2-config --cflags)

LDFLAGS := -static-libgcc -static-libstdc++

TARGET := builds/Ethereal_Rift.exe

SRC_DIR := src 

SRCS = src/main.cpp

.PHONY: all clean

all: $(TARGET)

$(SRCS): $(shell find include -name "*.hpp")

$(TARGET) : $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
