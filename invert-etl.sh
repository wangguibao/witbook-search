#!/bin/bash

if [ $# -ne 1 ]; then
	echo "invert-etl.sh INVERT_RECORD_FILE"
	exit
fi
sort -n -k2 $1
