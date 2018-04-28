/*
 * netkm.c
 *
 *  Created on: Apr 26, 2018
 *      Author: Andrew Pineiro
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h> // NF_DROP
#include <linux/netfilter_ipv4.h>
#include <linux/socket.h>

// this is the structure we shall use to register our function
static struct nf_hook_ops nfho;
static int x;

// this is the hook function itself
unsigned int hook_func(void * priv,
					   struct sk_buff * skb,
					   const struct nf_hook_state * state) {

	printk(KERN_INFO "Packet dropped!\n");
    return NF_DROP; // drop ALL packets
}

static int netkm_init(void) {

	printk(KERN_ALERT "Hello world!\n");

	// fill in our hook structure
	//nfho.hook = (nf_hookfn *) hook_func; // handler function
	nfho.hook = hook_func; // handler function
	nfho.hooknum = NF_INET_PRE_ROUTING; // first hook for IPv4
	nfho.pf = PF_INET;
	nfho.priority = NF_IP_PRI_FIRST; // make our function first

	x = nf_register_net_hook(&init_net, &nfho);
	//nf_register_hook(&nfho);

	printk(KERN_ALERT "Register hook: %d\n", x);

	return 0;
}

static void netkm_cleanup(void) {

	nf_unregister_net_hook(&init_net, &nfho);
	//nf_unregister_hook(&nfho);
	printk(KERN_ALERT "Goodbye world!\n");
}

module_init(netkm_init);
module_exit(netkm_cleanup);
