
## ssh

### local forward
1 
```
$A: ssh bUser@bHost -fCNL aHost:aPort: bHost:bPort
```
先登录 b host， 然后将a port 映射到 b port；
实现的效果： 访问 aHost:aPort 就相当于访问 bHsot:bPort

2 
如果 bHost 还能访问 cHost, 那么还可以将 aPort通过 bPort 映射到 cPort
```
$A: ssh bUser@bHost -fCNL aHost:aPort: cHost:cPort
```
先登录 bHost，然后将 aPort转发到 cPort
实现的效果： 访问 aHost:aPort 就相当于访问 cHsot:cPort

### remote forward
1
```
$A: ssh bUser@bHost -fCNR bHost:bPort: aHost:aPort
```
先登录 bHsot，然后将 bPort 转发到 aPort
实现的效果： 访问 bHost:bPort 就相当于访问 aHsot:aPort

2 
如果 aHost能访问 cHost，那么还可以将 bPort 通过 aPort 转发到 cPort
```
$A: ssh bUser@bHost -fCNR bHost:bPort: cHost:cPort
```
先登录 bHsot，然后将 bPort 转发到 cPort
实现的效果： 访问 bHost:bPort 就相当于访问 cHsot:cPort
此时，cHost的sshd config 要配置 GatewayPorts yes
反向代理常用来实现内网穿透，如a是内网树莓派，b是公网云服务器，在a执行反向代理，访问b就相当于访问a，实现内网穿透，一般还需要在b上开启一个正向代理。
示例如下
```
$A: ssh bUser@bHost -fCNR bHost:bPort1:aHost:aPort
$B: ssh localhost -fCNL *:bPort2:localhost:bPort1
#此时要在cHost使用aUser登录bHost
$C: ssh aUser@bHost -p bPort2
```

### dynamic forward
```
$A: ssh bUser@bHost -D localhost:8080
```
先登录bHost，然后将通过localhost:8080端口的数据全部转发到bHost,bPort会自动选择，再从bHost发送到正确的服务器
