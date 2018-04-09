#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define PATH_MAX 4096

/* VARIABLES */
char *mysort;
char **filePaths;
const char *OPTSTRING = "r";
int lineCounter = 0;
int numberOfPaths = 0;

/* PROTOTYPES */
void usage(void);
char **readLines(char **paths, const int numberOfPaths);
int myCompare(const void *a, const void *b);
void printArray(char **arr, const int size, const int isReverse);

int main(int argc, char **argv)
{
    mysort = argv[0];
    int opt_r = 0;
    filePaths = malloc(0);
    for (int i = 1; i < argc; i++)
    {
        int opt;
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

        char *p = argv[i];
        if (opt == -1 && p[0] != '-')
        {
            filePaths = realloc(filePaths, (numberOfPaths + 1) * sizeof(char *));
            size_t len = strnlen(p, PATH_MAX) + 2;
            filePaths[numberOfPaths] = malloc(sizeof(char) * len);
            strncpy(filePaths[numberOfPaths], p, len);
            numberOfPaths++;
            if ((i + 1) < argc)
            {
                optind = i + 1;
            }
        }
    }

    char **lines = malloc(0);
    if (numberOfPaths) //there is at lease one file name given, because optind optionlari gösterdi simdi argumenti gösteriyor.
    {
        lines = readLines(filePaths, numberOfPaths);
        for (int i = 0; i < numberOfPaths; i++)
        {
            free(filePaths[i]);
        }
        free(filePaths);
    }
    else
    {
        char *line = NULL;
        size_t size;
        int length = 0;
        while ((length = getline(&line, &size, stdin)) != -1)
        {
            if (line[length - 1] == '\n')
            {
                line[length - 1] = '\0';
            }
            lines = realloc(lines, (lineCounter + 1) * sizeof(char *));
            lines[lineCounter] = malloc(size * sizeof(char *));
            strncpy(lines[lineCounter], line, size);
            lineCounter++;
        }
        free(line);
    }
    qsort(lines, lineCounter, sizeof(char *), myCompare);
    printArray(lines, lineCounter, opt_r);

    for (int i = 0; i < lineCounter; i++)
    {
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
            fprintf(stderr, "[%s] ERROR: fopen for file '%s' failed: %s\n", mysort, paths[i], strerror(errno));
            exit(EXIT_FAILURE);
        }

        char *line = NULL;
        size_t size;
        int length = 0;
        while ((length = getline(&line, &size, file)) != -1)
        {
            if (line[length - 1] == '\n')
            {
                line[length - 1] = '\0';
            }
            lines = realloc(lines, (lineCounter + 1) * sizeof(char *));
            lines[lineCounter] = malloc(size * sizeof(char *));
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

void printArray(char **arr, const int size, const int isReverse)
{
    if (isReverse)
    {
        for (int i = size - 1; i >= 0; i--)
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

status: 
- macde ve serverda calisiyor ama server su anda hic biseyi calistirmiyor.
- 1021, 1022 ve 1023 karakterli txt'ler ile ve birer harfli txtler ve yüzlerce bos satirli txtler ve bombos/bozuk txt ile test.
- hizlica dokumentation ve abgabede silincek notlari yaz ve client-server isine basla.
- - files/sockets ve diger en son folieler iyi. folieleri tekrar et.
*/
