#1 ��ǰbranch׷��origin/master
git branch -u origin/master
git branch --set-upstream-to origin/master

# EOF
# ���ļ�test.sh���������ݡ�
 cat << EOF >test.sh 
> 123123123
> asdfasdfs
> EOF

cat test.sh 
123123123
asdfasdfs

# ׷������
cat << EOF >>test.sh 
> 7777
> 8888
> EOF

# ����ǰ�µĹ������͵�remote���µ�repo
# 1��remote���ȴ�����repo
# 2���ڵ�ǰ���̸�Ŀ¼��ִ�� git init
# 3������ git remote add origin url
# 4��Ȼ�� git pull origin master��master
# ���������½git ��ִ��pull �������ڱ�������origin/master ��master��֧

#git clone�󣬲���ֱ��git push��������user.email��user.name��Ȼ��ſ���git push������Ҳ�����ֶ�׷��origin branch���ƺ��Ѿ���׷�ٹ�ϵ�ˡ�

#20181202_1 track��ĳ�����뱻track���ļ�������ȡ��stage��
git rm --cached path/filename
# ȡ��׷��ĳ���ļ���
# ������ -r �����ݹ�rm��
git rm --cached -r path/

#20181202_2 ͨ��git init ������repo������pull��Ҳ����push��
# ��� remote��git remote add origin remoteRepoUrl��
# ���ڿ���git pull���������δ���õ�ǰbranch�� upstream������push��
# ͨ�� git config --list ����û��branch.master.remote��
# ִ��git push -u*�������õ�ǰbranch��upstream����push��
# Ҳ���Ե���������upstream��Ȼ��push�����#1��
git push -u origin master:master

#20181202_3 head->master��ʾlocal�汾�У�headָ��master��
# head->master, origin/master��ʾlocal�汾�У�headָ��master��origin/master;
# origin/master��remote��masterָ����local�汾�е�һ��ָ�룻
# ��ʱmaster��origin/masterָ��ͬһ��commit��

#������repo��remote repo������ϵ
git init
git remote add origin url
git config --global user.email "hellofxin@qq.com"
git pull origin master:master
git branch -u origin/master
git push