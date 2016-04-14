# Directories

INCDIR=include
SRCDIR=src
TESTDIR=tests
BINDIR=bin

# Compiler / compiler flags

OPTIMIZATION_LEVEL=0
CXX=g++
CXXFLAGS=-g -Wall -Wextra -pedantic -O$(OPTIMIZATION_LEVEL) -I $(INCDIR) -Wno-long-long

# What do I compile?

SOURCES=$(shell find $(SRCDIR) -type f -name \*.cpp)
HEADERS=$(shell find $(INCDIR) -type f -name \*.h)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))
TARGET=$(BINDIR)/project

TEST_SOURCES=$(shell find $(TESTDIR) -type f -name \*.cpp)
TEST_BINARIES=$(patsubst %.cpp,%.bin,$(TEST_SOURCES))

# Objects required for headers (template specializations)
SRC_HEADER_OBJS=src/hashtable.o\
src/load.o

# Rules

.PHONY: all
all: build

.PHONY: run
run: build
	./bin/project input

.PHONY: build
build: $(TARGET)

.PHONY: clean
.SILENT: clean
clean: clean-project clean-test

.PHONY: clean-project
.SILENT: clean-project
clean-project:
	-@rm -r $(BINDIR) $(OBJECTS)

.PHONY: clean-test
.SILENT: clean-test
clean-test:
	-@rm -r $(TESTDIR)/*.bin

.PHONY: test
test: compile-tests
	./scripts/run-tests.sh

.PHONY: compile-tests
compile-tests: $(TEST_BINARIES)

$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Default rule for compiling a .cpp to an object
# Note that whenever a header changes, .cpp files get automatically recompiled

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $< -o $@ -c

# Default rule for compiling a .cpp to a binary

%.bin: %.cpp $(HEADERS) $(SRC_HEADER_OBJS)
	$(CXX) $(CXXFLAGS) $< $(SRC_HEADER_OBJS) -o $@
