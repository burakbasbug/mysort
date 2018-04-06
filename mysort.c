#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* VARIABLES */
char *mysort;
const char *OPTSTRING = "r";
FILE *files = malloc(0);

/* PROTOTYPES */
void usage(void);
char **getFilePaths(int argc, char **argv, int optind);
char **readLines(char **filePaths);

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
        char **filePaths = getFilePaths(argc, argv, optind);
        readLines(filePaths);
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
 * Programa verilen pathleri array'e atar.
 */
char **getFilePaths(int argc, char **argv, int optind)
{
    char **filePaths;
    int numberOfPaths = argc - optind; //number of positional arguments
    printf("%d paths were given\n", numberOfPaths);
    if (numberOfPaths == 0)
    {
        return NULL;
    }
    filePaths = malloc(numberOfPaths * sizeof(char *)); //path sayisi * char-POINTER-size'i, string size'i degil !!! o pointerlarin gösterdigi noktaya ayrica memory atayacagim!!

    for (int i = 0; i < numberOfPaths; ++i)
    {
        long sizeOfPathString = sizeof(argv[optind]);
        //printf("Argument: %s,  sizeOfPathText: %ld\n", argv[optind], sizeOfPath);
        filePaths[i] = malloc(sizeOfPathString);
        strcpy(filePaths[i], argv[optind]); //YADA filePaths[i] = argv[optind];
        //printf("File%d: %s\n", i, filePaths[i]);
        optind++;
    }
    return filePaths;
}

/**
 * Given paths to each file, tries to read each file. If one of the files can not be read, returns null.
 */
char** readLines(char **paths)
{

    if (paths == NULL || paths[0] == NULL)
    {
        printf("bos amk");
        //return NULL;
    }
    
    char **lines = malloc(numberOfPaths * (char*));
    const int NR_OF_CHARS_TO_READ = 100;
    int numberOfPaths = sizeof(paths) / sizeof(paths[0]);
    for (int i = 0; i < numberOfPaths; ++i)
    {
        FILE *file;
        if ((file = fopen(paths[i], "r")) == NULL) //null if EOF
        {
            fprintf(stderr, "[%s] ERROR: fopen failed: %s\n", mysort, strerror(errno));
            exit(EXIT_FAILURE);
        }

        char* line = malloc(0);
        char* buff = malloc(sizeof(char) * NR_OF_CHARS_TO_READ);
        while((buff = fgets(buff, NR_OF_CHARS_TO_READ, file)) != NULL) //fgets bir satiri okumaktayken newline'a rastlarsa orada okumayi durduruyor ama newline char'i okudugu stringe dahil ediliyor.
        {   
            int lastChar = strlen(buff) - 1;
            if(buff[lastChar] == '\n'){
                buff[lastChar] = '\0';
            }
            line = realloc(line, sizeof(buff) + sizeof(line) + 1); //daha cok yer, icerik ayni
            line = strncat(line, buff, NR_OF_CHARS_TO_READ); //ayrilan yere nr kadar buff
            
        }
        printf("%s", line);
        fclose(file);
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
1. read:  C'de her array isleyen fonksiyona array-length verilmesinin nedeni
2. FILE *files = malloc(0); icine acilan dosyalarin birer REFERANSINI AT ki ben baska yerde kapatsam bile acik kalmasinlar ve acik kalirlarsa da ben kapatabileyim.
3. 

*/