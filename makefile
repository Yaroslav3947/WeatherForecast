# Compiler and Linker
CC = g++

# The name of the executable
TARGET = main

# Source files
SOURCES = main.cpp

# CURL library
CURL_INCLUDE = -I/usr/include
CURL_LIB = -lcurl

# jsoncpp library
JSONCPP_INCLUDE = -I/usr/include/jsoncpp/
JSONCPP_LIB = -ljsoncpp

# Compiler flags
CFLAGS = -std=c++11 $(CURL_INCLUDE) $(JSONCPP_INCLUDE)

# Linker flags
LDFLAGS = $(CURL_LIB) $(JSONCPP_LIB)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)
