#!/bin/sh

io_log=$1
sudo rm -f $io_log

iostat -x -p 1 >& $io_log

