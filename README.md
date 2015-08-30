# splrgd
splrgd is a daemon to listen for requests to run puppet on this host.

## Usage
````
./splrgd -h
Usage: splrgd [-hv] [-c /etc/splrgd.conf] [-l <n>]
splrgd is a daemon to listen for requests to run puppet on this host.

  -c, --conf-file=/etc/splrgd.conf configuration file
  -h, --help           print this help and exit
  -l, --log-level=<n>  7=LOG-DEBUG .. 0=LOG_EMERG - default: 5 (LOG_NOTICE)
  -v, --version        print version information and exit

report bugs to operations@bgch.co.uk
````

## Building
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

## Configuration
no changes will be required to the configuration file if these defaults suffice (obvs. they don't as we disable the umbongo user)
 1. portno: port number to listen on (default: 2208)
 1. username: username to run the daemon as (default: umbongo)
 2. groupname: groupname to run the daemon as (default: umbongo)
 3. puppetbin: path to the puppet-apply.sh script (default: /usr/lib/bgch-puppet/puppet-apply.sh)
 4. puppetlog: where to put the output of the puppet-apply.sh script (default: /var/log/last_puppet_run.log)

## Username
the daemon drops root priviledges to the configured user.
 1. This user must have a valid shell
 2. add the following to /etc/sudoers:
````
Defaults:username !requiretty
username ALL=(root) NOPASSWD: /usr/lib/bgch-puppet/puppet-apply.sh
````
adjust the above path if the location of the puppet-apply script changes
