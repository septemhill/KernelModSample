#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/wait.h>

struct cli_data {
  int a;
  int b;
  char c;
};

struct work_data {
  struct delayed_work work;
  struct cli_data data;
} work1, work2;

DECLARE_WAIT_QUEUE_HEAD(wait_qh);

static void work_func(struct work_struct *work)
{
  struct delayed_work *del_tmp = to_delayed_work(work);
  struct work_data *data = container_of(del_tmp, struct work_data, work);

  wait_event_interruptible(wait_qh, 1);
  
//  data->data.a += 1000;
//  data->data.b += 2000;

  printk("WORK: 0x%02x\n", data->data.c);

  schedule_delayed_work(&data->work, 10000);
}

static int kwait_init(void)
{
  work1.data.a = 100;
  work1.data.b = 200;
  work1.data.c = 0x41;

  work2.data.a = 999;
  work2.data.b = 888;
  work2.data.c = 0x42;

  INIT_DELAYED_WORK(&work1.work, work_func);
  INIT_DELAYED_WORK(&work2.work, work_func);
  schedule_delayed_work(&work1.work, 10000);
  schedule_delayed_work(&work2.work, 10000);

  printk("======= [WAIT] Module Installed =======\n");

  return 0;
}

static void kwait_exit(void)
{
  cancel_delayed_work(&work1.work);
  cancel_delayed_work(&work2.work);

  printk("======= [WAIT] Module Removed =======\n");
}

module_init(kwait_init);
module_exit(kwait_exit);
MODULE_LICENSE("GPL");
