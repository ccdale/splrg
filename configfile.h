/*
 * vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4 foldmethod=marker:
 *
 * configfile.h
 *
 * C.C.Allison
 * chris.allison@hotmail.com
 *
 * Started: Monday 24 December 2012, 13:11:56
 * Version: 0.00
 * Revision: $Id: configfile.h 37 2013-02-03 02:47:31Z chris.charles.allison@gmail.com $
 * Last Modified: Monday  1 June 2015, 07:33:04
 */
#ifndef CCA_CONFIGFILE_H
#define CCA_CONFIGFILE_H

#include "defs.h"
#include "macros.h"
#include "tools.h"

struct ConfigItem{
    char *key;
    char *value;
    struct ConfigItem *next;
};

/* globals */
extern struct ConfigItem *configuration;

#endif

int initConfig( void );
void getConfigFromFile(char *filename);
struct ConfigItem *addConfig(char *key, char *value);
struct ConfigItem *updateConfig(char *key, char *value);
int deleteConfigItem(char *key);
void unlinkConfig(struct ConfigItem *prev, struct ConfigItem *dc);
void deleteConfig( void );
struct ConfigItem *searchConfigP(char *key);
char *configValueDup(char *key);
char *configValue(char *key);
void printConfig( void );
