CC = g++
CC_FLAGS = -Wall \
		   -lSDL2 \
		   -lSDL2main
TARGET = tictactoe
DIR_TARGET = bin
SRC = $(shell find ./src -name "*.cpp")

$(TARGET): dir
	$(CC) -o ./$(DIR_TARGET)/$(TARGET) $(SRC) $(CC_FLAGS)

debug: CC_FLAGS += -g3 -fsanitize=address,undefined
debug: $(TARGET)

run: $(TARGET)
	./$(DIR_TARGET)/$(TARGET)

.PHONY: dir
dir:
	if [ ! -d "bin" ]; then mkdir bin; fi

.PHONY: clean
clean: ./$(DIR_TARGET)/$(TARGET)
	rm ./$(DIR_TARGET)/*
