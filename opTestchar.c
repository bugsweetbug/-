#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define DATA_NUM (10)

int main()
{
    int fd, i;
    int WD, RD;
    fd_set fdset;
    char buf[DATA_NUM] = "1111";
    memset(buf, 0, DATA_NUM);
    fd = open("/dev/testchar", O_RDWR);
    printf("%d\r\n", fd);
    if(-1 == fd)
    {
        perror("open file error\r\n");
    }
    else
    {
        printf("open success\r\n");
    }
    WD = write(fd, buf, DATA_NUM);
    RD = read(fd, buf, DATA_NUM);
    printf("%d %d\r\n", WD, RD);
    printf("%s\r\n", buf);
    return 0;
}