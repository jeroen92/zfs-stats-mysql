CC = gcc
FILES = main.c mysql.c parsing.c settings.c
OUTPUT = -o zfsstats

BSD_SQLCONNECTOR = ./mysqlBSD
SUN_SQLCONNECTOR = ./mysqlSolaris

INCLUDES = -I$(SQLCONNECTOR)/include
LIBS = -lmysqlclient

prepare bsd:
	cp -R $(BSD_SQLCONNECTOR)/lib/ /usr/lib/

prepare sun:
	cp -R $(SUN_SQLCONNECTOR)/lib/ /usr/lib/

all: $(FILES)
	$(CC) $(FILES) $(OUTPUT) $(INCLUDES) $(LIBS)
