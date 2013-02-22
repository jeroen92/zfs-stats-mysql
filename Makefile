CC = gcc
FILES = main.c mysql.c parsing.c settings.c
OUTPUT = -o zfsstats

SQLCONNECTOR = ./mysqlBSD

INCLUDES = -I$(SQLCONNECTOR)/include
LIBS = -lmysqlclient

prepare:
	cp -R $(SQLCONNECTOR)/lib/ /usr/lib/

all: $(FILES)
	$(CC) $(FILES) $(OUTPUT) $(INCLUDES) $(LIBS)
