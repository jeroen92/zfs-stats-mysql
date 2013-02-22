CC = gcc
FILES = main.c mysql.c parsing.c settings.c
OUTPUT = -o zfsstats
BSD_SQLCONNECTOR = ./mysqlBSD
SUN_SQLCONNECTOR = ./mysqlSolaris
OSX_SQLCONNECTOR = ./mysqlOSX
INCLUDES = -I$(SQLCONNECTOR)/include
LIBS = -lmysqlclient
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
ifeq ($(uname_S),FreeBSD)
	SQLCONNECTOR = $(BSD_SQLCONNECTOR)
endif
ifeq ($(uname_S),SunOS)
	SQLCONNECTOR = $(SUN_SQLCONNECTOR)
endif
ifeq ($(uname_S),Darwin)
	SQLCONNECTOR = $(OSX_SQLCONNECTOR)
endif

prepare:
	cp -R $(SQLCONNECTOR)/lib/ /usr/lib/

all: $(FILES)
#	$(CC) $(FILES) $(OUTPUT) $(INCLUDES) $(LIBS)
	$(CC) $(FILES) $(OUTPUT) -L$(SQLCONNECTOR)/lib/ $(LIBS)
