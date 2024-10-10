#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <stdlib.h>
#include <bool.h>

#define FilePath "Chandrayaan.txt"
#define BSIZE 1000

typedef struct Word {
    char * word;
    int occ;
} Word;


int word_idx (Word * word_list, int len, char * word){
    int idx = -1;
    for (int i=0; i<len, i++){
	if ( strcmp(word_list[i].word, word)  == 0 ){
	    idx = i;
	    break;
	}
    }
    return idx;
}


int main (){
    int file_len;
    char * buffer;

    // opening file 
    FILE * file ;
    file = fopen (FilePath,"r");

    // getting file length
    fseek (file, 0, SEEK_END);
    file_len = ftell(file);
    fseek (file, 0, SEEK_SET);

    // defining buffer 
    buffer  = (char*) malloc (sizeof(char) * (file_len + 1) ) ;

    // reading file content 
    fread (buffer, 1, file_len, file);
    buffer[file_len] = '\0';


    // printing content for testing
    // printf("%s\n",buffer);

    // closing file
    fclose(file);

    // defining array of Word objects
    Word * word_list = (Word*) malloc (sizeof(Word));

    // reading words from buffer and saving their frequency
    char * temp;
    temp = strtok(buffer, " ,.;:(){}'\"!\n\t");
    while (temp != NULL){
	
	printf("%s %d\n",temp, strlen(temp));
	temp = strtok(NULL, " ");
    }

    

    return  0;
}
