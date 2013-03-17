#include <stdio.h>
#include <sys/fs/zfs.h>
#include <libzfs.h>

#include "main.h"

typedef struct iostatcollection {
	uint64_t	readops;
	uint64_t	writeops;
	uint64_t	readbytes;
	uint64_t	writebytes;
} iostatcollection;

int collectZpoolStats();
