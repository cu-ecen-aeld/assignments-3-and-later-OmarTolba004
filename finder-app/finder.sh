#!/bin/bash

#check the number of enetered arugments
if [ $# -ne 2 ]
then
	 echo expected Number of arguments is 2, while entered is $#
	 echo please use the script with arguments: path to file system and search string
	 exit 1
fi
#storing commands arguments into variables
filesdir=$1
searchstr=$2

if ! [ -d $filesdir ]
then
	echo Entered path doesn\'t represent directory on the file system
	exit 1 
fi

# X is the number of files in the directory and all subdirectories 
X=$(find $filesdir -type f | wc -l)
# Y is the number of matching lines found in respective files,
Y=$(grep -r "$searchstr"  $filesdir | wc -l) 
echo The number of files are $X and the number of matching lines are $Y
