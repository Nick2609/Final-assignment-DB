CC = gcc
CFLAGS = -Wall -Wextra -g -m64
TARGET = test_program
SRC = storage_mgr.c dberror.c test_assign1_1.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -lc

clean:
	rm -f $(TARGET) *.o
