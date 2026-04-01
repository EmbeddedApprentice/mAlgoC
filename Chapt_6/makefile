COMPONENT_NAME = Chapt_6

# Source and include dirs (extended by module .mk files)
SRC_DIRS     =
INCLUDE_DIRS = tests

# Borrow the singly-linked list from Chapter 5
SRC_DIRS     += ../Chapt_5/list/src
INCLUDE_DIRS += ../Chapt_5/list/inc

include stack/stack.mk
include queue/queue.mk

# ── Toolchain ────────────────────────────────────────────────────────────────
CC   = gcc
CXX  = g++
LD   = g++

CFLAGS   = -Wall -Werror -Wfatal-errors
CXXFLAGS = -Wall -Werror -Wfatal-errors

# ── CppUTest (via pkg-config or fallback) ────────────────────────────────────
CPPUTEST_CXXFLAGS := $(shell pkg-config --cflags cpputest 2>/dev/null)
CPPUTEST_LDFLAGS  := $(shell pkg-config --libs   cpputest 2>/dev/null || echo "-lCppUTest -lCppUTestExt -lstdc++")

# ── Derived lists ─────────────────────────────────────────────────────────────
SRC_FILES  := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
TEST_FILES := $(wildcard tests/*.cpp)

INC_FLAGS  := $(addprefix -I,$(INCLUDE_DIRS)) \
              $(addprefix -I,$(SRC_DIRS:%/src=%/inc)) \
              $(addprefix -I,$(SRC_DIRS))

OBJ_DIR    = objs

SRC_OBJS   := $(patsubst %.c,  $(OBJ_DIR)/%.o, $(SRC_FILES))
TEST_OBJS  := $(patsubst %.cpp,$(OBJ_DIR)/%.o, $(TEST_FILES))
ALL_OBJS   := $(SRC_OBJS) $(TEST_OBJS)

TARGET = $(COMPONENT_NAME)_tests

# ── Rules ─────────────────────────────────────────────────────────────────────
.PHONY: all tests test_stack test_queue clean

all: $(TARGET)
	./$(TARGET) -v

tests: $(TARGET)
	./$(TARGET) -v

test_stack: $(TARGET)
	./$(TARGET) -g Stack -v

test_queue: $(TARGET)
	./$(TARGET) -g Queue -v

$(TARGET): $(ALL_OBJS)
	$(LD) $^ $(CPPUTEST_LDFLAGS) -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPUTEST_CXXFLAGS) $(INC_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
