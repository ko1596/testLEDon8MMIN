#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/* ./key_led /dev/input/event1 noblock */
int main(int argc, char **argv)
{
    int fd, bg_fd;
    int err, len, i;
    unsigned char flag;
    unsigned int data[1];
    char *bg = "/sys/class/leds/user/brightness";
    struct input_event event;
    if (argc < 2)
    {
        printf("Usage: %s <dev> [noblock]\n", argv[0]);
        return -1;
    }
    if (argc == 3 && !strcmp(argv[2], "noblock"))
    {
        fd = open(argv[1], O_RDWR | O_NONBLOCK);
    }
    else
    {
        fd = open(argv[1], O_RDWR);
    }
    if (fd < 0)
    {
        printf("open %s err\n", argv[1]);
        return -1;
    }
    while (1)
    {
        len = read(fd, &event, sizeof(event));
        if (event.type == EV_KEY)
        {
            if (event.value == 1) //key down and up
            {
                printf("key test \n");
                bg_fd = open(bg, O_RDWR);
                if (bg_fd < 0)
                {
                    printf("open %d err\n", bg_fd);
                    return -1;
                }
                read(bg_fd, &flag, 1);
                if (flag == '0')
                    system("echo 1 > /sys/class/leds/user/brightness"); //led on
                else
                    system("echo 0 > /sys/class/leds/user/brightness"); //led off
            }
        }
    }
    return 0;
}