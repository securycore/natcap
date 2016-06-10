/*
 * Author: Chen Minqiang <ptpt52@gmail.com>
 *  Date : Sun, 05 Jun 2016 16:27:20 +0800
 */
#ifndef _NATCAP_COMMON_H_
#define _NATCAP_COMMON_H_
#include <linux/ctype.h>
#include <linux/device.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_zones.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_core.h>
#include "natcap.h"

extern unsigned int debug;
extern unsigned int mode;
extern unsigned int server_seed;

extern const char *const hooknames[];

#define NATCAP_println(fmt, ...) \
	do { \
		printk(KERN_DEFAULT "{" MODULE_NAME "}:%s(): " pr_fmt(fmt) "\n", __FUNCTION__, ##__VA_ARGS__); \
	} while (0)

#define NATCAP_FIXME(fmt, ...) \
	do { \
		if (debug & 0x10) { \
			printk(KERN_ALERT "fixme: " pr_fmt(fmt), ##__VA_ARGS__); \
		} \
	} while (0)

#define NATCAP_DEBUG(fmt, ...) \
	do { \
		if (debug & 0x8) { \
			printk(KERN_DEBUG "debug: " pr_fmt(fmt), ##__VA_ARGS__); \
		} \
	} while (0)

#define NATCAP_INFO(fmt, ...) \
	do { \
		if (debug & 0x4) { \
			printk(KERN_DEFAULT "info: " pr_fmt(fmt), ##__VA_ARGS__); \
		} \
	} while (0)

#define NATCAP_WARN(fmt, ...) \
	do { \
		if (debug & 0x2) { \
			printk(KERN_WARNING "warning: " pr_fmt(fmt), ##__VA_ARGS__); \
		} \
	} while (0)

#define NATCAP_ERROR(fmt, ...) \
	do { \
		if (debug & 0x1) { \
			printk(KERN_ERR "error: " pr_fmt(fmt), ##__VA_ARGS__); \
		} \
	} while (0)

#define IP_TCPUDP_FMT	"%pI4:%u->%pI4:%u"
#define IP_TCPUDP_ARG(i,t)	&(i)->saddr, ntohs((t)->source), &(i)->daddr, ntohs((t)->dest)
#define TCP_ST_FMT	"%c%c%c%c%c%c%c%c"
#define TCP_ST_ARG(t) \
	(t)->cwr ? 'C' : '.', \
	(t)->ece ? 'E' : '.', \
	(t)->urg ? 'U' : '.', \
	(t)->ack ? 'A' : '.', \
	(t)->psh ? 'P' : '.', \
	(t)->rst ? 'R' : '.', \
	(t)->syn ? 'S' : '.', \
	(t)->fin ? 'F' : '.'
#define UDP_ST_FMT "UL:%u,UC:%04X"
#define UDP_ST_ARG(u) ntohs((u)->len), ntohs((u)->check)

#define DEBUG_FMT_PREFIX "[%s](%s)"
#define DEBUG_ARG_PREFIX hooknames[hooknum], __FUNCTION__

#define DEBUG_FMT "[" IP_TCPUDP_FMT "|ID:%04X,IL:%u|" TCP_ST_FMT "]"
#define DEBUG_ARG(i, t) IP_TCPUDP_ARG(i,t), ntohs((i)->id), ntohs((i)->tot_len), TCP_ST_ARG(t)

#define DEBUG_FMT_UDP "[" IP_TCPUDP_FMT "|ID:%04X,IL:%u|" UDP_ST_FMT "]"
#define DEBUG_ARG_UDP(i, u) IP_TCPUDP_ARG(i,u), ntohs((i)->id), ntohs((i)->tot_len), UDP_ST_ARG(u)

#define TUPLE_FMT "%pI4:%u-%c"
#define TUPLE_ARG(t) &(t)->ip, ntohs((t)->port), (t)->encryption ? 'e' : 'o'

int skb_csum_test(struct sk_buff *skb);
int natcap_tcp_encode(struct sk_buff *skb, const struct natcap_tcp_tcpopt *nto);
int natcap_tcp_decode(struct sk_buff *skb, struct natcap_tcp_tcpopt *nto);

int natcap_udp_encode(struct sk_buff *skb, int mode);
int natcap_udp_decode(struct sk_buff *skb, struct natcap_udp_tcpopt *nuo);

int ip_set_test_dst_ip(const struct net_device *in, const struct net_device *out, struct sk_buff *skb, const char *ip_set_name);
int ip_set_add_dst_ip(const struct net_device *in, const struct net_device *out, struct sk_buff *skb, const char *ip_set_name);
int ip_set_del_dst_ip(const struct net_device *in, const struct net_device *out, struct sk_buff *skb, const char *ip_set_name);
int ip_set_test_src_mac(const struct net_device *in, const struct net_device *out, struct sk_buff *skb, const char *ip_set_name);

unsigned int natcap_tcp_dnat_setup(struct nf_conn *ct, __be32 ip, __be16 port);

int natcap_common_init(void);

void natcap_common_exit(void);

#endif /* _NATCAP_COMMON_H_ */
