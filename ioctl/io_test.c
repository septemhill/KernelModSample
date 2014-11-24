#include <stdio.h>
#include <linux/ioctl.h>
#include <fcntl.h>

#define IOCTLS_MAGIC_NUM    (0x2F)
#define IOCTLS_ACCESS_START _IO (IOCTLS_MAGIC_NUM, 1)
#define IOCTLS_ACCESS_END   _IO (IOCTLS_MAGIC_NUM, 2)
#define IOCTLS_DATA_START   _IO (IOCTLS_MAGIC_NUM, 3)
#define IOCTLS_DATA_END     _IO (IOCTLS_MAGIC_NUM, 4)


int main(int argc, char **argv)
{
  int fd = open("/dev/ioctlS", O_RDWR);
  char buf[200];

  if (!fd) {
    printf("ERROR\n");
  }

  ioctl(fd, IOCTLS_ACCESS_START, NULL);

  read(fd, buf, 10);

  ioctl(fd, IOCTLS_ACCESS_END, NULL);


  write(fd, "Hi, Septem", 10);
  close(fd);

  return 0;
}
