#include "includes/client.h"

int sock;
int port;
Node *head;

int main(int argc, char *argv[])
{
	pthread_t listen;
	pthread_t update;

	head = NULL;
	(void) argc;
	(void) argv;
	
	//signal_init();
	srand(time(NULL));
	port = rand() % 63535 + 2000;
	add_pre_list();
	pthread_create(&listen, NULL, (void *)listen_manager, NULL);
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
		sleep(1000);
		char c = getchar();
		if (c == 'q')
		{
			break;
		}
		else if (c == 'p')
		{
			print_list();
		}
		else if (c == 'r')
		{
			// Set check to 0
			Node *tmp = head;
			while (tmp)
			{
				tmp->check = 0;
				tmp = tmp->next;
			}
			printf("Reset check\n");
		}
		while (getchar() != '\n');
	}
	close(sock);
	pthread_cancel(listen);
	pthread_cancel(update);
	return (0);
}