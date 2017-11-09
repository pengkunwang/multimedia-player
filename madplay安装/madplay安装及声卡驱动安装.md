## madplay安装及声卡驱动安装

参考链接：[madplay 音乐播放器移植](https://yq.aliyun.com/articles/10724)

madplay官网: [**madplay官网**](http://www.underbit.com/products/mad)	

​			 [**zlib**官网](http://www.zlib.net/)



### 1. zlib、libid3tag、libmad、madplay安装:

### （可用sudo）

​	`./configure	`

​	` /make`

​	`make install`

### 2、配置

​	利用 `ldd madplay`查看madplay所依赖的库，所有库的默认安装位置在:/usr/local/lib，根据实际情况，将库复制到 /usr/ib。

​	然后在 `ldd madplay`查看是否可以运行，如运行执行一下：

​	修改系统文件/etc/ld.so.conf ，在文件尾添加/usr/lib

​	执行 `sudo ldconfig`



### 3、debian 下的驱动设置

​	`sudo mknod /dev/dsp c 14 3`

​	` sudo chmod 666 /dev/dsp` 

​	`sudo modprobe snd-pcm-oss`

​	`sudo modprobe snd-mixer-oss`

参考链接：http://blog.csdn.net/tq384998430/article/details/54599234



### 4、madplay使用方法



`system("madplay north.mp3 &");`//利用system函数调用madplay播放器播放*.mp3音乐

`system("madplay north.mp3 -r &");`//循环播放：参数-r

`system("killall -9 madplay");`//利用system函数调用killall命令将madplay终止掉 

`system("killall -STOP madplay &");`//利用system函数调用killall命令将madplay暂停

`system("killall -CONT madplay &");`//利用system函数调用killall命令恢复madplay的播放