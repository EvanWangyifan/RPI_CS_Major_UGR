/* forked.c */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>

/* implement these functions in fork-functions.c */
char lecex2_child(){
  char file[] = "data.txt";
  int fd = open(file, O_RDONLY );
  if ( fd == -1 ){
    fprintf(stderr, "ERROR: <open file failed>\n");
    abort();
  }

  char buffer[7];
  if (read( fd, buffer, 6 ) < 6){
    fprintf(stderr, "ERROR: <Invalid file size> \n");
    abort();
    }
    buffer[6] = '\0';
  return buffer[5];
};

int lecex2_parent(){
  int status;
  wait(&status);
  if (WIFSIGNALED(status) == 0){
    int exit_status = WEXITSTATUS(status);
    printf("PARENT: child process reported \'%c\'\n", exit_status);
    return EXIT_SUCCESS;
  }
  else{
    printf("PARENT: child process terminated abnormally\n");
    return EXIT_FAILURE;
  }
};

int main(){
  int rc;

  /* create a new (child) process */
  pid_t p = fork();

  if ( p == -1 )
  {
    perror( "fork() failed" );
    return EXIT_FAILURE;
  }

  if ( p == 0 )
  {
    rc = lecex2_child();
  }
  else /* p > 0 */
  {
    rc = lecex2_parent();
  }

  return rc;
}
