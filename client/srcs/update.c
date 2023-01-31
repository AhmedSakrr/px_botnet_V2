#include "../includes/client.h"

void update_list(void)
{
	Node *curr = head;
	while (curr != NULL)
	{
		if (curr->check == 0)
		{
			int sock = socket(AF_INET, SOCK_STREAM, 0);
			// create server address
			struct sockaddr_in server_addr;
			memset(&server_addr, 0, sizeof(server_addr));
			server_addr.sin_family = AF_INET;
			server_addr.sin_addr.s_addr = curr->ip;
			server_addr.sin_port = htons(curr->port);
			// connect to server
			if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
			{
				//printf("connect error %d.%d.%d.%d:%d failed\n", (curr->ip >> 24) & 0xFF, (curr->ip >> 16) & 0xFF, (curr->ip >> 8) & 0xFF, curr->ip & 0xFF, curr->port);
				remove_from_list(curr->ip, curr->port);
				continue;
			}
			// send ASK_LIST
			char tmp[BUF_SIZE];
			sprintf(tmp, "%d", port);
			write(sock, tmp, strlen(tmp));
			// receive list
			char buf[BUF_SIZE];
			int len = read(sock, buf, BUF_SIZE);
			if(len == -1)
			{
				remove_from_list(curr->ip, curr->port);
				continue;
			}
			// update list
			char *ptr = strtok(buf, " ");
			while(ptr != NULL)
			{
				uint32_t a, b, c, d;
				int port;
				sscanf(ptr, "%d.%d.%d.%d:%d", &a, &b, &c, &d, &port);
				uint32_t ip = (a << 24) | (b << 16) | (c << 8) | d;
				add_to_list(ip, port);
				ptr = strtok(NULL, " ");
			}
			curr->check = 1;
		}
		curr = curr->next;
	}
	// Re check all nodes to see if someone is not checked
	curr = head;
	while (curr != NULL)
	{
		if (curr->check == 0)
		{
			update_list();
		}
		curr = curr->next;
	}
}

void update_list_loop(void)
{
	while (1)
	{
		update_list();
		sleep(1);
	}
}