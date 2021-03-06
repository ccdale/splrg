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
 * Last Modified: Monday 31 August 2015, 14:50:13
 *
 * Copyright (c) 2015 Chris Allison chris.allison@bgch.co.uk
 */

#include "defs.h"

/* globals */
struct ConfigItem *configuration;
// struct sigaction *siga;
int llevel=5;
int timetodie=0;
int puppetrunning=0;

#include "macros.h"
#include "tools.h"
#include "configfile.h"

int setuphttpserver(void);
void httpserver(void);
void processinput(int isockfd, int ipaddr);
int parseinput(char *buf,char **data,int isock);
void runpuppet(int isock);
void catchsignal(int sig);
char *argprocessing(int argc,char **argv);
void setDefaultConfig(void);
void daemonize();
int main(int argc,char **argv);
