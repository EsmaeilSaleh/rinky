CC = gcc
RAYLIB_DIR = ../raylib/src

SRC = trainer.c
TARGET = trainer
CFLAGS = -I$(RAYLIB_DIR)

# Detect OS for linker flags
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    LDFLAGS = -L$(RAYLIB_DIR) -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
endif

ifeq ($(UNAME_S),Darwin)  # macOS
    LDFLAGS = -L$(RAYLIB_DIR) -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lpthread -lm
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)

