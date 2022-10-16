TARGETS = tema1.c homework.c homework.h struct.h other.h
CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99
EXEC = tema
ZIPNAME = 321CABuzeaAlexandru-Mihai-Iulian

build:
	$(CC) $(CFLAGS) $(TARGETS) -o $(EXEC)

clean:
	rm -f $(EXEC) memCheck input

checker:
	python3 checker.py

pack:
	zip -r $(ZIPNAME) $(TARGETS) README Makefile

.PHONY: clean checker
