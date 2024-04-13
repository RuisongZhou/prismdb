#!/bin/sh

io_log=$1
sudo rm -f $io_log

while true
do
        iostat -x -p >> $io_log
        sleep 1
done