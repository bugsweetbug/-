#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int on;
    int led_no;
    int fd;
    int ret;
    char allon[4] = {1, 1, 1, 1};
    char alloff[4] = {0, 0, 0, 0};

    fd = open("/dev/leds0", O_RDWR);
    if(fd < 0)
    {
        fd = open("/dev/leds", O_RDWR);
    }
    if(fd < 0)
    {
        perror("open device leds");
        exit(1);
    }

    ret = write(fd, allon, 4);
    if(ret == -1)
    {
        perror("Write device failed");
        exit(1);
    }
    sleep(5);
    ret = write(fd, alloff, 4);
    if(ret == -1)
    {
        perror("Write device failed");
        exit(1);
    }
    close(fd);
    return 0;
}