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

    
    long previousPercent = 0;
     // 1/50th of termination value 
    long increment = 0;
    

    int trackPlus = 0;
    //tracks current percent complete of wordcount function
    long percentComplete = 0;
    //the difference betwwen the current percent complete with the percious percent complete
    long difference = 0;
    ////number of hyphens to print each loop
    //long trackBar = difference / increment;
   
    long trackBar = 0;
 
    //check current percent complete with last percent complete and from there determine how many
    //more dashes to print (from Roch's office hours)
    
    // when it has completed 1/50th of the task, it will print one "-""
    while((*progStat -> CurrentStatus) < (progStat -> TerminationValue))
    {
        increment = 0.02 * (progStat -> TerminationValue);
        
        //tracks current percent complete of wordcount function
        percentComplete = (*progStat -> CurrentStatus)/(progStat -> TerminationValue) * 100;
        //the difference betwwen the current percent complete with the percious percent complete
        difference = percentComplete - previousPercent;
        //number of hyphens to print each loop
        trackBar = difference / increment; 

        //if the differences between current percent complete and pervious percent 
        //is greater than the increment value (the value of each hyphen) then enter loop
        if (difference >= increment) 
        {
            //loop runs trackBar times, trackBar is the number of hyphens to print per loop
            for (int i = 0; i < trackBar; i++)
            {
                printf("-");
                fflush(stdout);
                trackPlus++;
                // if it is the tenth "-" , print a "+"
                if (trackPlus == 9) 
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
    // create and open file for reading
    PROGRESS_STATUS *data = NULL;
    data = malloc(sizeof(PROGRESS_STATUS));
    // put info into struct 
    data->TerminationValue = byteSize;

    FILE *file;
    char c;
    char prevC;
    file = fopen(filename, "r");
 
    //thread id
    pthread_t thread_id;
 
    long totalWords = 0; // keeps count of the total words in file
    long currentBytes = 0;
 
    pthread_create(&thread_id, NULL, progress_monitor, (void *) data);
    //*(data -> CurrentStatus) = &currentBytes;

    if(filename==NULL) 
    { 
         printf("Could not open file"); 
    } 
    else 
    {
        //off by one byte
        c = fgetc(file);
        currentBytes++;
        data->CurrentStatus = &currentBytes;
        
        while(c != EOF)
        {
            // if current char is a space, newline, or tab 
            // and previous char was not also a space, newline, or tab
            if ( (c ==' ' || c =='\n' || c == '\t') &&  prevC != ' ' && prevC != '\n' && prevC != '\t')
            {
                totalWords++;
            }
            //fgetc() takes in one byte at a time
            prevC = c;
            c = fgetc(file); 
            currentBytes++;
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
 
    wordcount(argv[1], size);
    // bigFile has 2191390 words. 13234244 bytes
    return 0;
}

