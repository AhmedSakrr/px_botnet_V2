#include "../includes/client.h"

void listen_client(void)
{
	int timeout = 10;
	// create server address
	sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	struct timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));	
	int flags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flags | O_NONBLOCK);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	// bind server address
	if (bind(sock, (const struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		//printf("bind error on port %d failed\n", port);
		return;
	}
	// listen
	if (listen(sock, 5) == -1)
	{
		//printf("listen error on port %d failed\n", port);
		return;
	}
	while (1) {
		// accept connection
		struct sockaddr_in client_addr;
		socklen_t client_addr_size = sizeof(client_addr);
		int new_sock = accept(sock, (struct sockaddr*)&client_addr, &client_addr_size);
		if (new_sock == -1)
			continue;
		// receive ASK_LIST
		char buf[BUF_SIZE];
		int len = read(new_sock, buf, BUF_SIZE);
		if (len == -1)
		{
			//printf("read error on port %d failed\n", port);
			close(new_sock);
			continue;
		}
		int tmp_port = 0;
		// Parse ASK_LIST:port with port into tmp_port
		sscanf(buf, "%d", &tmp_port);
		char list[BUF_SIZE];
		Node *curr = head;
		while (curr != NULL)
		{
			sprintf(list, "%d.%d.%d.%d:%d\n", (curr->ip >> 24) & 0xFF, (curr->ip >> 16) & 0xFF, (curr->ip >> 8) & 0xFF, curr->ip & 0xFF, curr->port);
			write(new_sock, list, strlen(list));
			curr = curr->next;
		}
		// add client to list
		add_to_list(client_addr.sin_addr.s_addr, tmp_port);
		// close connection
		close(new_sock);
		// check if listen socket is still alive
		if (recv(sock, buf, 1, MSG_PEEK | MSG_DONTWAIT) == 0)
			break;
	}
	close(sock);

}

void listen_manager(void)
{
	// Restart the listen_client until it is successful
	while (1)
	{
		printf("Listening on port %d\n", port);
		listen_client();
		port = rand() % 63535 + 2000;
		sleep(1);
	}
}