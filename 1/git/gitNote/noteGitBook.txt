这是看gitBook的一些笔记。


0  引言

0.1 文件状态
    Git 中所有数据在存储前都计算校验和，然后以校验和来引用，而不是文件名。
     Git 有三种状态，你的文件可能处于其中之一：已提交（committed）、已修改（modified）和已暂存（staged）。 
    已提交表示数据已经安全的保存在本地数据库中。 已修改表示修改了文件，但还没保存到数据库中。 已暂存表示对一个已修改文件的当前版本做了标记，使之包含在下次提交的快照中。
    由此引入 Git 项目的三个工作区域的概念：Git 仓库、工作目录以及暂存区域。
    Git 仓库目录是 Git 用来保存项目的元数据和对象数据库的地方。 这是 Git 中最重要的部分，从其它计算机克隆仓库时，拷贝的就是这里的数据。
    工作目录是对项目的某个版本独立提取出来的内容。 这些从 Git 仓库的压缩数据库中提取出来的文件，放在磁盘上供你使用或修改。
    暂存区域是一个文件，保存了下次将提交的文件列表信息，一般在 Git 仓库目录中。 有时候也被称作索引，不过一般说法还是叫暂存区域。
    基本的 Git 工作流程如下：
        1. 在工作目录中修改文件。
        2. 暂存文件，将文件的快照放入暂存区域。
        3. 提交更新，找到暂存区域的文件，将快照永久性存储到 Git 仓库目录。
    如果 Git 目录中保存着的特定版本文件，就属于已提交状态。 如果作了修改并已放入暂存区域，就属于已暂存状态。 如果自上次取出后，作了修改但还没有放到暂存区域，就是已修改状态。

0.2 git配置
    如果想要检查你的配置，可以使用 git config --list 命令来列出所有 Git 当时能找到的配置。
    你可以通过输入 git config <key>： 来检查 Git 的某一项配置。


1 git基础

1.1 git基础_内容提示
     本章内容涵盖你在使用 Git 完成各种工作中将要使用的各种基本命令。 在学习完本章之后，你应该能够配置并初始化一个仓库（repository）、开始或停止跟踪(track）文件、暂存（stage）或提（commit)更改。

1.2 git基础_获取git仓库
    有两种取得 Git 项目仓库的方法。 第一种是在现有项目或目录下导入所有文件到 Git 中； 第二种是从一个服务器克隆一个现有的 Git 仓库。
1.2.1 在现有目录中初始化仓库
    $ git init
    该命令将创建一个名为 .git 的子目录，这个子目录含有你初始化的 Git 仓库中所有的必须文件，这些文件是Git 仓库的骨干。 但是，在这个时候，我们仅仅是做了一个初始化的操作，你的项目里的文件还没有被跟踪。
     你可通过 git add 命令来实现对指定文件的跟踪，然后执行 git commit 提交。
1.2.2 克隆现有的仓库
    克隆仓库的命令格式是 git clone [url] 。
    $ git clone https://github.com/libgit2/libgit2
    这会在当前目录下创建一个名为 “libgit2” 的目录，并在这个目录下初始化一个 .git 文件夹，从远程仓库拉取下所有数据放入 .git 文件夹，然后从中读取最新版本的文件的拷贝。

1.3 git基础_提交更新
    要查看哪些文件处于什么状态，可以用 git status 命令。 
    使用命令 git add 开始跟踪一个文件。
1.3.1 暂存已修改文件
    对一个文件readme.md执行commit->modify->add->modify，然后git status，会发现这个文件出现在 to be committed 和  not staged两个地方，因为暂存后又修改了，工作区文件比暂存区文件更加新，如果提交，则提交的是add时的文件版本，如需提交最新的modify后的版本，先add 再commit。
