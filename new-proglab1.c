
#include <stdio.h>
#include <string.h>

#define WORD_STARTED 1
#define WORD_ENDED 0

void Error(void);
int main(int argc, char *argv[])
{
    printf("\n");
    int file_index = argc - 1;
    FILE *file;
    file = fopen(argv[file_index], "r");
    if (argc != 3 || file == NULL)
    {
        Error();
        return -1;
    }
    else
    {
        int col_words = 0;
        int col_lines = 0;
        int col_bytes = 0;
        int word_status = 0;
        char read_char = getc(file);

        while (read_char != EOF)
        {
            if (read_char == '\n')
            {
                col_lines++;
            }
            if (read_char == ' ' || read_char == '\t' || read_char == '\n' || read_char == '\v')
            {
                word_status = WORD_ENDED;
            }
            else if (word_status == WORD_ENDED)
            {
                word_status = WORD_STARTED;
                col_words++;
            }
            read_char = getc(file);
        }

        fseek(file, 0, 2);
        col_bytes = ftell(file);
        fseek(file, -1, 2);
        read_char = getc(file);

        if (read_char != '\n')
        {
            col_lines++;
        }

        if (strcmp(argv[1], "--lines") == 0 || strcmp(argv[1], "-l") == 0)
        {
            printf("Number of lines: %d\n", col_lines);
        }
        else if (strcmp(argv[1], "--words") == 0 || strcmp(argv[1], "-w") == 0)
        {
            printf("Number of words: %d\n", col_words);
        }
        else if (strcmp(argv[1], "--bytes") == 0 || strcmp(argv[1], "-c") == 0)
        {
            printf("Number of bytes: %d\n", col_bytes);
        }
        else if (argv[1][0] == '-')
        {
            int number_of_char[4] = {0, 0, 0, 0}; // 0-l, 1-c, 2-w, 3-not {l,c,w}
            for (int i = 1; i < strlen(argv[1]); i++)
            {
                if (argv[1][i] == 'l')
                {
                    number_of_char[0]++;
                }
                else if (argv[1][i] == 'c')
                {
                    number_of_char[1]++;
                }
                else if (argv[1][i] == 'w')
                {
                    number_of_char[2]++;
                }
                else
                {
                    number_of_char[3]++;
                }
            }
            if (number_of_char[3] > 0 || number_of_char[0] > 1 || number_of_char[1] > 1 || number_of_char[2] > 1)
            {
                Error();
                return -1;
            }
            else
            {
                for (int i = 1; i < strlen(argv[1]); i++)
                {
                    if (argv[1][i] == 'l')
                    {
                        printf("Number of lines: %d\n", col_lines);
                    }
                    else if (argv[1][i] == 'c')
                    {
                        printf("Number of bytes: %d\n", col_bytes);
                    }
                    else if (argv[1][i] == 'w')
                    {
                        printf("Number of words: %d\n", col_words);
                    }
                }
            }
        }
        else
        {
            Error();
            return -1;
        }
        fclose(file);
    }

    return 0;
}
void Error()
{
    printf("The command should look like this:\n     WordCount.exe [OPTION] filename\n");
    printf("These are common WordCount commands used in various situations:\n");
    printf("     --lines          Show the number of lines\n");
    printf("     --bytes          Show the file size in bytes\n");
    printf("     --words          Show the number of words\n");
    printf("You can use short commands: { [-l] for [--lines], [-c] for [--bytes], [-w] for --words }\n");
}