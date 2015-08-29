/*
 * vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4 foldmethod=marker:
 *
 * splrg.h
 *
 * C.C.Allison
 * chris.allison@bgch.co.uk
 *
 * Started: Sunday 12 April 2015, 09:26:56
 * Version: 0.00
 * Last Modified: Saturday 29 August 2015, 17:09:28
 *
 * Copyright (c) 2015 Chris Allison chris.allison@bgch.co.uk
 */

#include <sys/types.h>
#include <argtable2.h>

#include "defs.h"

/* globals */
struct ConfigItem *configuration;
/* struct sigaction *siga; */
int llevel=5;

#include "macros.h"
#include "tools.h"
#include "configfile.h"

int setuphttpserver(void);
int httpserver(void);
void processinput(int isockfd);
int parseinput(char *buf);
void closedown(void);
void catchsignal(int sig);
char *argprocessing(int argc,char **argv);
void setDefaultConfig(void);
void daemonize();
int main(int argc,char **argv);
