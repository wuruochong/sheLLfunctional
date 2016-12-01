#include "rpm.c"

int main() {
  int clear; //define int used in clearing
  char str[C_SIZE] = ""; //define string
  char *line = str; //define pointer
  
  while (1738) { //run indefinitely
    getcwd(str, C_SIZE); //get path
    printf("\nsheLL %s $ ", str); //print sheLL 
    
    if ( fgets(line, C_SIZE, stdin) == 0 ) printf("Error: %s", strerror(errno)); //takes input from stdin

    if ( line[strlen(line) - 1] != '\n' ) { //checks for out of bounds
      printf("Error: Out of bounds; character limit is %d\n", C_SIZE - 1); 
      while ( (clear = getchar()) != '\n' && clear != EOF); //clears stdin
    }
    else {
        line[ strlen(line)-1 ] = 0; //replaces newline
        parse(line); //parses input
    }
  }
  return 0;
}