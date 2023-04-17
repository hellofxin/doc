#1 当前branch追踪origin/master
git branch -u origin/master
git branch --set-upstream-to origin/master

# EOF
# 向文件test.sh里输入内容。
 cat << EOF >test.sh 
> 123123123
> asdfasdfs
> EOF

cat test.sh 
123123123
asdfasdfs

# 追加内容
cat << EOF >>test.sh 
> 7777
> 8888
> EOF

# 将当前新的工程推送到remote端新的repo
# 1、remote端先创建空repo
# 2、在当前工程根目录下执行 git init
# 3、接着 git remote add origin url
# 4、然后 git pull origin master：master
# 输入密码登陆git 可执行pull 操作，在本地生成origin/master 和master分支

#git clone后，不能直接git push，先设置user.email和user.name，然后才可以git push，这里也不用手动追踪origin branch，似乎已经有追踪关系了。

#20181202_1 track了某个不想被track的文件，可以取消stage。
git rm --cached path/filename
# 取消追踪某个文件夹
# 增加了 -r 参数递归rm。
git rm --cached -r path/

#20181202_2 通过git init 创建的repo，不能pull，也不能push。
# 添加 remote。git remote add origin remoteRepoUrl；
# 现在可以git pull，但是如果未设置当前branch的 upstream，不能push；
# 通过 git config --list 发现没有branch.master.remote。
# 执行git push -u*可以设置当前branch的upstream，并push；
# 也可以单独先设置upstream，然后push，详见#1。
git push -u origin master:master

#20181202_3 head->master表示local版本中，head指向master。
# head->master, origin/master表示local版本中，head指向master和origin/master;
# origin/master是remote的master指针在local版本中的一个指针；
# 此时master和origin/master指向同一个commit。

#本地新repo与remote repo初次联系
git init
git remote add origin url
git config --global user.email "hellofxin@qq.com"
git pull origin master:master
git branch -u origin/master
git push