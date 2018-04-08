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

    char** lines = malloc(0);
    if (argv[optind]) //there is at lease one file name given, because optind optionlari gösterdi simdi argumenti gösteriyor.
    {
        const int numberOfPaths = argc - optind; //number of positional arguments
        char **filePaths = getFilePaths(argv, optind, numberOfPaths);
        if (filePaths != NULL)
        {
            lines = readLines(filePaths, numberOfPaths);
            for(int i = 0; i<numberOfPaths; i++){
                free(filePaths[i]);
            }
            free(filePaths);
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
            lines = realloc(lines, (lineCounter+1) * sizeof(char*));
            lines[lineCounter] = malloc(size * sizeof(char*));
            strncpy(lines[lineCounter], line, size); 
            lineCounter++;
        }
        free(line);
    }
    qsort(lines, lineCounter, sizeof(char *), myCompare);
    printArray(lines, lineCounter, opt_r);
    
    for(int i = 0; i<lineCounter; i++){
        free(lines[i]);
    }
    free(lines);

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
    char **lines = malloc(0);
    for (int i = 0; i < numberOfPaths; ++i)
    {
        FILE *file;
        if ((file = fopen(paths[i], "r")) == NULL)
        {
            fprintf(stderr, "[%s] ERROR: fopen for file '%s' failed: %s\n", mysort,paths[i] ,strerror(errno));
            exit(EXIT_FAILURE);
        }

        char *line = NULL;
        size_t size;
        int length = 0;
        while ((length = getline(&line, &size, file)) != -1) {
            if(line[length-1] == '\n'){
                line[length-1] = '\0';
            }
            printf("Line: '%s\n", line);
            lines = realloc(lines, (lineCounter+1) * sizeof(char*));
            lines[lineCounter] = malloc(size * sizeof(char*));
            strncpy(lines[lineCounter], line, size); 
            lineCounter++;
        }
        fclose(file);
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
- General Guidelines CHECK
- close resources + close resources in case ERROR
- tutorlara sormak icin bölüm bölüm kesin sorular hazirla, whole-code-check yapmayacaklar.
    - exit stratejim dogru mu?
    - makefile'im dogru mu
    - test dosyalarim / caselerim yeterli mi?
    - 
*/

/*
next:
0. read:  https://stackoverflow.com/questions/33047452/definitive-list-of-common-reasons-for-segmentation-faults
1. man 3 getline = GETLINE'I YALAYIP YUT!!


status: 
- (macde) stdin ile bazen segmentation fault 11 veriyor
- (serverda) dosya kapatma sorunlu.
*/
