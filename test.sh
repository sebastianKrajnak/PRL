#!/bin/bash

mpic++ --prefix /usr/local/share/OpenMPI -o oems oems.cpp
dd if=/dev/random bs=1 count=8 of=numbers
mpirun --prefix /usr/local/share/OpenMPI -oversubscribe -np 8 oems
rm -f oems numbers