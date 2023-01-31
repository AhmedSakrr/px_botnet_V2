#include "../includes/client.h"

void sigint_handler(int sig)
{
	(void) sig;
}

void signal_init(void)
{
	(void) signal(SIGINT, sigint_handler);
	(void) signal(SIGTERM, sigint_handler);
	(void) signal(SIGQUIT, sigint_handler);
	(void) signal(SIGKILL, sigint_handler);
	(void) signal(SIGSTOP, sigint_handler);
	(void) signal(SIGTSTP, sigint_handler);
	(void) signal(SIGABRT, sigint_handler);
	(void) signal(SIGSEGV, sigint_handler);
	(void) signal(SIGFPE, sigint_handler);
	(void) signal(SIGILL, sigint_handler);
	(void) signal(SIGBUS, sigint_handler);
	(void) signal(SIGSYS, sigint_handler);
	(void) signal(SIGPIPE, sigint_handler);
	(void) signal(SIGALRM, sigint_handler);
	(void) signal(SIGXCPU, sigint_handler);
	(void) signal(SIGXFSZ, sigint_handler);
	(void) signal(SIGVTALRM, sigint_handler);
	(void) signal(SIGPROF, sigint_handler);
	(void) signal(SIGUSR1, sigint_handler);
	(void) signal(SIGUSR2, sigint_handler);
	(void) signal(SIGCHLD, sigint_handler);
	(void) signal(SIGCONT, sigint_handler);
	(void) signal(SIGTTIN, sigint_handler);
	(void) signal(SIGTTOU, sigint_handler);
	(void) signal(SIGIO, sigint_handler);
	(void) signal(SIGPWR, sigint_handler);
	(void) signal(SIGWINCH, sigint_handler);
	(void) signal(SIGURG, sigint_handler);
	(void) signal(SIGPOLL, sigint_handler);
	(void) signal(SIGSTKFLT, sigint_handler);
	(void) signal(SIGPWR, sigint_handler);
	(void) signal(SIGRTMIN, sigint_handler);
}