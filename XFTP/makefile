all:XFTP
SRC=ev_ftp_server.cpp XThreadPool.cpp XThread.cpp XFtpServerCMD.cpp\
XFtpFactory.cpp XFtpLIST.cpp XFtpPORT.cpp XFtpRETR.cpp XFtpSTOR.cpp\
XFtpTask.cpp XFtpUSER.cpp
XFTP:$(SRC)
	g++ $^ -o $@ -std=c++14 -I/opt/libevent/include -L/opt/libevent/lib -levent -lpthread 
clean:
	rm -rf XFTP
	rm -rf *.o

