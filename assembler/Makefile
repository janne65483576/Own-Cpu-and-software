# compiler flags
CC     = gcc
CFLAGS = -Wall -Wextra -g

# project structure
SRC_DIR = src
BUILD_DIR = build
TARGET = assembler

# Automatic source code search
SRCS = $(filter-out $(SRC_DIR)/sort.c, $(wildcard $(SRC_DIR)/*.c))
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default-Target
all: $(TARGET)

# link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# create build directory if it does not exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Aufräumen
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
