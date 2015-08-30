# splrg

## building
this needs the argtable library installed

make depends:
````
apt-get install libargtable2-dev
````

run depends:
````
apt-get install libargtable2-0
````

 1. edit the config file ````splrgd.conf```` and update the user and group names
 1. edit ````defs.h```` and remove the comment in front of ````// #define PRODUCTION```` on line 17, this will remove most of the debug statements from the binary.
 1. run ````make clean; make```` in the src directory. The binary is called ````splrgd````.
 2. copy the config file ````splrgd.conf```` to /etc/

## username
the daemon drops root priviledges to the configured user.
 1. This user must have a valid shell
 2. add the following to /etc/sudoers:
````
Defaults:username !requiretty
username ALL=(root) NOPASSWD: /usr/lib/bgch-puppet/puppet-apply.sh
````
adjust the above path if the location of the puppet-apply script changes
