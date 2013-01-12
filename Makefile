CC = gcc
FILES = main.c mysql.c parsing.c settings.c
OUTPUT = -o zfsstats

SQLCONNECTOR = ./mysqlFreeBSD

INCLUDES = -I$(SQLCONNECTOR)/include
LIBS = -lmysqlclient

prepare:
	cp -R $(SQLCONNECTOR)/lib/ /usr/lib/

build: $(FILES)
	$(CC) $(FILES) $(OUTPUT) $(INCLUDES) $(LIBS)

all: $(FILES)
	cp -R $(SQLCONNECTOR)/lib/ /usr/lib/
	$(CC) $(FILES) $(OUTPUT) $(INCLUDES) $(LIBS)