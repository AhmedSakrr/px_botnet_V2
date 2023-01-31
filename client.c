#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#define BUF_SIZE 1024

int sock;
int port;

typedef struct node
{
	uint32_t ip;
	int port;
	int check;
	struct node *next;
} Node;

Node *head = NULL;

void add_to_list(uint32_t ip, int port)
{
	// Check if the node is already in the list
	Node *curr = head;
	while (curr != NULL)
	{
		if (curr->ip == ip && curr->port == port)
			return;
		curr = curr->next;
	}
	// Add the node to the list
	Node *new_node = (Node*)malloc(sizeof(Node));
	new_node->ip = ip;
	new_node->port = port;
	new_node->next = head;
	new_node->check = 0;
	head = new_node;
}

void remove_from_list(uint32_t ip, int port)
{
	Node *curr = head;
	Node *prev = NULL;

	while (curr != NULL)
	{
		if (curr->ip == ip && curr->port == port)
		{
			if (prev == NULL)
				head = curr->next;
			else
				prev->next = curr->next;
			free(curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}

void print_list(void)
{
	Node *curr = head;
	printf("List of nodes:\n");
	while (curr != NULL)
	{
		printf("    - %s:%d %d\n", inet_ntoa(*(struct in_addr*)&(curr->ip)), curr->port, curr->check);
		curr = curr->next;
	}
	printf("----------------\n");
}

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


void listen_client(void)
{
	// create server address
	sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
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
	}
	close(sock);

}

void add_pre_list(void)
{
	struct in_addr addr;
	int result = inet_aton("127.0.0.1", &addr);
	add_to_list(addr.s_addr, port);
}

void update_list_loop(void)
{
	while (1)
	{
		update_list();
		sleep(5);
	}
}

int main(int argc, char *argv[])
{
	pthread_t listen;
	pthread_t update;

	srand(time(NULL));
	port = rand() % 63535 + 2000;
	add_pre_list();
	pthread_create(&listen, NULL, (void *)listen_client, NULL);
	pthread_create(&update, NULL, (void *)update_list_loop, NULL);

	////Anonimize
	//pid_t pid1;
	//pid_t pid2;
	//int status;
	//char name[] = "htop";
	//remove(argv[0]);
	//strncpy(argv[0], "htop", strlen("htop"));
	//sigset_t set;
	//sigfillset(&set);
	//sigprocmask(SIG_SETMASK, &set, NULL);
	//if (pid1 = fork())
	//{
	//	waitpid(pid1, &status, 0);
	//	exit(0);
	//}
	//else if (!pid1)
	//	if (pid2 = fork())
	//		exit(0);
	//setsid();
	//chdir("/");
	//signal(SIGPIPE, SIG_IGN);
	//prctl(PR_SET_NAME, (unsigned long)name, 0, 0, 0);

	printf("Press q to quit and p to print ip %d\n", port);
	while (1)
	{
		// Wait for keyboard input to stop
		char c = getchar();
		if (c == 'q')
		{
			break;
		}
		else if (c == 'p')
		{
			print_list();
		}
		while (getchar() != '\n');
	}
	close(sock);
	pthread_cancel(listen);
	pthread_cancel(update);
	return (0);
}