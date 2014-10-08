#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int current = 0;
enum status { on, off};
typedef enum status status;


unsigned  i = 0;
status flag = on;
int pid[3] = {0};

struct sigaction newhandler, oldhandler;
sigset_t sig;

void next( int code)
{
      kill(pid[current],SIGSTOP);		// stop current process 
      current++;				// increment to next process
      if (current == 3) current = 0;		// reset the process index 
 
     kill(pid[current],SIGCONT);		// stop the next process
}

void main()
  {
    // create array for 3 different children
    

    // create child process 1
    pid[0] = fork();
    if ( pid[0] == 0 )
        {
          while (1) putc('1',stderr);
        }
    else {
	pid[1] = fork(); 	// creat child process 2
	if (pid[1] == 0)
	{
	  while(1) putc('2',stderr);
	}else
	{
		pid[2] = fork();	// create child process 3
		if (pid[2] == 0){
		  while(1) putc('3',stderr);
		}
		else{
 	sigemptyset(&sig);  /* clear the signal mask */
        newhandler.sa_handler = next;
        newhandler.sa_mask = sig;
        newhandler.sa_flags = 0;

        if ( sigaction(SIGALRM,&newhandler,&oldhandler) == -1 )
          {
            printf("1 - can't install signal handler \n");
            exit(-1);
          }
        while (1) {
          alarm(3);
          pause();

		}
	}
        	
        }
     }
}
