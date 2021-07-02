# FtpServer 基于libevent的高性能ftp服务器

(学习实践)


⚠️.使用<br>
  --
  -使用环境中正常构建libevent<br>
  -修改makefile中的头文件和动态库路径<br>

🌟.线程池<br>
--
  每个线程对应一个eventloop，采用管道事件激活线程提取任务
🌟.单例模式  工厂模式<br>
--
  面对对象
🌟.事件驱动<br>
--
  利用libevent的事件驱动，实现IO复用达到高性能
<br>
<br>
✅(遵守rfc959协议，使用相同协议的客户端即可连接)<br>
<br>
✨libevent : https://libevent.org<br>
  libevent是一个跨平台的网络库，稳定易用高性能<br>
<br>
🐝todo<br>
  -添加删除文件功能<br>
