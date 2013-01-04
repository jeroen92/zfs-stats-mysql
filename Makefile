CC = gcc
FILES = main.c mysql.c
OUTPUT = -o zfsstats

SQLCONNECTOR = ./mysqlFreeBSD

INCLUDES = -I$(SQLCONNECTOR)/include
LIBS = -lmysqlclient

build: $(FILES)
	cp -R $(SQLCONNECTOR)/lib/ /usr/lib/
	$(CC) $(FILES) $(OUTPUT) $(INCLUDES) $(LIBS)
