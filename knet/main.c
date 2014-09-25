#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/in.h>
#include <linux/net.h>

struct task_struct *net_th;
DECLARE_WORK_QUEUE_HEAD(sock_wqh);

int net_thread(void *data)
{
  int timeout;

  while (1) {
    if (kthread_should_stop()) {
      break;
    }

    do {
      set_current_state(TASK_INTERRUPTIBLE);
      timeout = schedule_timeout(3 * HZ);
    } while (timeout);
  }

  return 0;
}

static int knet_init(void)
{
  net_th = kthread_create(net_thread, NULL, "knet");

  if (net_th) {
    wake_up_process(net_th);
  }

  return 0;
}

static void knet_exit(void)
{
  kthread_stop(net_th);
}

module_init(knet_init);
module_exit(knet_exit);
MODULE_LICENSE("GPL");
