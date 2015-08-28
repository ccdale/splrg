#!/bin/bash
# $Id: depend.sh 40 2013-02-10 11:26:56Z chris.charles.allison@gmail.com $
# generates dependancy information for c files for make files.
gcc -MM -MG "$@" | sed -e 's@^\(.*\)\.o:@\1.d \1.o:@'
