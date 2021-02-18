#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*
- Current status is a pointer to a long which represents the current status of the computation being tracked
(note: will need to typecast the argument in the functiom to be able to access memeber fields of this struct)
- ^ We will refer to the long integer to which this variable dereferences as the progress indicator
- InitialValue is the starting value for the computation
- TerminationValue is the value at which the computation is complete 
- may assume TerminationValue >= Progress Indicator (*CurrentStatus) >= InitialValue
*/
typedef struct Progress {
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
- Request that they be printed immediately by using fflush(stdout)
*/	
void * progress_monitor(void * progStatus)
{
    //this is where we will print the progress bar
    //remember to use fflush(stdout)


}

/*
- simple program which given a filename determines the number of words in the file
- makefile should compile program to an executable file named wordcount
- returns a long integer with the number of words and takes a file descriptor or filename as input
- wordcount will spawn a progress_monitor thread with a populated PROGRESS_STATUS structure as the argument
*/
long wordcount(char filename[])
{
    FILE *file;
    char c;
    long totalWords = 0;
    file = fopen(filename, "r");

    if(file==NULL) 
     { 
         printf("Could not open file"); 
     } 
    else 
    {
        c = fgetc(file);
        while(c!=EOF)
        {
            if(c==' ' || c=='\n')
            {
                totalWords++;
            }
            c = fgetc(file); 
        }
    } 
    fclose(file);
    //printf("There are " + totalWords + "words in file");
    return totalWords;
}

//takes a command line argument of the filename to be counted
//main calls wordcount function
int main (int argc, char** argv)
{
    long count = 0;
    count = wordcount(argv[1]);
    //example on how to initalize struct
    struct Progress PROGRESS_STATUS = {0, 0, count};
    //example on how to access structs
    //PROGRESS_STATUS.CurrentStatus = 0
    
    printf("%ld", count);

    //thread id
    pthread_t thread_id;

    //Create attributes
    pthread_attr_t attr;
    pthread_attr_init(&attr);


    //creates thread, Nulls are the defult, i think we pass the struct into one of the NULL arguments
    //1st argument: pointer to thread1 which is set by this function
    //2nd argument: specifies attricutes, if val is NULL then defualt attributes shall be used
    //3rd argument: name of function to be executed for the thread to be created
    //4th argument: used to pass arguments to the function, progress_monitor (so probably the struct)
    pthread_create(&thread_id, NULL, progress_monitor, (void *) &PROGRESS_STATUS);
    //join functions for threads is the equivalent to wait() for processes
    pthread_join(thread_id, NULL);
    pthread_exit(NULL);


}







