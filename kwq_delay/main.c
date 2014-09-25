#include <linux/init.h>
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
  struct delayed_work work;
  struct cli_data data;
} data1, data2;

static void valOutput(struct work_struct *work)
{
  struct delayed_work *delay_work = to_delayed_work(work);
  struct work_data *data = container_of(delay_work, struct work_data, work);

  printk("A Value: %d\n", data->data.a);
  printk("B Value: %d\n", data->data.b);
  printk("C Value: 0x%X\n", data->data.c);

  INIT_DELAYED_WORK(&data->work, valOutput);
  schedule_delayed_work(&data->work, 10000);
}

static int __init kworkqueue_init(void)
{
  data1.data.a = 100;
  data1.data.b = 200;
  data1.data.c = 0x44;

  data2.data.a = 232;
  data2.data.b = 325;
  data2.data.c = 0x47;

  INIT_DELAYED_WORK(&data1.work, valOutput);
  INIT_DELAYED_WORK(&data2.work, valOutput);

  schedule_delayed_work(&data1.work, 10000);
  schedule_delayed_work(&data2.work, 20000);

  printk("======= [kwq_delay] Module Installed =======\n");

  return 0;
}

static void __exit kworkqueue_exit(void)
{
  cancel_delayed_work(&data1.work);
  cancel_delayed_work(&data2.work);

  printk("======= [kwq_delay] Module Removed =======\n");
}

MODULE_LICENSE("GPL");
module_init(kworkqueue_init);
module_exit(kworkqueue_exit);
