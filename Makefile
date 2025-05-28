# Compiler
CC = gcc

# Paths
RAYLIB_DIR = ../raylib/src

# Flags
CFLAGS = -I$(RAYLIB_DIR)
LDFLAGS = -L$(RAYLIB_DIR) -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

# Source and Target
SRC = trainer.c
TARGET = trainer

# Default build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Clean
clean:
	rm -f $(TARGET)
