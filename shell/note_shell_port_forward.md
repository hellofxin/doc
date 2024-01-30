ssh -fCNg -L <listen port>:<remote ip>:<remote port> user@<ssh server> -p <ssh server port>
VPS$: ssh -fCNg -L 1153:192.168.190.136:3389 root@192.168.174.133 -p 22

linux1:	VPS: 192.168.190.110:1153
linux2:	WEB: 192.168.174.133
win7:	SQL: 192.168.190.136:3389
win10:	ABC: 192.168.190.132


ssh -fNg -R <listen port>:<remote ip>:<remote port> user@<SSH server> -p <ssh server port>
WEB$: ssh -fCNg -R 1521:192.168.190.136:3389 root@192.168.190.110

VPS$: ssh fCNg -D 7000 root@192.168.174.133