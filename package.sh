#!/bin/bash
make clean
make
mkdir -p build/etc build/usr/bin
cp splrgd build/usr/bin/splrgd
cp splrgd.conf build/etc/splrgd.conf
fpm -s dir -t deb -C build -a amd64 --verbose --name splrg --version 0.9 -d libargtable2-0 .