1.3.2 查看已暂存和未暂存的修改
    如果 git status 命令的输出对于你来说过于模糊，你想知道具体修改了什么地方，可以用 git diff 命令。能通常会用它来回答这两个问题：当前做的哪些更新还没有暂存？ 有哪些更新已经暂存起来准备好了下次提交？ 尽管 git status 已经通过在相应栏下列出文件名的方式回答了这个问题，git diff 将通过文件补丁的格式显示具体哪些行发生了改变。
    diff --git a/readme.MD b/readme.MD
    index 15a7d11..f875aba 100644
    --- a/readme.MD
    +++ b/readme.MD
    @@ -1,3 +1,4 @@
     # temp //表示修改的文件是temp
     commit->modify
     commit->modify->add->modify
    +git diff (between not staged & staged)  //表示增加了行
    此命令比较的是工作目录中当前文件和暂存区域快照之间的差异， 也就是修改之后还没有暂存起来的变化内容。
    若要查看已暂存的将要添加到下次提交里的内容，可以用 git diff --cached /git diff --staged命令。

1.4 git基础_移除文件
1.4.1 彻底从git和仓库中移除
    要从 Git 中移除某个文件，就必须要从已跟踪文件清单中移除（确切地说，是从暂存区域移除），然后提交。
    可以用 git rm 命令完成此项工作，并连带从工作目录中删除指定的文件，这样以后就不会出现在未跟踪文件清单中了。
    如果只是简单地从工作目录中手工删除文件，运行 git status 时就会在 “Changes not staged for
commit” 部分出现deleted: filename。然后git rm filename, Changes to be committed:出现deleted: filename。
    下一次提交时，该文件就不再纳入版本管理了。
     如果删除之前修改过并且已经放到暂存区域的话，则必须要用强制删除选项 -f
1.4.2 仅从git中移除，但保留在磁盘上，也即文件仍然存在于工作区
    我们想把文件从 Git 仓库中删除（亦即从暂存区域移除），但仍然希望保留在当前工作目录中。 换句话说，你想让文件保留在磁盘，但是并不想让 Git 继续跟踪。 
    $ git rm --cached README

1.5 git基础_查看提交历史
    在提交了若干更新，又或者克隆了某个项目之后，你也许想回顾下提交历史。 完成这个任务最简单而又有效的工具是 git log 命令。
    选项。一个常用的选项是 -p，用来显示每次提交的内容差异。 你也可以加上 -2 来仅显示最近两次提交。
    --stat 选项在每次提交的下面列出所有被修改过的文件、有多少文件被修改了以及被修改过的文件的哪些行被移除或是添加了。 在每次提交的最后还有一个总结。

1.6 git基础_撤销操作
    vim 写：I；
    vim 退出：esc->:->x；
    $ git commit --amend
    这个命令会将暂存区中的文件提交。 如果自上次提交以来你还未做任何修改（例如，在上次提交后马上执行了此命令），那么快照会保持不变，而你所修改的只是提交信息。
1.6.1 取消暂存的文件
     git reset HEAD <file>
    用commit文件版本替换staged文件版本，使暂存区文件恢复。
1.6.2 撤销对文件的修改
    git checkout -- [file]
    用staged文件版本替换工作区文件版本。你只是拷贝了另一个文件来覆盖它。

1.7 git基础_远程仓库
1.7.1 查看远程仓库
    以运行 git remote 命令，它会列出你指定的每一个远程服务器的简写。 
    指定选项 -v，会显示需要读写远程仓库使用的 Git 保存的简写与其对应的 URL。
    如果想要查看某一个远程仓库的更多信息，可以使用 git remote show [remote-name] 命令。
1.7.2 添加远程仓库
    git remote add <shortname> <url> 添加一个新的远程 Git 仓库，同时指定一个你可以轻松引用的简写。
     git fetch pb
    现在 Paul 的 master 分支可以在本地通过 pb/master 访问到 - 你可以将它合并到自己的某个分支中，或者如果你想要查看它的话，可以检出一个指向该点的本地分支。
1.7.3 从远程仓库抓取
    $ git fetch [remote-name]
    这个命令会访问远程仓库，从中拉取所有你还没有的数据。 执行完成后，你将会拥有那个远程仓库中所有分支的引用，可以随时合并或查看。 git fetch 命令会将数据拉取到你的本地仓库 - 它并不会自动合并或修改你当前的工作。 
    如果你使用 clone 命令克隆了一个仓库，命令会自动将其添加为远程仓库并默认以 “origin” 为简写。 git clone 命令会自动设置本地 master 分支跟踪克隆的远程仓库的 master 分支。
    如果你有一个分支设置为跟踪一个远程分支，可以使用 git pull 命令来自动的抓取然后合并远程分支到当前分支。
