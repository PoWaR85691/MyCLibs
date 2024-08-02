TARGET := main
TARGET_EXT := .exe
ARGS :=

CC := gcc
CC_FLAGS := -std=c11 -Wall -Wextra -O3

SRC_DIR := ./src/
OBJ_DIR := ./obj/

SRC_EXT := .c
OBJ_EXT := .o

RM := rm

SRC := $(wildcard $(SRC_DIR)*$(SRC_EXT))
OBJ := $(patsubst $(SRC_DIR)%$(SRC_EXT), $(OBJ_DIR)%$(OBJ_EXT), $(SRC))

$(TARGET)$(TARGET_EXT) : $(OBJ)
	$(CC) $(CC_FLAGS) $^ -o $@

$(OBJ_DIR)%$(OBJ_EXT) : $(SRC_DIR)%$(SRC_EXT)
	$(CC) $(CC_FLAGS) -c $< -o $@

run : $(TARGET)$(TARGET_EXT)
	./$^ $(ARGS)

clean :
	$(RM) $(TARGET)$(TARGET_EXT) $(OBJ_DIR)*$(OBJ_EXT)