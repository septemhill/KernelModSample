#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>

dev_t dev_num;
struct cdev cdev;

extern int extedvar;

int varexter_open(struct inode *indoe, struct file *filp)
{
  return 0;
}

int varexter_release(struct inode *inode, struct file *filp)
{
  return 0;
}

ssize_t varexter_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
  extedvar += 100;
  printk("[VAREXTER PLUS 100]: %d\n", extedvar);
  return 0;
}

ssize_t varexter_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
  extedvar -= 4;
  printk("[VAREXTER MINUS 4]: %d\n", extedvar);
  return 0;
}

struct file_operations varexter_ops = {
  .owner   = THIS_MODULE,
  .read    = varexter_read,
  .write   = varexter_write,
  .open    = varexter_open,
  .release = varexter_release
};

static int varexter_init(void)
{
  int err;

  if (alloc_chrdev_region(&dev_num, 0, 1, "varexter")) {
    printk("[VAREXTER] Dynamic register failed\n");
  }

  cdev_init(&cdev, &varexter_ops);
  err = cdev_add(&cdev, dev_num, 1);

  if (err) {
    printk("[VAREXTER] cdev_add failed\n");
  }

  printk("======= [VAREXTER] Module Installed =======\n");

  return 0;
}

static void varexter_exit(void)
{
  unregister_chrdev_region(dev_num, 1);

  printk("======= [VAREXTER] Module Removed =======\n");
}

module_init(varexter_init);
module_exit(varexter_exit);
MODULE_LICENSE("GPL");

