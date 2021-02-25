/* 
 * CS 570
 * Professor Roch
 * Assignment 2: Wordcount/ProgressBar
 * This program uses POSIX threads
 * This program takes in a file specifed by the user and counts the number of words in that file
 * A progress bar prints out while the main thread, wordcount, is counting the number of words.
 * After the progress bar is complete, the program will print out the number of words in the file specified
 * 
 * 
 *
 * We the undersigned promise that we have in good faith attempted to follow the principles of 
 * pair programming. Although we were free to discuss ideas with others, the implementation is 
 * our own. We have shared a common workspace and taken turns at the keyboard for the majority
 * of the work that we are submitting. Furthermore, any non programming portions of the
 * assignment were done independently. We recognize that should this not be the case, we will
 * be subject to penalties as outlined in the course syllabus.
 * Anoodnya Sangam
 * Vivian Reyes
 */

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

    //termination is the fileSize that is being computed
    float termination = progStat->TerminationValue - progStat->InitialValue;
    //tracking current percent complete of wordcount with currentPercent
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
            int i;
            for(i = 0; i < difference; i++)
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
        if ( (c ==' ' || c =='\n' || c == '\t') &&  prevC != ' ' && prevC != '\n' && prevC != '\t' && prevC != '\0')
        {
            totalWords++;
        }
        //fgetc takes in one byte at a time
        prevC = c;
        c = fgetc(file); 
    }

    if (c == EOF && prevC != ' ' && prevC != '\n' && prevC != '\t')
    {
        totalWords++;
    } 
 
    pthread_join(thread_id, NULL);
    fclose(file);
    
    return totalWords;
}
 
//main takes a command line argument of the filename, 
//calls wordcount to count total number of words of file specified
int main (int argc, char** argv)
{
    // use stat to get number of bytes in file
    struct stat buf;  
    stat(argv[1], &buf);
    long byteSize = buf.st_size;
    long totalWords = 0;
    //sets file relative path to char pointer path
    char *path = realpath(argv[1], NULL);

    FILE *file;
    char c;
    char prevC;
    file = fopen(argv[1], "r"); 

    //if user did not enter a file, print error messege
    if(argv[1]==NULL) 
    { 
        printf("no file specified"); 
        printf("\n");
        return;
    } 

    if(file==NULL) 
    { 
         printf("could not open file"); 
         printf("\n");
         return;

    }

 
    //calling wordcount, returns a long and placing that into totalWords var.
    //pass byteSize into wordCount, this is used later for TerminationValue
    totalWords = wordcount(argv[1], byteSize);
    
    //print out total number of words with file path
    printf("\n");
    printf("There are " );
    printf ("%ld", totalWords);
    printf(" words in ");
    printf("%s", argv[1]);
    printf(".");
    printf("\n");
    return 0;
}

