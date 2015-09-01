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
mkdir -p build/etc/init.d build/usr/bin
cp splrgd build/usr/bin/splrgd
cp splrgd.conf build/etc/splrgd.conf
cp init-splrgd.sh build/etc/init.d/splrgd
if [ -r build/etc/init.d/splrgd -a -r build/etc/splrgd.conf -a -r build/usr/bin/splrgd ]; then
  fpm -s dir -t deb -C build -a amd64 --verbose --name splrg --version 0.9-${iteration} -d libargtable2-0 --config-files /etc/splrgd.conf .
else
  echo "failed to build package for some reason or other"
fi
