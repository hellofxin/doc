# д���ļ�
    ## ׷��д��
    echo text >> temp.txt
    ## ����д��
echo text > temp.txt
## �����ж���׷��д��
cat < EOF >> temp.txt
1
2
3
EOF


# xargs��pipe
echo '--help' | cat 
��� --help
echo '--help' | xargs cat 
��� usage balabala
ʵ���ϣ�echo '--help' | cat ����echo��������ͨ���ܵ�����cat�������ˣ���echo '--help' | xargs cat �ȼ��� cat --help ������xargs������ܵ��ַ��� --help ����cat��һ���������������cat���
    ͨ���ܵ������ַ������ݣ�ͨ���ܵ���xargs�ͽ��ܵ���������ݵ��������в�����
xargs����ѡ�
1. -d ѡ��
Ĭ�������xargs�����׼�����е������Կհ�(�����ո�Tab���س����е�)�ָ�ɶ��֮���������в������ݸ����������������֮�����ǿ���ʹ�� -d ����ָ���ָ��������磺
echo '11@22@33' | xargs echo 
�����
11@22@33
Ĭ��������Կհ׷ָ��ô11@22@33����ַ�����û�пհף�����ʵ���ϵȼ��� echo 11@22@33 �����ַ��� '11@22@33' ������echo�����һ�������в���

echo '11@22@33' | xargs -d '@' echo 
�����
11 22 33
ָ����@���ŷָ���������Եȼ��� echo 11 22 33 �൱�ڸ�echo������3���������ֱ���11��22��33

2. -p ѡ��
ʹ�ø�ѡ��֮��xargs����������ִ����������������������Ҫִ�е�����������(���������Լ����ݸ�����������в���)��ѯ���Ƿ�ִ�У����� y �ż���ִ�У�����ִ�С�

3. -n ѡ��
��ѡ���ʾ��xargs���ɵ������в�����ÿ�δ��ݼ�������������������ִ�У��������xargs�ӱ�׼�����ж������ݣ�Ȼ���Էָ����ָ�֮�����ɵ������в�����10����ʹ�� -n 3 ֮���ʾһ�δ��ݸ�xargs�����������3����������Ϊһ����10������������Ҫִ��4�Σ����ܽ��������ꡣ���磺

4. -E ѡ��е�ϵͳ��xargs�汾������-e  eof-str
��ѡ��ָ��һ���ַ�������xargs��������������в�����ʱ�����������-eָ���������в�������ֻ�Ὣ-eָ���������в���֮ǰ�Ĳ���(������-eָ�����������)���ݸ�xargs���������
echo '11 22 33' | xargs -E '33' echo 
�����
11 22
���Կ��������������3�������в��� 11��22��33 ����ʹ����-E '33' ��ʾ�ڽ������в��� 33 ֮ǰ�Ĳ������ݸ�ִ�е����33���������ݡ��ȼ��� echo 11 22 ����-Eʵ���������������ã���ʾֻȡxargs�����������в���ǰ���ĳЩ���ָ�����ִ�С�
ע�⣺-Eֻ����xargs��ָ��-d��ʱ����Ч�����ָ����-d�������ã�������-dָ������ʲô�ַ����ո�Ҳ���С�

