#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* VARIABLES */
char *mysort;
const char *OPTSTRING = "r";
const int BUFFER_SIZE = 1022;
int lineCounter = 0; //struct icine alinacak!

/* PROTOTYPES */
void usage(void);
char **getFilePaths(char **argv, int optind, const int numberOfPaths);
char **readLines(char **paths, const int numberOfPaths);
int myCompare(const void *a, const void *b);
void printArray(char** arr, const int size, const int isReverse);

int main(int argc, char **argv)
{
    mysort = argv[0];
    int opt, opt_r = 0;
    while ((opt = getopt(argc, argv, OPTSTRING)) != -1)
    {
        switch (opt)
        {
        case 'r':
            opt_r++; // must be the first in argument list
            break;
        default:
            usage();
            return EXIT_FAILURE;
        }
    }

    if (argv[optind]) //there is at lease one file name given, because optind optionlari gösterdi simdi argumenti gösteriyor.
    {
        const int numberOfPaths = argc - optind; //number of positional arguments
        char **filePaths = getFilePaths(argv, optind, numberOfPaths);
        if (filePaths != NULL)
        {
            char **lines = readLines(filePaths, numberOfPaths);
            qsort(lines, lineCounter, sizeof(char *), myCompare);
            printArray(lines, lineCounter, opt_r);
        }
    }
    else
    { //no filename => stdin

        char *line = NULL;
        size_t size;
        int length = 0;
        while ((length = getline(&line, &size, stdin)) != -1) {
            if(line[length-1] == '\n'){
                line[length-1] = '\0';
            }
            printf("%s\n", line);

            lineCounter++;
        }
        free(line);
        //usage();
    }
    return EXIT_SUCCESS;
}

void usage(void)
{
    fprintf(stderr, "Usage: %s [-r] [file1] ...\n", mysort);
    exit(EXIT_FAILURE);
}

/**
 * Returns positional arguments (file paths) as an array. 
 */
char **getFilePaths(char **argv, int optind, const int numberOfPaths)
{
    char **filePaths;
    if (numberOfPaths == 0)
    {
        return NULL;
    }
    filePaths = malloc(numberOfPaths * sizeof(char *));

    for (int i = 0; i < numberOfPaths; ++i)
    {
        long sizeOfPathString = sizeof(argv[optind]);
        filePaths[i] = malloc(sizeOfPathString);
        strcpy(filePaths[i], argv[optind]);
        optind++;
    }
    return filePaths;
}

/**
 * Given paths to each file, tries to read each file.
 */
char **readLines(char **paths, const int numberOfPaths)
{
    char **lines = malloc(numberOfPaths * sizeof(char *));
    for (int i = 0; i < numberOfPaths; ++i)
    {
    	FILE *file = NULL;
        if ((file = fopen(paths[i], "r")) == NULL)
        {
            fprintf(stderr, "[%s] ERROR: fopen for file '%s' failed: %s\n", mysort,paths[i] ,strerror(errno));
            exit(EXIT_FAILURE);
        }
        char *buff = malloc(BUFFER_SIZE);
        while (feof(file) || fgets(buff, BUFFER_SIZE, file) != NULL)
        {
            if (!feof(file))
            {
                int lastChar = strlen(buff) - 1;
                buff[lastChar] = '\0';
            }
            lines[lineCounter] = malloc(BUFFER_SIZE);
            strncpy(lines[lineCounter], buff, BUFFER_SIZE);
            lineCounter++;
            if (feof(file))
            {
            	if(file != NULL){
            		fclose(file);
            		file = NULL;
            	}
                break;
            }
        }
        free(buff);
    }
    return lines;
}

int myCompare(const void *a, const void *b)
{
    const char *pa = *(const char **)a;
    const char *pb = *(const char **)b;

    return strcmp(pa, pb);
}

void printArray(char** arr, const int size, const int isReverse){
    if(isReverse){
        for (int i = size-1; i >= 0; i--)
        {
            printf("%s\n", arr[i]);
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            printf("%s\n", arr[i]);
        }
    }
}
/* fgets, scanf
TODOS:
- man 3 exit !!!! !!! !!!! COOOK ÖNEMLI
- tüm metotlar icin errno var mi kontrol!! Varsa errno'lu satir eklenecek.
- synopsis icin man sort!
- makefile check! foliede her asamada ayni parametreleri kullaniyor.
- General Guidelines CHECK
- close resources + close resources in case ERROR
- tutorlara sormak icin bölüm bölüm kesin sorular hazirla, whole-code-check yapmayacaklar.
    - free yapilmasi gereken malloc var mi?
    - realloclar dogru mu?
    - exit stratejim dogru mu?
*/

/*
next:
1. read:  https://stackoverflow.com/questions/33047452/definitive-list-of-common-reasons-for-segmentation-faults
1.1 and NOTE into intro.md
2. FILE *files = malloc(0); icine acilan dosyalarin birer REFERANSINI AT ki ben baska yerde kapatsam bile acik kalmasinlar ve acik kalirlarsa da ben kapatabileyim.
3. FGETS + STDIN : https://stackoverflow.com/questions/3919009/how-to-read-from-stdin-with-fgets

*/
