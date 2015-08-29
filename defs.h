/*
 * vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4 foldmethod=marker:
 *
 * defs.h
 *
 * C.C.Allison
 * chris.allison@hotmail.com
 *
 * Started: Monday  6 April 2015, 09:24:04
 * Version: 0.00
 * Last Modified: Saturday 29 August 2015, 11:08:15
 */

/*
#define _XOPEN_SOURCE 700
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>

/*
 * libgen for the posix version of basename (tools.c:129)
 */
#include <libgen.h>

#include <errno.h>
#include <limits.h>

/* for directory stuff */
#include <sys/types.h>
#include <dirent.h>

#define PROGNAME "splrgd"
#define PROGVERSION "0.0.1"
#define PROGEMAIL "chris.allison@hotmail.com"
#define PROGCOPYRIGHT "Copyright (C) 2014-2015 Chris Allison"
#define PROGLICENSE "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n"

/*
 * uncomment for production build
 */
/*
 * #define CCA_PRODUCTION 1
 */
#ifdef CCA_PRODUCTION
#define PROGCONF "/etc/"PROGNAME".conf"
#else
#define PROGCONF "/home/chrisallison/src/splrg/"PROGNAME".conf"
#endif

#define CCA_HOME "/"
#define CCA_LOCK_FILE "/var/tmp/"PROGNAME".pid"
#define MAX_MSG_LEN 4096

#define CCA_DEFAULT_CONFIGFILE PROGCONF
#define CCA_DEFAULT_DAEMONIZE "1"
#define CCA_DEFAULT_PUPPETBIN "/usr/lib/bgch-puppet/puppet-apply.sh"
