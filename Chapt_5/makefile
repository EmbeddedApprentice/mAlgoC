COMPONENT_NAME = Chapt_5

# Source and include dirs (extended by module .mk files)
SRC_DIRS     =
INCLUDE_DIRS = tests

include list/list.mk
# include future_module/future_module.mk

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
.PHONY: all clean

all: $(TARGET)
	./$(TARGET) -v

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
