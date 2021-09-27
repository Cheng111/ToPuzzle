# Copyright 2021 cchen67
#cs594 lab1 jgraph
CC = g++
INC_DIR = includes
CFLAGS = -Wall -Wextra -std=c++11 -I$(INC_DIR)
SRC_DIR = src
#OBJ_DIR = obj
TES_DIR = test
FILES = $(SRC_DIR)/ToPuzzle.cpp $(SRC_DIR)/ToPuzzleMain.cpp
#OBJFILES = ToPuzzle.o ToPuzzleMain.o
TARGET = ToPuzzle
TEST1 = $(TES_DIR)/test1.png
TEST2 = $(TES_DIR)/test2.jpg
TEST3 = $(TES_DIR)/test3.jpg
TEST4 = $(TES_DIR)/test4.png
TEST5 = $(TES_DIR)/test5.jpeg

all : $(TARGET)

$(TARGET) : $(FILES)
#	$(CC) $(CFLAGS) -c $(FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(FILES)

clean:
	rm -f $(TARGET)
	rm -f puzzle.jgr test2.jgr test3.jgr test4.jgr test5.jgr
	rm -f puzzle.jpg test2.jpg test3.jpg test4.jpg test5.jpg

check:
	./$(TARGET) -i $(TEST1)
	./$(TARGET) -i $(TEST2) -o test2
	./$(TARGET) -i $(TEST3) -o test3 -r 8 -c 10 -d h -w 0.2 0.2 1
	./$(TARGET) -d h -i $(TEST4) -o test4 -w 0 1 0
	./$(TARGET) -c 16 -d h -i $(TEST5) -o test5


