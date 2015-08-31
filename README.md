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

 1. edit ````defs.h```` and remove the comment in front of ````// #define PRODUCTION```` on line 17, this will remove most of the debug statements from the binary.
 1. run 
 ````
 make clean
 make
 ````
 in the src directory. The binary is called ````splrgd````.
 2. copy the config file ````splrgd.conf```` to /etc/

NOTE: ````make```` will output 3 errors regarding missing dependency files, then it will correct itself as it runs depends.sh to create them. Compilation should then continue (and take less than a 3 seconds on a reasonably modern machine).

run
````
./package.sh
````
in the source directory to build a debian package. This will create a build directory, run make clean and make, then run fpm.

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

## Usage
There are currently 3 commands that you can use:
 1. ````status```` returns the number of forked processes attempting to run puppet. The locking mechanism in puppet-apply.sh will keep the machine from running more than one instance of puppet at once. normally ruturns 0 or 1.
 2. ````runpuppet```` runs puppet-apply.sh in a forked process. Does not return any data (though the http status header will be 200).
 3. ````lastlog```` returns the contents of /var/tmp/last_puppet_run.log (or wherever you have re-directed it to).
Any other command will return an hhtp status code of 404

### Examples
````
curl http://localhost:2208/status
0
curl http://localhost:2208/runpuppet

curl http://localhost:2208/status
1
````

