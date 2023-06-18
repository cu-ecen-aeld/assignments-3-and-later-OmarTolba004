#!/bin/bash
if [ $# -ne 2 ]
then
	echo excptected number of arguments are 2
	exit 1 
fi

writefile=$1
writestr=$2

install -D /dev/null $writefile
echo $writestr | cat > $writefile

