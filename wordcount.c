#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>

typedef struct {
long * CurrentStatus;
long InitialValue;
long TerminationValue;
} PROGRESS_STATUS;


void * progress_monitor(void * progStatus)
{
    //typecast the void pointer to a PROGRESS_STATUS pointer
    PROGRESS_STATUS *progStat = (PROGRESS_STATUS *) progStatus;

    float termination = progStat->TerminationValue - progStat->InitialValue;
    int currentPercent = 0;
    int perviousPercent = 0;
    int difference = 0;
    int trackPlus = 0;

    while(currentPercent < 50)
    {
        currentPercent = (*(progStat -> CurrentStatus)/termination * 50);
        difference = currentPercent - perviousPercent;
        if(currentPercent > perviousPercent)
        {
            perviousPercent = currentPercent;
            for(int i = 0; i < difference; i++)
            {
               if (trackPlus == 9) 
                {
                    printf("+");
                    fflush(stdout);
                    trackPlus = 0;
                }
                else{
                    printf("-");
                    fflush(stdout);
                    trackPlus++;
                }               
            }
        }
    }
    pthread_exit(0);
    return NULL;
}

long wordcount(char filename[], long byteSize)
{
    FILE *file;
    char c;
    char prevC;
    file = fopen(filename, "r");
 
    long totalWords = 0;
    long currentBytes = 0;

    PROGRESS_STATUS progStat;
    progStat.CurrentStatus = &currentBytes;
    progStat.InitialValue = 0;
    progStat.TerminationValue = byteSize;

    pthread_t thread_id;

    pthread_create(&thread_id, NULL, progress_monitor, (void*) &progStat);

    //off by one byte
    c = fgetc(file);
        
    while(c != EOF)
    {
        currentBytes++;
        // if current char is a space, newline, or tab 
        // and previous char was not also a space, newline, or tab
        if ( (c ==' ' || c =='\n' || c == '\t') &&  prevC != ' ' && prevC != '\n' && prevC != '\t')
        {
            totalWords++;
        }
        //fgetc takes in one byte at a time
        prevC = c;
        c = fgetc(file); 
    }

    if (c == EOF)
    {
        totalWords++;
    } 
 
    pthread_join(thread_id, NULL);
    fclose(file);
    
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
    long totalWords = 0;
    char *path = realpath(argv[1], NULL);

    if(argv[1]==NULL) 
    { 
        printf("no file specified"); 
        fflush(stdout);
    } 
 
    totalWords = wordcount(argv[1], size);
    // bigFile has 2191390 words. 13234244 bytes
    
    printf("\n");
    printf("There are ");
    fflush(stdout);
    printf ("%ld", totalWords);
    fflush(stdout);
    printf(" words in ");
    fflush(stdout);
    printf("%s", path);
    fflush(stdout);
    return 0;
}

