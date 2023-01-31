#include "../includes/client.h"

void add_pre_list(void)
{
	struct in_addr addr;
	int result = inet_aton("192.168.1.19", &addr);
	if (result && port > 1024)
		add_to_list(addr.s_addr, 7328);
	else
		remove_from_list(addr.s_addr, 7328);
}
