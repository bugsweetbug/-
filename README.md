charDev.c：应用层调用open、close调用驱动的testOpen、testClose
opTestchar.c:对应charDev.c的应用层
charLed.c：应用层调用write、ioctl调用驱动的testWrite、testIoctl操纵led（测试时需要关闭内核led驱动）
writeLed.c/ioctlLed.c对应charLed.c的应用层