1.7.4 推送到远程分支
    $ git push origin master:master
1.7.5 远程仓库移除与重命名
     git remote rename oldname newname
    值得注意的是这同样也会修改你的远程分支名字。那些过去引用 pb/master 的现在会引paul/master。
     git remote rm paul

1.8 git基础_标签
1.8.1 列出标签
    $ git tag
    你也可以使用特定的模式查找标签。 例如，Git 自身的源代码仓库包含标签的数量超过 500 个。 如果只对 1.8.5系列感兴趣，可以运行：
        $ git tag -l 'v1.8.5*'
1.8.2 创建标签
ear
Git 使用两种主要类型的标签：轻量标签（lightweight）与附注标签（annotated）。
    一个轻量标签很像一个不会改变的分支 - 它只是一个特定提交的引用。
    附注标签是存储在 Git 数据库中的一个完整对象。 它们是可以被校验的；其中包含打标签者的名字、电子邮件地址、日期时间；还有一个标签信息；并且可以使用 GNU Privacy Guard （GPG）签名与验证。 
    在 Git 中创建一个附注标签是很简单的。 最简单的方式是当你在运行 tag 命令时指定 -a 选项：
        $ git tag -a v1.4 -m 'my version 1.4'
     git show 命令可以看到标签信息与对应的提交信息：
        $ git show v1.4
    轻量标签本质上是将提交校验和存储到一个文件中 - 没有保存任何其他信息。 创建轻量标签，不需要使用 -a、-s 或 -m 选项，只需要提供标签名字：
        $ git tag v1.4-lw
    后期打标签：
    $ git tag -a v1.2 9fceb02
    默认情况下，git push 命令并不会传送标签到远程仓库服务器上。 在创建完标签后你必须显式地推送标签到共享服务器上。 这个过程就像共享远程分支一样 - 你可以运行 git push origin [tagname]。


2 git分支

2.1 git分支_简介
    在进行提交操作时，Git 会保存一个提交对象（commit object）。该提交对象会包含一个指向暂存内容快照的指针。 但不仅仅是这样，该提交对象还包含了作者的姓名和邮箱、提交时输入的信息以及指向它的父对象的指针。首次提交产生的提交对象没有父对象，普通提交操作产生的提交对象有一个父对象，而由多个分支合并产生的提交对象有多个父对象，
    当使用 git commit 进行提交操作时，Git 会先计算每一个子目录（本例中只有项目根目录）的校验和，然后在Git 仓库中这些校验和保存为树对象。 随后，Git 便会创建一个提交对象，它除了包含上面提到的那些信息外，还包含指向这个树对象（项目根目录）的指针。
    blob 对象（保存着文件快照）。
    树对象（记录着目录结构和 blob 对象索引）。
    提交对象（包含着指向树对象的指针和所有提交信息，还可能包括一个或几个commit对象指针，也就是parent指针）。
    Git 的分支，其实本质上仅仅是指向提交对象的可变指针。 Git 的默认分支名字是 master。 在多次提交操作之后，你其实已经有一个指向最后那个提交对象的 master 分支。 它会在每次的提交操作中自动向前移动。

2.2 分支 创建
    $ git branch branchTest
    git branch 命令仅仅 创建 一个新分支，并不会自动切换到新分支中去。
    那么，Git 又是怎么知道当前在哪一个分支上呢？ 也很简单，它有一个名为 HEAD 的特殊指针。它是一个指针，指向当前所在的本地分支。

2.3 git分支_切换
    $ git checkout branchTest
    将branchTest分支内容提取到工作区，也就是分支切换。原理和恢复修改过的文件相似。
    $ git checkout master
    这条命令做了两件事。 一是使 HEAD 指回 master 分支，二是将工作目录恢复成 master 分支所指向的快照内容。 也就是说，你现在做修改的话，项目将始于一个较旧的版本。 本质上来讲，这就是忽略 testing 分支所做的修改，以便于向另一个方向进行开发。

