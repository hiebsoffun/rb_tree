CFLAGS=-g -O2 -Wall -Wextra -Isrc -DNDEBUG -rdynamic $(OPTFLAGS)
LIBS=-ldl $(OPTLIBS)
GCC=gcc
RM=rm -rf
OBJECTS_OUT_DIR=build
TEST_OUT_DIR=bin


SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
OBJECTS_OUT=$(patsubst src/%,$(OBJECTS_OUT_DIR)/%,$(OBJECTS))

TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))
TESTS_OUT=$(patsubst tests/%,$(TEST_OUT_DIR)/%,$(TESTS))

TARGET=build/rb_tree.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# The Target Build
all: $(TARGET) $(SO_TARGET) tests

$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS_OUT)
	ranlib $@

$(OBJECTS): CFLAGS += -fPIC
$(OBJECTS): $(SOURCES)
	$(GCC) $(CFLAGS) -c $(SOURCES) -o $(OBJECTS_OUT)

$(SO_TARGET): $(TARGET) $(OBJECTS_OUT)
	$(GCC) -shared -o $@ $(OBJECTS_OUT)

build:
	@mkdir -p build
	@mkdir -p bin

# The Unit Tests
.PHONY: tests
# tests: CFLAGS = $(TARGET)
tests: $(TEST_SRC)
	$(GCC) $(CFLAGS) $(TEST_SRC) $(TARGET) -o $(TESTS_OUT)

clean:
	$(RM) $(OBJECTS_OUT) $(TESTS_OUT) $(TARGET) $(SO_TARGET) build bin
