/**
 * @file mysort.c
 * @author Burak Basbug, 1029465
 * @date 2018-03-15
 * @brief Line sorting program for OSUE exercise 1A `mysort'.
 * @details Similar to sort, it takes files as parameter or reads from stdin and sorts 
 * given lines.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/** Maximum allowed length of a path */
#define PATH_MAX 4096

/* VARIABLES */
static char *mysort; /**< The program name */
static char **filePaths; /**< A list to storage the paths of files to sort their lines */
static const char *OPTSTRING = "r"; /**< The option string */
static int lineCounter = 0; /**< Counter for the read lines */
static int numberOfPaths = 0; /**< Counter for the read paths */

/* PROTOTYPES */
static void usage(void);
static char **readLines(char **paths, const int numberOfPaths);
static int myCompare(const void *a, const void *b);
static void printArray(char **arr, const int size, const int isReverse);

/**
 * @brief It reads lines from the input, sorts and prints them into stdout.
 * @details This method parses given arguments and options to the program,
 * validates and saves them for other methods. If given arguments and options are valid, 
 * it reads lines either from (if given) files or from stdin.
 * global variables: mysort filePaths numberOfPaths lineCounter OPTSTRING
 * @param argc The argument counter.
 * @param argv The argument vector.
 * @return Returns EXIT_SUCCESS, if process executed successfully.
 */
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
                opt_r++;
                break;
            default:
                usage();
                break;
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
    if (numberOfPaths)
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



/* Implementations */

/**
 * @brief Prints the synopsis and leaves the program.
 * @details Prints the synopsis and leaves the program with EXIT_FAILURE.
 * global variables: prog_name
 */
static void usage(void)
{
    fprintf(stderr, "Usage: %s [-r] [file1] ...\n", mysort);
    exit(EXIT_FAILURE);
}

/**
 * @brief Given paths to each file, reads each file.
 * @details It reads each file given in the parameter, reads each of these paths 
 * and saves each line of each file into lines pointer. After each iteration the file
 * will be closed. If a path is invalid, the execution stops and method exists with 
 * EXIT_FAILURE. 
 * global variables: numberOfPaths 
 * @param paths A pointer, which contains paths to read.
 * @param numberOfPaths Indicated the number of paths to read.
 * @return A pointer, which contains all of the lines from given list of files.
 */
static char **readLines(char **paths, const int numberOfPaths)
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

/**
 * @brief Compares strings.
 * @details This method uses strcmp function of C Library. 
 * Given two strings, this method compares them and returns an integer indicator. See 'man 3 strcmp'.
 * @param a String to compare.
 * @param b String to compare.
 * @return An integer indicator for the result of comparison.
 */
static int myCompare(const void *a, const void *b)
{
    const char *pa = *(const char **)a;
    const char *pb = *(const char **)b;

    return strcmp(pa, pb);
}

/**
 * @brief Prints given array to stdout.
 * @details Given a string array, it prints elements of this array into the
 * stdout either in ascending or descending order.
 * @param arr String array to print to the stdout.
 * @param size Size of given string array.
 * @param isReverse Indicator for the printing order of array.
 */
static void printArray(char **arr, const int size, const int isReverse)
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

