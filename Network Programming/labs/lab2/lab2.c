#include "unp.h"

int currentChild = 0;

void signalAct( int sig )
{
	int wstat;
	pid_t child;
	while((child = waitpid(-1, &wstat, WNOHANG)) > 0)
	{
		printf("Parent sees child PID %d has terminated.\n", child);
		currentChild -= 1;
	}
}
	

int main()
{
	setbuf(stdout, NULL);
	printf("Number of children to spawn: ");
	scanf("%d", &currentChild);
	printf("Told to spawn %d children\n", currentChild);
	int i = 0;
	for( i = 0; i < currentChild; i++ )
	{
		pid_t child = fork();
		if(child == 0)
		{
			srand (getpid());
			int waitTime = rand()%6;
			printf("Child PID %d dying in %d seconds.\n", getpid(), waitTime);
			sleep( waitTime );
			printf("Child PID %d terminating.\n", getpid());
			exit(0);
		}
		else
		{
			printf("Parent spawned child PID %d\n", child);
		}	
	}
	
	while( currentChild != 0 )
	{
		Signal(SIGCHLD, signalAct);
	}
	return 0;
}