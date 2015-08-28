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
 * Last Modified: Monday  1 June 2015, 07:31:15
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
/*
#include <regex.h>
*/
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

#define PROGNAME "loft"
#define PROGVERSION "0.01"
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
#define PROGCONF "/home/chris/src/c/loft/"PROGNAME".conf"
#endif

#define CCA_HOME "/"
#define CCA_LOCK_FILE "/var/run/"PROGNAME".pid"
#define MAX_MSG_LEN 4096

#define CCA_DEFAULT_CONFIGFILE "/etc/"PROGNAME".conf"
#define CCA_DEFAULT_DAEMONIZE "1"
#define CCA_DEFAULT_BUFFERSIZE "1024"
#define CCA_DEFAULT_SLEEPTIME "1"
#define CCA_DEFAULT_FPOS "0"
#define CCA_DEFAULT_TIME_FORMAT "%a, %d %b %Y %H:%M:%S %Z"
#define CCA_DEFAULT_FILEBIN "/usr/bin/file"
#define CCA_DEFAULT_FILEBINARGS "-ib"
#define CCA_DEFAULT_GUNZIPBIN "/bin/gunzip"
#define CCA_DEFAULT_GUNZIPBINARGS "-c"
#define CCA_DEFAULT_UNZIPBIN "/usr/bin/unzip"
#define CCA_DEFAULT_UNZIPBINARGS "-p"
#define CCA_DEFAULT_RLFEXT ".1.gz"
