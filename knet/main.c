#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/in.h>
#include <linux/net.h>

struct socket *sock, *client;
struct task_struct *net_th;

int KernelNetThread(void *data)
{
  int errno;
  struct sockaddr_in *sockaddr;
  unsigned long timeout;
  struct msghdr hdr;
  struct kvec iv = { "Septem", 6 };


  memset(&hdr, 0, sizeof(struct msghdr));

  errno = sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);

  if (errno < 0) {
    printk("Create Socket Failed\n");
  }
  
  sockaddr = (struct sockaddr_in *)kmalloc(sizeof(struct sockaddr_in), GFP_KERNEL);

  sockaddr->sin_family      = AF_INET;
  sockaddr->sin_port        = 13207;
  sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
 
  errno = kernel_bind(sock, (struct sockaddr *)sockaddr, sizeof(struct sockaddr_in));

  if (errno < 0) {
    printk("BIND ERROR\n");
  }

  errno = kernel_listen(sock, 5);

  if (errno < 0) {
    printk("LISTEN ERROR\n");
  }

  while (1) {
    if (kthread_should_stop()) break;

    errno = kernel_accept(sock, &client, O_NONBLOCK);

    if (errno < 0) {
      printk("Accept Failed\n");
    }
    else {
      kernel_sendmsg(sock, &hdr, &iv, 1, iv.iov_len);
      sock_release(client);
    }
    //printk("Where Is Septem ?\n");
    //printk("Nobody know\n");

    do {
      set_current_state(TASK_INTERRUPTIBLE);
      timeout = schedule_timeout(3 * HZ);
    } while (timeout);
  }

  return 0;
}

static int KernelNetTestInit(void)
{
  net_th = kthread_create(KernelNetThread, NULL, "KNET");

  if (net_th) {
    printk("NET Thread Wakeup\n");
    wake_up_process(net_th);
  }

  return 0;
}

void KernelNetTestClean(void)
{
  sock_release(sock);

  kthread_stop(net_th);
}

MODULE_LICENSE("GPL");
module_init(KernelNetTestInit);
module_exit(KernelNetTestClean);
