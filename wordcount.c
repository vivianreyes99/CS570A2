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
 * 
 * Anoodnya Sangam RED ID: 821598603
 * Vivian Reyes RED ID: 821862269
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>


/*
* Current status is a pointer to a long which represents the current status of the computation being tracked
* InitialValue is the starting value for the computation
* TerminationValue is the value at which the computation is complete, aka, number of bytes in file
* We may assume TerminationValue >= Progress Indicator (*CurrentStatus) >= InitialValue
*/
typedef struct {
long * CurrentStatus;
long InitialValue;
long TerminationValue;
} PROGRESS_STATUS;

/*
* progress_monitor functions monitors the progress of a task that executes
in a separate thread
* the void * argument is a pointer to the PROGRESS_STATUS struct
* loops until the progress bar is printed and complete and exits
* prints hyphens (-) for progress bar, but every 10th progress mark should be dislpayed as a plus (+)
*/ 
void * progress_monitor(void * progStatus)
{
    //typecast the void pointer to a PROGRESS_STATUS pointer
    PROGRESS_STATUS *progStat = (PROGRESS_STATUS *) progStatus;

    //termination is the fileSize that is being computed
    float termination = progStat->TerminationValue - progStat->InitialValue;
    //tracking current percent complete of wordcount with currentPercent var
    int currentPercent = 0;
    //tracking pervious percent complete of last iteration
    int perviousPercent = 0;
    //difference used to find the difference of currentPercent and perviousPercent
    int difference = 0;
    //var to track how many hyphens have been printed to print a + at every tenth hyphen
    int trackPlus = 0;

    //while condition loops until 50 hyphens have been printed, when exits progress bar/wordcount is complete
    while(currentPercent < 50)
    {
        //currentPercent is set to the ratio of percent complete based on 50 hyphens
        currentPercent = (*(progStat -> CurrentStatus)/termination * 50);
        //difference is used for how many hyphens need to printed since the last iteration
        difference = currentPercent - perviousPercent;

        //if the perecent completed(currentPercent) has increased since last iteration 
        //then need to print more hyphens
        if(currentPercent > perviousPercent)
        {
            //set perviousPercent to currentPercent so that it is set up for the next iteration
            perviousPercent = currentPercent;
            int i;
            //loops difference amount of times bc differnce = to num of hyphens to print in current iteraiton
            for(i = 0; i < difference; i++)
            {
               //when trackPlus == 9 then we print a "+" bc it is at it's 10th hyphen of progress bar
               //else print hyphen
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

/*
* simple program which given a filename determines the number of words in the file
* returns a long integer with the number of words and takes a file descriptor or filename as input
* wordcount will spawn a progress_monitor thread with a populated PROGRESS_STATUS structure as the argument
*/
long wordcount(char filename[], long byteSize)
{
    // create and open file for reading
    FILE *file;
    char c;
    char prevC;
    file = fopen(filename, "r"); 
 
    long totalWords = 0; // keeps count of the total words in file
    long currentBytes = 0; // keeps count of the total bytes (chars)

    // assign values to struct progStat, that holds all the information about the file
    PROGRESS_STATUS progStat;
    progStat.CurrentStatus = &currentBytes;
    progStat.InitialValue = 0;
    progStat.TerminationValue = byteSize;

    pthread_t thread_id;
    
    // if file does not exist, print error message
    if(file==NULL) 
    { 
        printf("could not open file"); 
        fflush(stdout);
        printf("\n");
        fflush(stdout);
        exit(0);
    } 

    // creates separate thread to run the progress_monitor function
    // which updates the progress bar as the words are counted
    pthread_create(&thread_id, NULL, progress_monitor, (void*) &progStat);

    c = fgetc(file); // read one byte at a time
        
    while(c != EOF) // loop through file until it reaches the EOF, and update the struct with info
    {
        currentBytes++;
        // if current char is whitespace, and previous char was NOT whitespace, increment words
        if ( (c ==' ' || c =='\n' || c == '\t') &&  prevC != ' ' && prevC != '\n' && prevC != '\t' && prevC != '\0')
        {
            totalWords++;
        }
        prevC = c; // set prev char to current char, to be able to check in the next loop
        c = fgetc(file); // read next char
    }

    // if it reaches the EOF, and the prev char was not whitespace, increment words
    if (c == EOF && prevC != ' ' && prevC != '\n' && prevC != '\t')
    {
        totalWords++;
    } 
 
    pthread_join(thread_id, NULL); //  wait til progress monitor function is done running to return to main
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

    //if user did not enter a file, print error messege
    if(argv[1]==NULL) 
    { 
        printf("no file specified"); 
        printf("\n");
        return 0;
    } 
 
    //calling wordcount, returns a long and placing that into totalWords var.
    //pass byteSize into wordCount, this is used later for TerminationValue
    totalWords = wordcount(argv[1], byteSize);
    
    //print out total number of words with file name
    printf("\n");
    printf("There are " );
    printf ("%ld", totalWords);
    printf(" words in ");
    printf("%s", argv[1]);
    printf(".");
    printf("\n");
    return 0;
}

