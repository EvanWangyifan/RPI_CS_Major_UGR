
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  	
  pid_t p;    /* process id (pid) */
  /* create a new (child) process */
  p = fork();
  /* fork() will attempt to create a new process by
      duplicating/copying the existing running process */

  if ( p == -1 )
  {
    perror( "fork() failed" );
    return EXIT_FAILURE;
  }
  if ( p == 0 )
  {
	printf( "PARENT: okay, start here.\n");
    printf( "CHILD: happy birthday to me!\n" );
    return 47;  /* 0-255 valid range */
  }
  else
  {
    int status;
    waitpid( p, &status, 0 );    /* BLOCKED */

    printf( "CHILD: i'm bored....self-terminating....good-bye!\n" );

    if ( WIFSIGNALED( status ) )   /* child process was terminated   */
    {                              /*  by a signal (e.g., seg fault) */
      printf( "PARENT: ...abnormally (killed by a signal)\n" );
    }
    else /* if ( WEXITED( status ) ) */
    {
      printf( "PARENT: child process terminated successfully.\n" );
    }

  }
  printf("PARENT: sigh, i'm gonna miss that little child process.\n");

  return EXIT_SUCCESS;
}