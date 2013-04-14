#include "zpoolstats.h"

/*
 *	Collects zpool stats of the pool configured in the configData var.
 *	It opens a handler to the pool, load a tree of vdevs and then the
 *	stats of the root vdev. These stats are temporarily saved and then
 *	inserted in the SQL database.
 */
int collectZpoolStats() {
	// Open a handle to the zfs filesystems
	libzfs_handle_t *g_zfs = libzfs_init();

	// Open a handle to the defined zpool
	zpool_handle_t *zhp;
	zhp = zpool_open_canfail(g_zfs, configData.poolname->value);

	// Declarations and such...
	nvlist_t *configuration, *vdevroot;
	vdev_stat_t *vdevstats;
	iostatcollection statisticscollection;
	int nelem;
	configuration = zpool_get_config(zhp, NULL);

	// Now we have the config, we can release the handle to the zpool
	libzfs_fini(g_zfs);
	free(zhp);

	// Put the vdev tree belonging to the pool in newconfig in newnvroot.
	verify(nvlist_lookup_nvlist(configuration, ZPOOL_CONFIG_VDEV_TREE, &vdevroot) == 0);

	// Load the new vdev stats in newvdevstat
	verify(nvlist_lookup_uint64_array(vdevroot, ZPOOL_CONFIG_VDEV_STATS, (uint64_t **)&vdevstats, &nelem) == 0);

	// Place the collected statistics in the collection
	statisticscollection.readops = vdevstats->vs_ops[ZIO_TYPE_READ];
	statisticscollection.writeops = vdevstats->vs_ops[ZIO_TYPE_WRITE];
	statisticscollection.readbytes = vdevstats->vs_bytes[ZIO_TYPE_READ];
	statisticscollection.writebytes = vdevstats->vs_bytes[ZIO_TYPE_WRITE];
	statisticscollection.checksum_errors = vdevstats->vs_checksum_errors;
	statisticscollection.state = vdevstats->vs_state;
	statisticscollection.space_alloc = vdevstats->vs_alloc;
	statisticscollection.space = vdevstats->vs_space;

	
	// Create the query and post it to MySQL
	char queryString[1024];
	snprintf(queryString, 1024, "INSERT INTO io_stats (date, %s, %s, %s, %s, %s, %s, %s, %s) VALUES (NOW(), '%llu', '%llu', '%llu', '%llu', '%llu', '%llu', '%llu', '%llu')", "iop_read", "iop_write", "bandwidth_read", "bandwidth_write", "space", "space_alloc", "checksum_errors", "state", statisticscollection.readops, statisticscollection.writeops, statisticscollection.readbytes, statisticscollection.writebytes, statisticscollection.space, statisticscollection.space_alloc, statisticscollection.checksum_errors, statisticscollection.state);
	if (executeQuery(queryString)) return 1;
	return 0;
}
