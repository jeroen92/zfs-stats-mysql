CC = gcc
FILES = main.c mysql.c parsing.c settings.c zpoolstats.c
OUTPUT = -o zfsstats
BSD_SQLCONNECTOR = ./mysqlFreeBSD
SUN_SQLCONNECTOR = ./mysqlSolaris
OSX_SQLCONNECTOR = ./mysqlOSX
INCLUDES = -I$(SQLCONNECTOR)/include
LIBS = -lmysqlclient -lzfs -lzpool -lnvpair

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
	cp -R $(SQLCONNECTOR)/lib/* /usr/lib/

all: $(FILES)
	$(CC) $(FILES) $(OUTPUT) $(INCLUDES) $(LIBS)
