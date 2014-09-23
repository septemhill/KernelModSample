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
  DECLARE_WAITQUEUE(wait_q, current);

  prepare_to_wait(&wait_qh, &wait_q, TASK_INTERRUPTIBLE);

  schedule();

  finish_wait(&wait_qh, &wait_q);
  
  data->data.a += 1000;
  data->data.b += 2000;

  INIT_DELAYED_WORK(&data->work, work_func);
  schedule_delayed_work(&data->work, 10000);
}

static int kwait_init(void)
{
  work1.data.a = 100;
  work1.data.b = 200;
  work1.data.c = 0x47;

  work2.data.a = 999;
  work2.data.b = 888;
  work2.data.c = 0x44;

  INIT_DELAYED_WORK(&work1.work, work_func);
  INIT_DELAYED_WORK(&work2.work, work_func);
  schedule_delayed_work(&work1.work, 10000);
  schedule_delayed_work(&work2.work, 10000);

  return 0;
}

static void kwait_exit(void)
{
  cancel_delayed_work(&work1.work);
  cancel_delayed_work(&work2.work);
}

module_init(kwait_init);
module_exit(kwait_exit);
MODULE_LICENSE("GPL");
