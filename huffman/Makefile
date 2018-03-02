CXX=g++
CXXFLAGS=-std=c++17 -Wconversion -Wpedantic -O2
LFLAGS=
SRCDIR=src
BINDIR=bin

# OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp))
COMMON_SOURCES=src/huffman.cpp src/huffman_exception.cpp src/encoder.cpp src/decoder.cpp
COMMON_OBJECTS=$(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(COMMON_SOURCES))
ENCODE_OBJECTS=$(COMMON_OBJECTS) $(BINDIR)/encode.o
DECODE_OBJECTS=$(COMMON_OBJECTS) $(BINDIR)/decode.o
ENCODE_EXE=encode
DECODE_EXE=decode

.DEFAULT_GOAL := all

$(ENCODE_EXE): $(BINDIR) $(ENCODE_OBJECTS)
	$(CXX) $(ENCODE_OBJECTS) -o $(ENCODE_EXE) $(LFLAGS)

$(DECODE_EXE): $(BINDIR) $(DECODE_OBJECTS)
	$(CXX) $(DECODE_OBJECTS) -o $(DECODE_EXE) $(LFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR) $(ENCODE_EXE) $(DECODE_EXE)

# all: $(ENCODE_EXE) $(DECODE_EXE)
all: $(DECODE_EXE) $(ENCODE_EXE) 

.PHONY: clean all

