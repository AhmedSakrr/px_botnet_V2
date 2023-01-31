#include "../includes/client.h"

void add_pre_list(void)
{
	struct in_addr addr;
	int result = inet_aton("127.0.0.1", &addr);
	if (result && port > 1024)
		add_to_list(addr.s_addr, 1053);
	else
		remove_from_list(addr.s_addr, 1053);
}
