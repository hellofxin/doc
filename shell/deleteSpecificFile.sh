#!/bin/bash
stringSeparate="-----------------------------------------------------------------------------"
stringBegin="=============================================================================="
stringMark="##########################################################################################################################################"
echo "$stringBegin"
echo "$stringSeparate"

# 配置是否进入目标目录
cfgCD=false
echo "cfgCD: $cfgCD"
cfgTargetFilePattern="*.[c]"
echo "cfgTargetFilePattern: ${cfgTargetFilePattern}"
workingRoot=`pwd`
echo "workingRoot: $workingRoot"

echo "$stringSeparate"
echo "pwd: `pwd`"
sourcedir=$1
echo "sourcedir: ${sourcedir}"
if [ $cfgCD = true ]; then
    echo "cd: $sourcedir"
    cd $sourcedir
    workingRoot=.
    echo "workingRoot: $workingRoot"
else
    workingRoot=$sourcedir
    echo "workingRoot: $workingRoot"
fi
echo "pwd: `pwd`"


# get the all dir and subdir
list_alldir(){
    for file2 in `ls -a $1`
    do
        if [ x"$file2" != x"." -a x"$file2" != x".." ];then
            if [ -d "$1/$file2" ];then
                echo "$1/$file2"
                list_alldir "$1/$file2"
            fi
        fi
    done
}

dirlist=$(list_alldir $workingRoot)
# echo "dirlist: $dirlist"
echo " " > filenames.txt
echo "" > deletedFile.txt
# 打印第9个字段(文件名)： >awk '/^-.*/{print tmpname1$9}' tmpname1="$tmpname"
#删除空行： >sed '/^ *$/d '
for element in ${workingRoot} ${dirlist[*]}
do
    # echo "$stringSeparate"
    # echo "element: $element"
    # echo -e "ls $element:\n `ls $element`"
    ls -l $element | awk '/^-.*/{print tmpname1$9}' tmpname1="$tmpname" | sed '/^ *$/d ' >> filenames.txt
done

find $workingRoot -type f -name ${cfgTargetFilePattern} >> deletedFile.txt
find $workingRoot -type f -name ${cfgTargetFilePattern} -delete

echo "$stringSeparate"
totalfileCount=$(cat filenames.txt | wc -l)
echo "totalfileCount: ${totalfileCount}"
deletedFileCount=$(cat deletedFile.txt | wc -l)
echo "deletedFileCount: ${deletedFileCount}"
echo "$stringSeparate"
# 任意字符若干，.c .h .s .a结尾，打印第一个字段
# 任意字符若干，.cc 结尾，打印第一个字段
# 任意字符若干，.cpp 结尾，打印第一个字段
# 任意字符若干，.hh 结尾，打印第一个字段
array=$(
    awk '
        /.*\.[chsa]$/{print $1}
        /.*\.cc$/{print $1;}
        /.*\.cpp$/{print $1;}
        /.*\.hh$/{print $1;}
    ' filenames.txt)

for element in ${array[*]}
do
    echo $element
done

echo ""
echo "$stringSeparate"
echo "deleted file:"
cat ./deletedFile.txt