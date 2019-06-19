// -t type rsa, -C optional comment 
ssh-keygen -t rsa -C "hellofxin@qq.com"
// start ssh program ssh-agent
ssh-agent $SHELL
// add id_rsa to ssh-agent
ssh-add /d/doc/.ssh/id_rsa
// connect
ssh -T git@github.com
//git pull through ssh
git pull git@github.com:userName/repository.git
// note that you might need to add id_rsa to ssh-agent everytime you start git-bash
// 如果因为本地没有repo而pull失败，则先clone
git clone git@github.com:hellofxin/folder.git
