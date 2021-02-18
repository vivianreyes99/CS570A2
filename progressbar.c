#include <stdio.h>
#include <pthread.h>

/*
- Current status is a pointer to a long which represents the current status of the computation being tracked
(note: will need to typecast the argument in the functiom to be able to access memeber fields of this struct)
- InitialValue is the starting value for the computation
- TerminationValue is the value at which the computation is complete
- may assume TerminationValue >= Progress Indicator (*CurrentStatus) >= InitialValue
*/
typedef struct {
long * CurrentStatus;
long InitialValue;
long TerminationValue;
} PROGRESS_STATUS;

/*
- progress_monitor functions monitors the progress of a task that executes
in a separate thread
- the void * argument is a pointer to the PROGRESS_STATUS struct
- this function is only invoked once by the thread
- loops until the progress bar is complete and exits
- each time the thread executing the function progress_monitor is allocated the CPU time
- should compute the % of the task that has been completed and add to a progress bar of 50 chars 
representing the amount of progress that has been made
- most chars are hyphens (-), but every 10th progress mark should be dislpayed as a plus (+)
- when new marker chars need to be added, print them without a line feed char, so smooth progression
- read A2 pdf for examples and more in depth info
*/	
//void * progress_monitor(void *)
//{

//}

/*
- simple program which given a filename determines the number of words in the file
- makefile should compile program to an executable file named wordcount
- returns a long integer with the number of workds and takes a file descriptor or filename as input
- wordcount will spawn a progress_monitor thread with a populated PROGRESS_STATUS structure as the argument
*/
long wordcount(char filename[])
{

}

//takes a command line argument of the filename to be counted
//main calls wordcount function
int main (int argc, char** argv)
{

}







