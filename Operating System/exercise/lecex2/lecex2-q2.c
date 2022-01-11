
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  	
  pid_t p1,p2;    /* process id (pid) */
  /* create a new (child) process */
  p1 = fork();
  /* fork() will attempt to create a new process by
      duplicating/copying the existing running process */

  if ( p1 == -1 )
  {
    perror( "fork() failed" );
    return EXIT_FAILURE;
  }
  if ( p1 == 0 )
  {
	  printf( "PARENT: okay, start here.\n");
    printf( "CHILD A: happy birthday to me!\n" );
    printf( "CHILD A: i'm bored....self-terminating....good-bye!\n" );

    return 47;  /* 0-255 valid range */
  }
  else
  {
    int status;
    waitpid( p1, &status, 0 );    /* BLOCKED */
    p2 = fork();
    if ( p2 == -1 ){
        perror( "fork() failed" );
        return EXIT_FAILURE;
    }
    if ( p2 == 0 ){
        printf( "CHILD B: and happy birthday to me!\n" );
        printf( "CHILD B: see ya later....self-terminating!\n" );

        return 47;  /* 0-255 valid range */
    }
    waitpid( p2, &status, 0 );
    if ( WIFSIGNALED( status ) )   /* child process was terminated   */
    {                              /*  by a signal (e.g., seg fault) */
      printf( "PARENT: ...abnormally (killed by a signal)\n" );
    }
    else /* if ( WEXITED( status ) ) */
    {
      printf( "PARENT: both child processes terminated successfully.\n" );
    }

  }
  printf("PARENT: phew, i'm glad they're gone!\n");

  return EXIT_SUCCESS;
}