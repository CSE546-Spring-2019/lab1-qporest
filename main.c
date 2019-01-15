#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* How many bytes to read at once */
#define BUFFER_SIZE 20
/* Length of search string */
#define MAX_SEARCH_LENGTH 20

void help();
long get_file_size(FILE * file);
int count_repeats(char * search, FILE * file);

int main(int argc, char *argv[])
{
    if(argc < 4){
        help();
        exit(1);
    }
    FILE * input_file;
    FILE * out_file;
    char * search = argv[2];
    int count;
    long size;

    input_file = fopen(argv[1], "r");
    if (input_file == NULL){
        printf("Input file %s cannot be opened.\n", argv[1]);
        help();
        exit(1);
    }
    out_file = fopen(argv[3], "w");
    if (out_file == NULL){
        printf("Output file %s cannot be opened.\n", argv[3]);
        help();
        exit(1);
    }
    /* Get data */
    size = get_file_size(input_file);
    count = count_repeats(search, input_file);

    printf("Size: %ld\n", size);
    printf("Count: %d\n", count);
    fprintf(out_file, "Size: %ld\n", size);
    fprintf(out_file, "Count: %d\n", count);

    /* clean up */
    fclose(input_file);
    fclose(out_file);
    return 0;
}

/* Returns file size and resets cursor to the beginning. */
long get_file_size(FILE * file){
    long size;
    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    rewind(file);
    return size;
}

/*
    Returns count of the search word in file, cursor at the end.
    The algorithm for counting is to keep an array that keeps current "streaks".
    It only gets incremented if the current character is the same as the
    character in search_string[streak #].
    If first letter is the same, new streak is started by making a -1 into a 0.
*/
int count_repeats(char * search, FILE * file){
    char buffer[BUFFER_SIZE];
    char indices[MAX_SEARCH_LENGTH];
    int search_len = strlen(search);
    int i, j;
    int idx;
    int read_bytes;
    int count = 0;

    for(i = 0; i<MAX_SEARCH_LENGTH; i++) indices[i] = -1;
    while(!feof(file)){
        read_bytes = fread(buffer, sizeof(char), BUFFER_SIZE, file);
        for(i=0; i<read_bytes; i++){
            /* it's the first letter of the search string*/
            if(buffer[i] == (search[0] & 0xff)){
                idx = 0;
                /* shoudln't logically ever be out of bounds */
                while(idx<MAX_SEARCH_LENGTH && indices[idx]!=-1){
                  idx++;
                }
                if(idx == MAX_SEARCH_LENGTH){
                    printf("Error occured. Too many matches\n");
                    exit(1);
                }
                indices[idx] = 0;
            }
            /* check is the streaks are updated */
            for(j = 0; j < MAX_SEARCH_LENGTH; j++){
                if(indices[j] == -1) continue;
                if(buffer[i] == (search[(int)indices[j]] & 0xff)){
                    indices[j]++;
                    if(indices[j] == search_len){
                        count++;
                        indices[j] = -1;
                    }
                } else {
                    indices[j] = -1;
                }
            }
        }
    }
    return count;
}

/* Output information about properly calling the file */
void help(){
    printf("\
    Please provide the arguments in the requested format\n\
    program <input> <search string> <output>\n");
}