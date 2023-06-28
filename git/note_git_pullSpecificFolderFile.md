在进行项目开发的时候，有时候会有这样的需求那就是：我们只希望从Git仓库里取指定的文件或者文件夹出来。在SVN里面，这非常容易实现，因为SVN基于文件方式存储，而Git却是基于元数据方式分布式存储文件信息的，它会在每一次Clone的时候将所有信息都取回到本地，即相当于在你的机器上生成一个克隆版的版本库。因此在Git1.7.0以前，这无法实现，但是幸运的是在Git1.7.0以后加入了Sparse Checkout模式，这使得Check Out指定文件或者文件夹成为可能。

具体实现如下：

$mkdir project_folder
$cd project_folder
$git init
$git remote add -f origin <url>
上面的代码会帮助你创建一个空的本地仓库，同时将远程Git Server URL加入到Git Config文件中。 
接下来，我们在Config中允许使用Sparse Checkout模式：

$git config core.sparsecheckout true
接下来你需要告诉Git哪些文件或者文件夹是你真正想Check Out的，你可以将它们作为一个列表保存在.git/info/sparse-checkout文件中。 
例如：

$echo “libs” >> .git/info/sparse-checkout
$echo “apps/register.go” >> .git/info/sparse-checkout
$echo “resource/css” >> .git/info/sparse-checkout
最后，你只要以正常方式从你想要的分支中将你的项目拉下来就可以了：

$git pull origin master
具体您可以参考Git的Sparse checkout


实践：
mkdir gitPull
cd gitPull
# 本地新建空repo
git init
# 在remote端添加 origin repo
git remote add origin https://github.com/hellofxin/ReadingMeters
# sparse	英[spɑ:s]	稀少的
git config core.sparsecheckout true
# 将文件夹或文件名输入到.git/info/sparse-checkout
# echo "Comparing.cpp" > .git/info/sparse-checkout
# 将多个文件名换行写入.git/info/sparse-checkout EOF和EOF之间的内容原样输出到文件
(
cat << EOF
.gitignore
.gitattributes
EOF
) > .git/info/sparse-checkout
git pull origin master:master
#将日期时间写入datestr
datestr=$(date +%Y%m%d%H%m)
mkdir ../pull$datestr
# mv * ../pull$datestr/
# 删除路径，包括里面的文件，递归强制
# rm ../gitPull -rf

# 下面这个方法没有git pull方便
# git fetch origin master:localBranch
# 下面这一步是fetch的文件显示出来
# git checkout localBranch
