#!/bin/bash
iteration=
ifn=.iteration
[ -r $ifn ] && <$ifn read iteration
iteration=${iteration:-0}
iteration=$(( iteration + 1 ))
echo $iteration >$ifn
rm -rf build
make clean
make
mkdir -p build/etc build/usr/bin
cp splrgd build/usr/bin/splrgd
cp splrgd.conf build/etc/splrgd.conf
fpm -s dir -t deb -C build -a amd64 --verbose --name splrg --version 0.9 --iteration $iteration -d libargtable2-0 .
