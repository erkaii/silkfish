# Makefile for Silkfish Chess Engine

CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -pthread -flto -Isrc
LDFLAGS = -pthread -flto

# Uncomment for debug build
# CXXFLAGS = -std=c++17 -g -Wall -Wextra -pthread -DDEBUG -Isrc

TARGET = silkfish
SRCDIR = src

# Only main.cpp needs to be compiled
SRC_FILES = $(SRCDIR)/main.cpp
OBJECTS = main.o

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile main.cpp
main.o: $(SRCDIR)/main.cpp $(SRCDIR)/*.h
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/main.cpp -o main.o

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Rebuild from scratch
rebuild: clean all

# Install (optional - adjust path as needed)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Create distribution archive
dist: clean
	mkdir -p silkfish-1.0
	cp -r src Makefile README.md silkfish-1.0/
	tar -czf silkfish-1.0.tar.gz silkfish-1.0
	rm -rf silkfish-1.0
	@echo "Created silkfish-1.0.tar.gz"

.PHONY: all clean rebuild install dist
