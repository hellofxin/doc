# sed -nr ':a;N;/TIME/!ba;/RFTIME/!p;' list.txt
sed -nr ':a;N;/\bTIME/!ba;/RFTIME/!p;' list.txt >> getList.txt