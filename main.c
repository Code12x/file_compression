#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int scan_buf(char* buffer, int width, int offset);
void add_compression_meta(char* buffer, int width, int pos, char* meta_buf);
int meta_buf_items_count(char* meta_buf, size_t size);

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
    size_t meta_buf_size = sizeof(char) * 9;
    char* meta_buf = malloc(meta_buf_size);
    meta_buf[0] = '!';
    meta_buf[1] = '<';
    meta_buf[2] = 07;
    meta_buf[3] = '>';
    meta_buf[4] = '<';
    meta_buf[5] = 07;
    meta_buf[6] = '>';
    meta_buf[7] = '!';
    meta_buf[8] = '\n';

    meta_buf_items_count(meta_buf, meta_buf_size);
    return 0;

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
            if (current_occurence == next_occurence) {  // The current string can be longer to maximize string replacement.
                width++;
                continue;
            } else if (current_occurence < next_occurence){
                fputs("SOME WEIRD ERROR", stderr);
                exit(-1);
            } else {
                /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
                // ALERT! Before compressing the section, walk through
                // the section with position+1 and width-1 until width
                // is = 4 and then compress the option that saved the
                // most bytes.
                // (EX. "3 INFO [" would end up giving up the 3 and then compressing the " INFO [")
                /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
                // compress_sect(buffer, width, pos, res_buf);
            }
        }
    }

    free(buffer);
    free(meta_buf);

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
 * The meta data begins with !<<07>>
 * and ends with <<07>>!
 *
 * The meta data stores a set of chars as a single char.
 * (ex. <200>=this repeats<06>) with <char> representing the char by the ascii code (<200> = char of 200)
 * char values range from 174 to 255
 *
 * Each short-hand is ended by a <06>
 * 
*/

void add_compression_meta(char* buffer, size_t buffer_size, int width, int offset, char* meta_buf, size_t meta_buf_size) {
   char* pointer_map[];
}


int meta_buf_items_count(char* meta_buf, size_t size) {
    if (size < 9) {
        if (meta_buf[0] != '!' && meta_buf[1] != '<' && meta_buf[2] != 07 && meta_buf[3] != '>'
                && meta_buf[size-1] != '!' && meta_buf[size-2] != '>' && meta_buf[size-3] != 07 && meta_buf[size-4] != '<') {
            return -1;
        }
    }

    int count = 0;

    for (int i=4; i<size-4; i++) {

    }
}


