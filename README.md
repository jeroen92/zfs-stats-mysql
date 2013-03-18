zfs-stats-mysql
===============

Program written in C that parses ZFS statistics to a MySQL database. When runned, the program will continue as a deamon on the background.
Every 30 minutes it will collect and parse certain ZFS statistics and upload it to a user-specified MySQL DB.

Dependencies:

  -FreeBSD/Solaris installation

  -ZFS

  -MySQL connector (delivered with this package)

VERSION 1.2
Added zpool IO statistics. For the full set of changes, please read the commit history.
 To install:

  1. Download the files from the GIT repo to your FreeBSD machine.

  2. Create a user and a new schema in MySQL. You don't have to create any tables, the program will do it for you ;)
 
  3. Give the user sufficient rights on the created schema (INSERT, CREATE TABLE, SHOW)
  
  4. Copy the MySQL C API with 'make prepare'. If these are already installed, compile with 'make all'.
  
  5. Run the ./zfsstats executable and follow the steps on your screen.
