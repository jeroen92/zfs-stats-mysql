#include "zpoolstats.h"

int collectZpoolStats() {
	libzfs_handle_t *g_zfs = libzfs_init();

	zpool_handle_t *zhp;
	zhp = zpool_open_canfail(g_zfs, configData.poolname->value);
	
	nvlist_t *configuration, *vdevroot;
	vdev_stat_t *vdevstats;
	iostatcollection statisticscollection;
	configuration = zpool_get_config(zhp, NULL);

	// Release the handle to the zpool
	free(zhp);

	// Put the vdev tree belonging to the pool in newconfig in newnvroot.
	verify(nvlist_lookup_nvlist(configuration, ZPOOL_CONFIG_VDEV_TREE, &vdevroot) == 0);

	int nelem;
	// Load the new vdev stats in newvdevstat
	verify(nvlist_lookup_uint64_array(vdevroot, ZPOOL_CONFIG_VDEV_STATS, (uint64_t **)&vdevstats, &nelem) == 0);

	statisticscollection.readops = vdevstats->vs_ops[ZIO_TYPE_READ];
	statisticscollection.writeops = vdevstats->vs_ops[ZIO_TYPE_WRITE];
	statisticscollection.readbytes = vdevstats->vs_bytes[ZIO_TYPE_READ];
	statisticscollection.writebytes = vdevstats->vs_bytes[ZIO_TYPE_WRITE];
	
	return 0;
}
