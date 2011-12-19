#!/usr/bin/env sh

#
# Regression test :-)
#

./spp_cpp ../../prog02/NYC.sp -s 0 -t 5 -a
./spp_cpp ../../prog02/NYC.sp -s 96008 -t 172944 -a     # d=900041
./spp_cpp ../../prog02/NYC.sp -s 1 -t 100 -a            # d=114015
./spp_cpp ../../prog02/NYC.sp -s 98748 -t 2347 -a       # d=847551
./spp_cpp ../../prog02/NYC.sp -s 75849 -t 164985 -a     # d=739783
./spp_cpp ../../prog02/samples/unconnected.graph -s 0 -t 1 -a