2.4 git分支_新建与合并
2.4.1 新建分支
    git checkout -b iss53
    相当于：
        $ git branch iss53
        $ git checkout iss53  
2.4.2 合并分支
    $ git checkout master
    $ git merge hotfix
    合并分支B到分支A，先切换到分支A，执行git merge B，将分支B合并到分支A。
    $ git branch -d hotfix
2.4.3 遇到冲突时的分支合并
    有时候合并操作不会如此顺利。 如果你在两个不同的分支中，对同一个文件的同一个部分进行了不同的修改，Git 就没法干净的合并它们。 
    Git 做了合并，但是没有自动地创建一个新的合并提交。 Git 会暂停下来，等待你去解决合并产生的冲突。你可以在合并冲突后的任意时刻使用 git status 命令来查看那些因包含合并冲突而处于未合并
（unmerged）状态的文件。
    出现冲突，取消merge：git merge --abort
    出现冲突，修改冲突，对每个文件使用 git add 命令来将其标记为冲突已解决，如果你对结果感到满意，并且确定之前有冲突的的文件都已经暂存了，这时你可以输入 git commit 来完成合并提交。

2.5 git分支_管理
    --merged 与 --no-merged 这两个有用的选项可以过滤这个列表中已经合并或尚未合并到当前分支的分支。如果要查看哪些分支已经合并到当前分支，可以运行 git branch --merged：
    因为之前已经合并了 iss53 分支，所以现在看到它在列表中。 在这个列表中分支名字前没有 * 号的分支通常可以使用 git branch -d 删除掉；你已经将它们的工作整合到了另一个分支，所以并不会失去任何东西。
2.5.1 推送
    $ git push origin serverfix
    推送本地的 serverfix分支来更新远程仓库上的 serverfix 分支。
    git push origin serverfix:serverfix
    它会做同样的事 - 相当于它说，“推送本地的 serverfix 分支，将其作为远程仓库的 serverfix 分支” 可以通过这种格式来推送本地分支到一个命名不相同的远程分支。
    下一次其他协作者从服务器上抓取数据时，他们会在本地生成一个远程分支 origin/serverfix，指向服务器的 serverfix 分支的引用：要特别注意的一点是当抓取到新的远程跟踪分支时，本地不会自动生成一份可编辑的副本（拷贝）。 换一句话说，这种情况下，不会有一个新的 serverfix 分支 - 只有一个不可以修改的 origin/serverfix 指针。
    可以运行 git merge origin/serverfix 将这些工作合并到当前所在的分支。
     如果想要在自己的serverfix 分支上工作，可以将其建立在远程跟踪分支之上：
    $ git checkout -b serverfix origin/serverfix
    这会给你一个用于工作的本地分支，并且起点位于 origin/serverfix。
2.5.2 跟踪分支
    1）从一个远程跟踪分支检出一个本地分支会自动创建一个叫做 “跟踪分支”。跟踪分支是与远程分支有直接关系的本地分支。
    !!! checkout追踪新分支，也即新建分支并跟踪某分支：
    git checkout -b [branch] [remotename]/[branch]。
    2）当克隆一个仓库时，它通常会自动地创建一个跟踪 origin/master 的 master 分支。 
    3）Git 提供了 --track 快捷方式：
        !!! checkout追踪已有分支：
        $ git checkout --track origin/serverfix
    4）设置已有的本地分支跟踪一个刚刚拉取下来的远程分支，或者想要修改正在跟踪的上游分支，你可以在任意时间使用 -u 或 --set-upstream-to 选项运行 git branch 来显式地设置。
        !!! $ git branch -u origin/serverfix
    如果想要查看设置的所有跟踪分支，可以使用 git branch 的 -vv 选项。
2.5.3 删除远程分支
    假设你已经通过远程分支做完所有的工作了 - 也就是说你和你的协作者已经完成了一个特性并且将其合并到了远程仓库的 master 分支。可以运行带有 --delete 选项的 git push 命令来删除一个远程分支。 如果想要从服务器上删除 serverfix 分支，运行下面的命令:
        $ git push origin --delete serverfix
    基本上这个命令做的只是从服务器上移除这个指针。

