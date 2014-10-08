#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

enum status { on, off};
typedef enum status status;

enum boolean { false, true };
typedef enum boolean boolean;

unsigned  short i = 0;
int pid[3] = {0};		// create an array for three processes
int numProcs = 0;		// init index for pid array
status flag = on;

struct sigaction newhandler, oldhandler;
sigset_t sig;

void setblock( int fd ,boolean block)
{
   static int blockf, nonblockf;
   static boolean first = true;

   int flags;

   if (first) {
      first = false;
      if (( flags = fcntl(fd,F_GETFL,0)) == -1)
      {
         fprintf(stderr," fcntl - error \n");
         exit(1);
      }
     blockf = flags & ~ O_NDELAY;
     nonblockf = flags | O_NDELAY;
  }
  if ( fcntl(fd,F_SETFL, block ? blockf : nonblockf) == -1 )
      {
         fprintf(stderr," fcntl2 - error \n");
         exit(1);
      }
}
void next( int code)
{
  
	
  	if ( 0 == pid[numProcs]) return;	// if in the child return to parent process
  	
	kill(pid[numProcs],SIGSTOP);		// stop current process
	numProcs++;				// increment process pointer
     	if (numProcs == 3) numProcs =0;		// wraps the array around to the 0th process
	kill(pid[numProcs],SIGCONT);		// start the next process
	
}

void main(int argc, char *argv[])
   {
    char mess[80];
    int fd;
    int numch;
    char par1[30] = "exec/";
    int lenPar1 = 5;   
     { 

        fd = open("/dev/tty",O_RDWR);
        setblock(fd,false);

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
          switch  ( numch = read(fd,mess,80))  {
          case -1 :
          case 0 :
              break;
          default: 
	      fprintf(stderr," %d   <%s>\n",numch,mess);
              mess[numch-1] = '\0';
    	      pid[numProcs] = fork();
              for( i=0; i< numch;i++) par1[lenPar1+i] = mess[i]; // combine the users arguement with the path of exec
	      fprintf(stderr," %d   <%s>   <%s>\n",numch,mess,par1);
              
    	      if ( pid[numProcs] == 0 )			//if in the child process exec the program from user input
        		{		
			execl(par1, mess, NULL);
          		}
          fprintf(stderr," in parent\n");
        }
     }
  }
}

