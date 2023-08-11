#!/bin/bash

cfgTargetFilePattern="META-INF"
cfgWorkingRoot="./plugins"
cfgTable="    "
echo "pwd: `pwd`"

if [ -e ${cfgWorkingRoot} ]; then
	echo "processing"
else
	echo "${cfgWorkingRoot} not exist"
	exit
fi


if [ -e "./copy" ]; then
	rm -r ./copy
	echo "rm ./copy"
else
	echo "not exist ./copy" 
fi

mkdir ./copy
echo "mkdir ./copy"


if [ -e ./modulename.txt ]; then
	rm ./modulename.txt
	echo "rm ./modulename.txt"
else
	echo "not exist ./modulename.txt"
fi

touch ./modulename.txt
echo "touch ./modulename.txt"


# find $cfgWorkingRoot -type d -name ${cfgTargetFilePattern} -exec cp -rp {}/../src ./copy \;

find $cfgWorkingRoot -type d -name ${cfgTargetFilePattern} \
-exec echo {}/.. >> modulename.txt \;

moduleElementArray=$(awk '{print $1}' ./modulename.txt)
for element in ${moduleElementArray[*]}
do
	echo
	echo $element
	if [ -e ${element}/include ]; then
		cp -r --parents ${element}/include ./copy
		echo "${cfgTable}cp ${element}/include"
	fi
	if [ -e ${element}/src ]; then
		cp -r --parents ${element}/src ./copy
		echo "${cfgTable}cp ${element}/src"
	fi
	if [ -e ${element}/header ]; then
		cp -r --parents ${element}/header ./copy
		echo "${cfgTable}cp ${element}/header"
	fi
	if [ -e ${element}/startup ]; then
		cp -r --parents ${element}/startup ./copy
		echo "${cfgTable}cp ${element}/startup"
	fi
	if [ -e ${element}/build_files ]; then
		cp -r --parents ${element}/build_files ./copy
		echo "${cfgTable}cp ${element}/build_files"
	fi
	if [ -e ./copy/${element}/META-INF ]; then
		rm -d ./copy/${element}/META-INF
		echo "${cfgTable}${cfgTable}rm ./copy/${element}/META-INF"
	fi
done
