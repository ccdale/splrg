#!/bin/bash
fpm -s dir -t deb -C build -a amd64 --verbose --name splrg --version 0.9 -d libargtable2-0 .
