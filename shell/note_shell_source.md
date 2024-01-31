经常见到source .bashrc, 这是执行.bashrc这个文件中的命令

## 是什么
source 命令也称为点命令，就是这个(.)，是bash内部命令。
功能：使shell读入指定文件，依次执行命令语句。
用法：source filename 或 .filename(没有/)

## 区别之source filename、sh filename、 ./filename
source filename 与 ./filename没有区别。
sh filename 会创建子shell执行脚本，子shell继承父shell环境变量，子shell新增和改变的变量不会带回父shell，除非使用export。
source file直接读取脚本执行命令，不会创建子shell，改变的量保存在当前环境中。

## 登录linux执行初始化文件
\# 系统环境变量
/etc/profile

~/.bash_profile | ~/.bash_longin | ~/.profile


~/.bashrc

\# 用户环境变量
/etc/bashrc

~/.bash_logout