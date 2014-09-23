#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/fs.h>

struct cli_data {
  int a;
  int b;
  char c;
};

struct work_data {
  struct work_struct work;
  struct cli_data data;
} data1, data2;

static void valOutput(struct work_struct *work)
{
  struct work_data *data = container_of(work, struct work_data, work);

  printk("A Value: %d\n", data->data.a);
  printk("B Value: %d\n", data->data.b);
  printk("C Value: 0x%X\n", data->data.c);

  INIT_WORK(&data->work, valOutput);
  schedule_work(&data->work);
}

static int __init kworkqueue_init(void)
{
  data1.data.a = 100;
  data1.data.b = 200;
  data1.data.c = 0x44;

  data2.data.a = 232;
  data2.data.b = 325;
  data2.data.c = 0x47;

  INIT_WORK(&data1.work, valOutput);
  INIT_WORK(&data2.work, valOutput);

  schedule_work(&data1.work);
  schedule_work(&data2.work);

  return 0;
}

static void __exit kworkqueue_exit(void)
{
}

MODULE_LICENSE("GPL");
module_init(kworkqueue_init);
module_exit(kworkqueue_exit);
