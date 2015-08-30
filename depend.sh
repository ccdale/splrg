#!/bin/bash
# generates dependancy information from c files for make files.
gcc -MM -MG "$@" | sed -e 's@^\(.*\)\.o:@\1.d \1.o:@'
