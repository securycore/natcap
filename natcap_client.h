/*
 * Author: Chen Minqiang <ptpt52@gmail.com>
 *  Date : Sun, 05 Jun 2016 16:24:31 +0800
 */
#ifndef _NATCAP_CLIENT_H_
#define _NATCAP_CLIENT_H_

#include <linux/types.h>
#include <linux/if_ether.h>
#include "natcap.h"

extern unsigned int client_forward_mode;
extern unsigned int server_persist_timeout;

extern u32 default_u_hash;
extern unsigned char default_mac_addr[ETH_ALEN];

void natcap_server_cleanup(void);
int natcap_server_add(const struct tuple *dst);
int natcap_server_delete(const struct tuple *dst);
void *natcap_server_get(loff_t idx);

int natcap_client_init(void);
void natcap_client_exit(void);

#endif /* _NATCAP_CLIENT_H_ */