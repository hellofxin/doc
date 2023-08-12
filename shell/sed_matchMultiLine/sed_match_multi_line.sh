# 首先找到aaa，然后执行大括号内的指令；
# 大括号内指令：
# 标记标签a的位置，读取下一行，执行指令（替换），然后如果没有找到eee，就back to 标签a的位置；
# sed -i '/aaa/{:a;n;s/123/xyz/g;/eee/!ba}' yourfile

# sourceFile=./Rte_COMCbk.c 
# sed -rn '/boolean ComIPduCallout/{:end; N; /}/!bend; s/}/\treturn TRUE\;\n}/g; p}' ${sourceFile}

sed -nr ':a; N; /\bTIME/!ba; /RFTIME/!p;' list.txt


