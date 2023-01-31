#pragma once

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

typedef struct node
{
	uint32_t ip;
	int port;
	int check;
	struct node *next;
} Node;

extern int sock;
extern int port;
extern Node *head;

// List functions
void add_to_list(uint32_t ip, int port);
void remove_from_list(uint32_t ip, int port);
void print_list(void);

// Listen functions
void listen_client(void);

// Pre list functions
void add_pre_list(void);

// Signal functions
void sigint_handler(int sig);
void signal_init(void);

// Update functions
void update_list(void);
void update_list_loop(void);