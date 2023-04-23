#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int scan_buf(char* buffer, int width, int offset);
void add_compression_meta(char* buffer, int width, int pos, char* meta_buf);


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


    /* 
    *
    * ===== Compression Algorithm =====
    *
    * The file is scanned in intervals called cycles.
    * The purpose of a cycle is to get the longest repeating phrase throuout the buffer
    * Each cycle has a starting position in the file and an initial width (4)
    * During the cycle, the buffer will be scanned at the current width as well as the current width +1
    * if the current width +1 has the same number of occurences as the current width,
    * then we start the cycle over again with width being equal to current width +1
    *
    */

    int pos = 0;
    while (pos <= size/2) {
        int width = 4;
        int cycle_finished = 0;
        while (!cycle_finished) {
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

/*
*
* ===== scan_buf =====
* This function returns the number of occurences that a "window" is repeated.
*
* A window is a portion of an array starting at offset and with a width of width.
* A window can be represented by either a new array in memory,
* or as a pointer of an array with a offset and width.
*
* For example:
*
* char my_array[6] = {'h', 'e', 'l', 'l', 'o', '!'};
* size_t = width = 3;
* size_t = offset = 1;
*
* // method 1:
* char* pWindow_1 = &my_array[offset] // it will just have to be accessed one char at a time with (i<width)
* for (int i=0; i<width; i++) {
*     printf("Position %d in window: %c\n", i, my_array[offset+i]);
* }
*
* // method 2:
* char window_2[3];
* for (int i=0; i<width; i++) {
*     window_2[i] = my_array[offset+i];
* }
*
*/

int scan_buf(char* buffer, int width, int offset) {
    int count = 0;
    char control_window[width]; // This is the array that the windows will be checked against
    for (int i=0; i<width; i++) {
        control_window[i] = buffer[offset+i]; // Filling the control array
    }

    for (int i=0; i<strlen(buffer)-width; i++) {
        char* currsor = &buffer[i];
        int is_repeat = 0;
        if (*currsor == control_window[0]) { // The next few characters might be a repeat of the control_window
            is_repeat = 1;
            for (int j=1; j<width; j++) {
                if (buffer[i+j] != control_window[j]) { // The current window has a character that does not match the control
                    is_repeat = 0;
                    break;
                }
            }
        }
        if (is_repeat) { // The current window is indeed a repeat of the control window
            count++;
        }
    }
    return count;
}

/*
 *
 * ===== add_compression_meta =====
 * The purpose of this function is to create the short-hand notation that
 * will be used in the compressed version of the file and add the short-hand
 * to the meta_buf. The meta_buf is an array that will be added to the
 * top of the result file for the decompression algorithm to use to readd
 * the original content
 *
*/

void add_compression_meta(char* buffer, int width, int pos, char* meta_buf) {

}

