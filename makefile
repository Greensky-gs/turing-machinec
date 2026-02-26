CC=gcc
FLAGS=-Wall -Wextra
SANTS=-fsanitize=address,undefined -g
OUTPUT=main.uwu

SRC_DIR=src
BUILD_DIR=build
BIN_DIR=bin

SOURCES=$(wildcard $(SRC_DIR)/*.c)
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

GTK_INCLUDE=$$(pkg-config --cflags --libs gtk+-3.0) 

all: $(BUILD_DIR) $(BIN_DIR) $(BIN_DIR)/$(OUTPUT)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) $^ -o $@ -c $(GTK_INCLUDE)

$(BIN_DIR)/$(OUTPUT): $(OBJECTS)
	$(CC) $(OBJECTS) $(FLAGS) $(SANTS) $(GTK_INCLUDE) -o $(BIN_DIR)/$(OUTPUT)

$(BUILD_DIR):
	mkdir $(BUILD_DIR) -p
$(BIN_DIR):
	mkdir $(BIN_DIR) -p

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)
launch:
	./$(BIN_DIR)/$(OUTPUT)

full:
	make clean
	make
	clear
	make launch
