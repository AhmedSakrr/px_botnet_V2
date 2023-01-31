#include "../includes/client.h"

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