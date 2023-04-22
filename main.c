#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int scan_buf(char* buffer, int width, int offset);

void compress_sect(char* buffer, int width, int pos, char* res_buf);


int main(int argc, char *argv[]) {
    if (argc < 1) {
        exit(-1);
    }

    char* file_name = argv[1];

    FILE* pFile = fopen(file_name, "rb");
    char* buffer;
    size_t size;
    size_t result;

    if (pFile == NULL) {
        printf("Error reading %s!", file_name);
        exit(-1);
    }

    fseek(pFile, 0, SEEK_END);
    size = ftell(pFile);
    rewind(pFile);

    buffer = (char*) malloc(sizeof(char)*size);
    if (buffer == NULL) {
        fputs("Memory error", stderr);
        exit(2);
    }

    result = fread(buffer, 1, size, pFile);
    if (result != size) {
        fputs("Error reading file", stderr);
    }


    // The bit of info at the top to help decompress the file
    char meta_info[size];
    char* meta_buf = (char*) &meta_info;

    // The result, compressed file
    char res[size];
    char* res_buf = (char*) &res;


    // Compress algorithm
    int pos = 0;
    while (pos <= size/2) {
        int width = 2;
        int cycle_flag = 0;
        while (!cycle_flag) {
            int current_occurence = scan_buf(buffer, width, pos);
            int next_occurence = scan_buf(buffer, width+1, pos);
            if (current_occurence == next_occurence) { // The current string can be longer to maximize string replacement.
                width++;
                continue;
            } else if (current_occurence < next_occurence){
                fputs("SOME WEIRD ERROR", stderr);
                exit(-1);
            } else {
                compress_sect(buffer, width, pos, res_buf);
            }
        }
    }

    free(buffer);

    return 0;
}


int scan_buf(char* buffer, int width, int offset) {

}


void compress_sect(char* buffer, int width, int pos, char* res_buf) {

}

