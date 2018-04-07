#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* VARIABLES */
char *mysort;
const char *OPTSTRING = "r";
int lineCounter = 0; //struct icine alinacak!

/* PROTOTYPES */
void usage(void);
char **getFilePaths(char **argv, int optind, const int numberOfPaths);
char **readLines(char **paths, const int numberOfPaths);

int myCompare(const void *a, const void *b);

int main(int argc, char **argv)
{
    mysort = argv[0];
    int opt, opt_r = 0;
    while ((opt = getopt(argc, argv, OPTSTRING)) != -1)
    {
        switch (opt)
        {
        case 'r':
            opt_r++;
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
            for (int i = 0; i < lineCounter; i++)
            {
                printf("%s\n", lines[i]);
            }
        }
    }
    else
    { //no filename => stdin
        usage();
    }
    return EXIT_SUCCESS;
}

void usage(void)
{
    fprintf(stderr, "Usage: %s [-r] [file1 ...]\n", mysort);
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
    filePaths = malloc(numberOfPaths * sizeof(char *)); //path sayisi * char-POINTER-size'i, string size'i degil !!! o pointerlarin gösterdigi noktaya ayrica memory atayacagim!!

    for (int i = 0; i < numberOfPaths; ++i)
    {
        long sizeOfPathString = sizeof(argv[optind]);
        filePaths[i] = malloc(sizeOfPathString);
        strcpy(filePaths[i], argv[optind]); //YADA filePaths[i] = argv[optind];
        optind++;
    }
    return filePaths;
}

/**
 * Given paths to each file, tries to read each file.
 */
char **readLines(char **paths, const int numberOfPaths)
{

    if (paths == NULL || paths[0] == NULL)
    {
        printf("bos amk");
        //return NULL;
    }

    char **lines = malloc(numberOfPaths * sizeof(char *));
    //char **lines = NULL;
    const int NR_OF_CHARS_TO_READ = 1022;
    for (int i = 0; i < numberOfPaths; ++i)
    {
        FILE *file;
        if ((file = fopen(paths[i], "r")) == NULL) //null if EOF
        {
            fprintf(stderr, "[%s] ERROR: fopen failed: %s\n", mysort, strerror(errno));
            exit(EXIT_FAILURE);
        }

        char *buff = malloc(NR_OF_CHARS_TO_READ);
        while (feof(file) || (buff = fgets(buff, NR_OF_CHARS_TO_READ, file)) != NULL)
        {

            if (!feof(file))
            {
                int lastChar = strlen(buff) - 1;
                buff[lastChar] = '\0'; //satir bitti
            }

            lines[lineCounter] = malloc(NR_OF_CHARS_TO_READ);       //lines'a yeni malloc
            strncpy(lines[lineCounter], buff, NR_OF_CHARS_TO_READ); //line -> lines
            lineCounter++;

            if (feof(file))
            {
                break;
            }
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
