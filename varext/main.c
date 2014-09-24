#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

dev_t dev_num;
struct cdev cdev;

int extedvar = 100;

int varextee_open(struct inode *indoe, struct file *filp)
{
  return 0;
}

int varextee_release(struct inode *inode, struct file *filp)
{
  return 0;
}

ssize_t varextee_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
  extedvar--;
  printk("[VAREXTEE MINUS 1]: %d\n", extedvar);
  return 0;
}

ssize_t varextee_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
  extedvar++;
  printk("[VAREXTEE PLUS 1]: %d\n", extedvar);
  return 0;
}

struct file_operations varextee_ops = {
  .owner   = THIS_MODULE,
  .read    = varextee_read,
  .write   = varextee_write,
  .open    = varextee_open,
  .release = varextee_release
};

static int varextee_init(void)
{
  int err;

  if (alloc_chrdev_region(&dev_num, 0, 1, "varextee")) {
    printk("[VAREXTEE] Dynamic register failed\n");
    return -1;
  }

  cdev_init(&cdev, &varextee_ops);
  err = cdev_add(&cdev, dev_num, 1);

  if (err) {
    printk("[VAREXTEE] cdev_add failed\n");
    return -1;
  }

  printk("======= [VAREXTEE] Module Installed =======\n");
  printk("[VAREXTEE] extedvar: %d\n", extedvar);

  return 0;
}

static void varextee_exit(void)
{
  unregister_chrdev_region(dev_num, 1);

  cdev_del(&cdev);

  printk("======= [VAREXTEE] Module Removed =======\n");
}

EXPORT_SYMBOL(extedvar);

module_init(varextee_init);
module_exit(varextee_exit);
MODULE_LICENSE("GPL");

