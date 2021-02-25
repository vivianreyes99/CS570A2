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
- Request that they be printed immediately by using fflush(stdout)
*/  
 
 
void * progress_monitor(void * progStatus)
{

    //typecast the void pointer to a PROGRESS_STATUS pointer
    PROGRESS_STATUS *progStat = (PROGRESS_STATUS *) progStatus;
    printf("TerminationValue: ");
    printf("%ld\n", (progStat -> TerminationValue)); 
    printf("Current Status: ");
    printf("%ld\n", (*progStat -> CurrentStatus));

    // while currstatus < term value, if percent complete increased by 2% of the term value, print a "-"

    // 1/50th of termination value, for test.txt, it is 1.44
    long increment = 0.02 * (progStat -> TerminationValue);
    long previousPercent = 0;

    printf("Increment: ");
    printf("%ld\n", increment); 

    int trackPlus = 0;

    long percentComplete = (*progStat -> CurrentStatus)/(progStat -> TerminationValue)*100;
    long difference = percentComplete - previousPercent;
    long trackBar = difference / increment;
    printf("Percent Complete: ");
    printf("%ld\n", percentComplete); 
    printf("Difference of current and pervious percent: ");
    printf("%ld\n", difference); 
    printf("TrackBar ");
    printf("%ld\n", trackBar);
    // need tracking variable so that when it = x, then print bars
 
    //check current percent complete with last percent complete and from there determine how many
    //more dashes to print
    
    // when it has completed 1/50th of the task, it will print one "-""
    while((*progStat -> CurrentStatus) < (progStat -> TerminationValue))
    {
        long percentComplete = (*progStat -> CurrentStatus)/(progStat -> TerminationValue) * 100;
        long difference = percentComplete - previousPercent;
        long trackBar = difference / increment; //number of hyphens/bars to print
        if (difference >= increment) // when trackIncrement == 1/50 of termination value, it prints one "-"
        {
            long trackBar = difference / increment; // trackBar is the number of dashes to print
            for (int i = 0; i < trackBar; i++)
            {
                printf("-");
                fflush(stdout);
                trackPlus++;
                if (trackPlus == 9) // if it is the tenth "-" , print a "+"
                {
                    printf("+");
                    fflush(stdout);
                    trackPlus = 0;
                }
            }
        }
        previousPercent = percentComplete;

    }
    pthread_exit(NULL);
}
 
 
/*
- simple program which given a filename determines the number of words in the file
- returns a long integer with the number of words and takes a file descriptor or filename as input
- wordcount will spawn a progress_monitor thread with a populated PROGRESS_STATUS structure as the argument
*/
long wordcount(char filename[], long byteSize)
{

    PROGRESS_STATUS *data = NULL;
    data = malloc(sizeof(PROGRESS_STATUS));
    // put info into struct 
    data->CurrentStatus = malloc(sizeof(data->CurrentStatus));
    data->TerminationValue = byteSize;

    FILE *file;
    char c;
    char prevC;
    //file = fopen(filename, "r");
    file = fopen(filename, "r");
 
    //thread id
    pthread_t thread_id;
 
    long totalWords = 0;
    long currentBytes = 0;
 
    pthread_create(&thread_id, NULL, progress_monitor, (void *) data);
 
    if(filename==NULL) 
    { 
         printf("Could not open file"); 
    } 
    else 
    {
        //off by one byte
        c = fgetc(file);
        currentBytes++;
        *(data -> CurrentStatus) = currentBytes;
        
        while(c != EOF)
        {
            // if current char is a space, newline, or tab 
            // and previous char was not also a space, newline, or tab
            if ( (c ==' ' || c =='\n' || c == '\t') &&  prevC != ' ' && prevC != '\n' && prevC != '\t')
            {
                totalWords++;
            }
            //one byte at a time
            prevC = c;
            c = fgetc(file); 
            currentBytes++;
            *(data -> CurrentStatus) = currentBytes;
        }

        if (c == EOF) // increment 
            totalWords++;
    }
 
    pthread_join(thread_id, NULL);
    fclose(file);
    
    printf("total number of words: ");
    fflush(stdout);
    printf ("%ld\n", totalWords);
    fflush(stdout);

    return totalWords;
}
 
//takes a command line argument of the filename to be counted
//main calls wordcount function
int main (int argc, char** argv)
{
    // use stat to get number of bytes in file
    struct stat buf;  
 
    stat(argv[1], &buf);
    long size = buf.st_size;
 
    long words = wordcount(argv[1], size);
 
    // bigFile has 2191390 words. 13234244 bytes
    return 0;
}

