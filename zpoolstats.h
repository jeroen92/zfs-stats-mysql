#include <stdio.h>
#include <sys/fs/zfs.h>
#include <libzfs.h>

#include "main.h"

/*
 *	Typedefs and structs
 */
typedef struct iostatcollection {
        uint64_t        readops;		/* 	Total read operations 	*/
        uint64_t        writeops;		/* 	Total write operations 	*/
        uint64_t        readbytes;		/* 	Total read bandwidth 	*/
        uint64_t        writebytes;		/* 	Total write bandwidth 	*/
        uint64_t	space_alloc;		/* 	Space allocated		*/
        uint64_t	space;			/* 	Space available		*/
        uint64_t	checksum_errors;	/* 	Checksum errors		*/
        uint64_t	state;			/*	VDEV state		*/
} iostatcollection;

/*
 *	Variable declarations
 */
iostatcollection statisticscollection;
/*
 *	Function prototypes
 */
int collectZpoolStats();

