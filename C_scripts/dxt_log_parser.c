#include <stdio.h>
#include <stdlib.h>
#include <string.h>


 
int main(int argc, char* argv[]) {
    FILE    *textfile;
    char    *text;
    long    numbytes;
     
    char* filename = argv[1];
    textfile = fopen(filename, "r");
    if(textfile == NULL)
        return 1;
     
    fseek(textfile, 0L, SEEK_END);
    numbytes = ftell(textfile);
    fseek(textfile, 0L, SEEK_SET);  
 
    text = (char*)calloc(numbytes, sizeof(char));   
    if(text == NULL)
        return 1;
 
    fread(text, sizeof(char), numbytes, textfile);
    fclose(textfile);

    int start_pos = match(text, " X_POSIX ");
    text = text + start_pos;

    log_part = 

    printf();

 
    printf("%d\n", numbytes);

 
    return 0;
}

int match(char text[], char pattern[]) {
  int c, d, e, text_length, pattern_length, position = -1;

  text_length    = strlen(text);
  pattern_length = strlen(pattern);

  if (pattern_length > text_length) {
    return -1;
  }

  for (c = 0; c <= text_length - pattern_length; c++) {
    position = e = c;

    for (d = 0; d < pattern_length; d++) {
      if (pattern[d] == text[e]) {
        e++;
      }
      else {
        break;
      }
    }
    if (d == pattern_length) {
      return position;
    }
  }
}

