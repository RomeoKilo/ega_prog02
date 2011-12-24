#!/usr/bin/env sh

#
# Regression test :-)
#

#./Dijkstra ../../prog02/NYC.sp -s 0 -t 5 -a
echo "Test run 1"
./Dijkstra ../../prog02/FLA.sp -s 123456 -t 123456 -a  # d=0
echo "Test run 2"
./Dijkstra ../../prog02/NYC.sp -s 96008 -t 172944 -a     # d=900041
echo "Test run 3"
./Dijkstra ../../prog02/NYC.sp -s 1 -t 100 -a            # d=114015
echo "Test run 4"
./Dijkstra ../../prog02/NYC.sp -s 500 -t 170000 -a       # d=707022
echo "Test run 5"
./Dijkstra ../../prog02/NYC.sp -s 100000 -t 150000 -a    # d=506442
echo "Test run 6"
./Dijkstra ../../prog02/NYC.sp -s 5000 -t 100000 -a      # d=820921
echo "Test run 7"
./Dijkstra ../../prog02/NYC.sp -s 142674 -t 123876 -a    # d=406508
echo "Test run 8"
./Dijkstra ../../prog02/NYC.sp -s 3424 -t 43214 -a       # d=923412
echo "Test run 9"
./Dijkstra ../../prog02/NYC.sp -s 8797 -t 145784 -a       # d=461545
echo "Test run 10"
./Dijkstra ../../prog02/NYC.sp -s 75849 -t 164985 -a     # d=739783
echo "Test run 11"
./Dijkstra ../../prog02/NYC.sp -s 98748 -t 2347 -a       # d=847551
echo "Test run 12"
./Dijkstra ../../prog02/samples/unconnected.graph -s 0 -t 1 -a # Test with unconnected graph -> d=UNCONN
echo "Test run 13"
./Dijkstra ../../prog02/FLA.sp -s 0 -t 1 -a             # Test with larger graph
echo "Test run 14"
./Dijkstra ../../prog02/FLA.sp -s 0 -t 1070376 -a       # Test with unconnected nodes

