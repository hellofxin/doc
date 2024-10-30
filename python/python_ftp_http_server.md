## FTP Server
ftp-server$ python -m pip install pyftpdlib

ftp-server$ python -m pyftpdlib -p 8088 -w -d /tmp/ -u fangx -P fangx@

ftp-client$ ftp localhost 8088


## HTTP Server
python -m http.server 8000 --directory /tmp/