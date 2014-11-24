#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

struct ioctls_dev {
  struct cdev cdev;
  char *buf_ptr;
  loff_t *s_off;
} sdev;

dev_t dev_num;
int rwflag = 0;

#define BUFSIZE             (1024)

#define IOCTLS_MAGIC_NUM    (0x2F)

#define DRIVER_ON           (1)
#define DRIVER_OFF          (0)

#define IOCTLS_ACCESS_START _IO (IOCTLS_MAGIC_NUM, 1)
#define IOCTLS_ACCESS_END   _IO (IOCTLS_MAGIC_NUM, 2)
#define IOCTLS_DATA_START   _IO (IOCTLS_MAGIC_NUM, 3)
#define IOCTLS_DATA_END     _IO (IOCTLS_MAGIC_NUM, 4)

int ioctlS_open(struct inode *inode, struct file *filp)
{
  struct ioctls_dev *dev = container_of(inode->i_cdev, struct ioctls_dev, cdev);

  filp->private_data = dev;

  return 0;
}

int ioctlS_release(struct inode *inode, struct file *filp)
{
  return 0;
}

ssize_t ioctlS_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
  struct ioctls_dev *dev = filp->private_data;

  printk("[WRITE OFFSET]: %d\n", *off);
  *off += 100;

  if (rwflag) {
    printk("WRITE OPEN\n");
    return 0;
  }
  else {
    printk("WRITE FAILED\n");
    return 0;
  }
}

ssize_t ioctlS_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
  struct ioctls_dev *dev = filp->private_data;

  printk("[READ OFFSET]: %d\n", *off);
  *off += 103;

  if (rwflag) {
    printk("READ OPEN\n");
    return 0;
  }
  else {
    printk("READ FAILED\n");
    return 0;
  }
}

int ioctlS_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
  switch (cmd) {
    case IOCTLS_ACCESS_START:
      rwflag = DRIVER_ON;
      return DRIVER_ON;
    case IOCTLS_ACCESS_END:
      rwflag = DRIVER_OFF;
      return DRIVER_OFF;
    case IOCTLS_DATA_START:
      /* Clear buffer */
      break;
    case IOCTLS_DATA_END:
      /* Flush buffer to send message */
      break;
    default:
      return -EINVAL;
  }

  return 0;
}

struct file_operations ioctlS_ops = {
  .owner   = THIS_MODULE,
  .read    = ioctlS_read,
  .write   = ioctlS_write,
  .unlocked_ioctl   = ioctlS_ioctl,
  .open    = ioctlS_open,
  .release = ioctlS_release
};

static int ioctlS_init(void)
{
  int err;

  if (alloc_chrdev_region(&dev_num, 0, 1, "ioctlS")) {
    printk("[IOCTLS] Dynamic register failed\n");
    return -1;
  }

  sdev.buf_ptr = kmalloc(BUFSIZE, GFP_KERNEL);
  cdev_init(&sdev.cdev, &ioctlS_ops);
  err = cdev_add(&sdev.cdev, dev_num, 1);

  if (err) {
    printk("[IOCTLS] cdev_add failed\n");
    return -1;
  }

  printk("======= [IOCTLS] Module Installed =======\n");

  return 0;
}

static void ioctlS_exit(void)
{
  unregister_chrdev_region(dev_num, 1);

  kfree(sdev.buf_ptr);
  cdev_del(&sdev.cdev);

  printk("======= [IOCTLS] Module Removed =======\n");
}

module_init(ioctlS_init);
module_exit(ioctlS_exit);
MODULE_LICENSE("GPL");