2.6 git分支_变基
    在 Git 中整合来自不同分支的修改主要有两种方法：merge 以及 rebase。 
2.6.1 变基的基本操作
    整合分支最容易的方法是 merge 命令。 它会把两个分支的最新快照（C3 和 C4）以及二者最近的
共同祖先（C2）进行三方合并，合并的结果是生成一个新的快照（并提交）。
    其实，还有一种方法：你可以提取在 C4 中引入的补丁和修改，然后在 C3 的基础上应用一次。 在 Git 中，这种操作就叫做 变基。 
    将分支 experiment 变基到 master：
        $ git checkout experiment
        $ git rebase master
    先到源分支，执行变基，将源分支变基到目标分支。
    然后，回到 master 分支，进行一次快进合并。
    它的原理是首先找到这两个分支（即当前分支 experiment、变基操作的目标基底分支 master）的最近共同祖先 C2，然后对比当前分支相对于该祖先的历次提交，提取相应的修改并存为临时文件，然后将当前分支指向目标基底 C3, 最后以此将之前另存为临时文件的修改依序应用。
2.6.2 变基的风险
    不要对在你的仓库外有副本的分支执行变基。
    总的原则是，只对尚未推送或分享给别人的本地修改执行变基操作清理历史，从不对已推送至别处的提交执行变基操作，这样，你才能享受到两种方式带来的便利。

5 git工具

5.7 git工具重置
    HEAD~是HEAD的父节点。
    reset和checkout操作的是三棵树：HEAD，index，working directory。
    git reset SHA(默认--mixed），用SHA对应的快照（仓库）恢复index（暂存区）；
    git reset SHA --hard，用SHA对应的快照（仓库）恢复index（暂存区），然后用index恢复working directory（工作区）；
    git reset SHA --soft,仅仅移动HEAD，不恢复index，不恢复working directory。
     当检出一个分支时，它会修改 HEAD 指向新的分支引用，将 索引 填充为该次提交的快照，然后将 索引 的内容复制到 工作目录 中。
5.7.1 reset过程：
        1）移动HEAD指向（--soft）；--soft到此结束。
        2）更新索引（--mixed），reset 会用 HEAD 指向的当前快照的内容来更新索引，mixed到此结束。
        3）更新工作目录（--hard），reset 要做的的第三件事情就是让工作目录看起来像索引。 --hard到此结束。
    reset HEAD~后，我们的 Git 数据库中的一个提交内还留有该文件的 v3 版本，我们可以通过 reflog 来找回它。
    reset 带路径：
    前面讲述了 reset 基本形式的行为，不过你还可以给它提供一个作用路径。 若指定了一个路径，reset 将会跳过第 1 步，并且将它的作用范围限定为指定的文件或文件集合。 可以部分更新索引和工作目录 。
     git reset file.txt
    上面命令跳过移动HEAD，用HEAD对应的index恢复index，到此为止。
     git reset SHA file.txt，从SHA对应的index恢复index。
5.7.2 checkout
    1）checkout 不带路径
    git checkout SHA，只是切换分支。
    2）checkout 带路径
    git checkout SHA file
    用指定分支的文件的快照恢复index，同时恢复working directory。
5.7.3 reset&checkout 总结
    commit level：
        reset --soft [commit]，表示移动HEAD引用，不改变index和workDir;
        reset --mixed [commit], 表示移动HEAD引用，恢复index，不改变workDir;
        reset --hard [commit], 表示移动HEAD引用，恢复index和workDir。
        checkout [commit], 表示切换HEAD引用。
    file level:
        reset (commit) [file], 表示用commit->file恢复index，不恢复workDir，不移动或切换HEAD；
        checkout commit) [file], 表示用commit->file恢复index，同时恢复workDir，不移动或切换HEAD。
    重点区别reset (commit) [file]和checkout (commit) [file]。


6 git内部原理

6.1
    


    
    

