#!/bin/bash

echo "=============================================================================="

sourcedir=$1
cd $sourcedir
echo "sourcedir: ${sourcedir}"

targetFilePattern="*.[cs]"
echo "targetFilePattern: ${targetFilePattern}"

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

dirlist=$(list_alldir $sourcedir)
echo " " > filenames.txt
echo "" > deletedFile.txt
# 打印第9个字段(文件名)： >awk '/^-.*/{print tmpname1$9}' tmpname1="$tmpname"
#删除空行： >sed '/^ *$/d '
for element in ${sourcedir} ${dirlist[*]}
do
    # echo "element: $element"
    ls -l $element | awk '/^-.*/{print tmpname1$9}' tmpname1="$tmpname" | sed '/^ *$/d ' >> filenames.txt
    find $element -type f -name ${targetFilePattern} >> deletedFile.txt
    find $element -type f -name ${targetFilePattern} -delete
done


totalfileCount=$(cat filenames.txt | wc -l)
echo "totalfileCount: ${totalfileCount}"
deletedFileCount=$(cat deletedFile.txt | wc -l)
echo "deletedFileCount: ${deletedFileCount}"
echo "-----------------------------------------------------------------------------"
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
echo "-----------------------------------------------------------------------------"
echo "deleted file:"
cat ./deletedFile.txt