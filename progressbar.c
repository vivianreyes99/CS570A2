#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>

/*
- Current status is a pointer to a long which represents the current status of the computation being tracked
(note: will need to typecast the argument in the functiom to be able to access memeber fields of this struct)
- ^ We will refer to the long integer to which this variable dereferences as the progress indicator
- InitialValue is the starting value for the computation
- TerminationValue is the value at which the computation is complete, number of bytes in file
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

/*
void * progress_monitor(void * progStatus)
{
    //this is where we will print the progress bar
    //remember to use fflush(stdout)

    // currentStatus/terminationValue * 100 = percent completed
    // percent completed (in decimal) * 50 = num of progress markers printed
    // need to find 1/50th of termination value, x
    // everytime trackX increases to x amount then print "-"
    // so we need to increment trackx everytime idk
    //https://roch.sdsu.edu/faq/os-faq-answers.shtml#pthreads
    //https://stackoverflow.com/questions/20196121/passing-struct-to-pthread-as-an-argument


    long x;
    // x = (long *) progStatus->InitialValue;
    x = .94;
    int trackX;
    int trackPlus;
    // need tracking variable so that when it = x, then print bars
    while(currentStatus <= terminationValue)
    {
        trackX++;
        if (trackX >= x) // if trackBar == 1/50 of termination value, it prints one "-"
        {
            if (trackPlus == 10) // if it is the tenth "-" , print a "+"
            {
                printf("+");
                fflush(stdout);
                trackPlus = 0;
            }
            printf("-");
            fflush(stdout);
            trackX = 0;
        }
        trackPlus++;
    }
}
*/



/*
- simple program which given a filename determines the number of words in the file
- makefile should compile program to an executable file named wordcount
- returns a long integer with the number of words and takes a file descriptor or filename as input
- wordcount will spawn a progress_monitor thread with a populated PROGRESS_STATUS structure as the argument
*/
long wordcount(char filename[], int byteSize)
{
    //example on how to initalize struct
    struct Progress PROGRESS_STATUS;
    
    // put info into struct 
    PROGRESS_STATUS.TerminationValue = byteSize;
    PROGRESS_STATUS.InitialValue = 0;
    PROGRESS_STATUS.CurrentStatus = 0;

    FILE *file;
    char c;
    long totalWords = 0;
    file = fopen(filename, "r");

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
    //join functions for threads is the equivalent to wait() for processes
    pthread_join(thread_id, NULL);

    long currentBytes = 0;

    if(file==NULL) 
     { 
         printf("Could not open file"); 
     } 
    else 
    {
        //off by one byte
        c = fgetc(file);
        currentBytes++;
        PROGRESS_STATUS.CurrentStatus = currentBytes;
        
        //pthread_create(&thread_id, NULL, progress_monitor, (void *) &PROGRESS_STATUS);
        while(c != EOF)
        {
            if(c==' ' || c=='\n')
            {
                totalWords++;
            }
            //one byte at a time
            c = fgetc(file); 
            //  
            currentBytes++;
            PROGRESS_STATUS.CurrentStatus = currentBytes;
        }
        printf("%ld" ,currentBytes);
        printf("\n");


    } 

    pthread_exit(NULL);

    fclose(file);
    //printf("There are " + totalWords + "words in file");
    return totalWords;
}

//takes a command line argument of the filename to be counted
//main calls wordcount function
int main (int argc, char** argv)
{
    // use stat to get number of bytes in file
    struct stat buf;  

    stat(argv[1], &buf);
    int size = buf.st_size;

    wordcount(argv[1], size);
}


/*
- Meeting 2/19
    - need to figure out how to work with pointers 
    - how to access the struct with pointers in the progress_monitor function
    - need to test if currentstatus is getting updated in the pthread when it's getting updated in the while loop in wordcount
    - figure out how and when to call pthread_create in wordcount
    - test math for printing progress bar
    - 
*/




