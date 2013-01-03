zfs-stats-mysql
===============

Program written in C that parses ZFS statistics to a MySQL database. When runned, the program will continue as a deamon on the background.
Every 30 minutes it will collect and parse certain ZFS statistics and upload it to a user-provided MySQL DB.

Dependencies:

  -FreeBSD installation with a working /sbin/sysctl

  -ZFS

  -MySQL connector (delivered with this package)

The files will be uploaded soon. I will be implementing lots of improvements over time to make the program more flexible, so the user can define which statistics he wants to keep track of.
