// Assignment2 part2 Sabina A 20180824
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for skeleton code */
#include <unistd.h> /* for getopt */
#include "str.h"

#define MAX_STR_LEN 1023

#define FALSE 0
#define TRUE  1


/*
 * Fill out your own functions here (If you need)
 */


// divides string into substring 
// and checks if they all exist in buffer
int SearchStarPattern(char* buf, char* pattern){
    // base case
    if(*pattern=='\0') return TRUE;
    if (!buf)  return FALSE;
    
    while(*pattern == '*')  pattern++;
    
    // when the pattern begins in buffer
    char *start = StrFindChr(buf, *pattern);
    if(start==NULL) 
      return FALSE;
    
    int exists = TRUE;
    int i = 0; // I need to remember the start
    // check one by one
    while ((*pattern != '*') && *pattern != '\0') { // BUG
      if (*start != *pattern)
        exists = FALSE;
      pattern++;
      start++;
      i++;
    
    }
    if (exists)
      return SearchStarPattern(start, pattern);
    start -= i; // please
    pattern -= i;
    start++;
    if (*start == '\0')  return FALSE;
    start = StrFindChr(start, *pattern); // ?
    return SearchStarPattern(start, pattern);
}



/*
pattern = pattern - (len - StrGetLength(pattern));
      while (1){
        while (*pattern == '*')
          pattern++;
        if (*pattern == '\0')
          break;
        char* substring;
        *substring = '\0';
        int count = 0;
        while ((*pattern != '*') && (*pattern != '\0')) {
          *substring = *pattern;
          pattern++;
          substring++;
          count++;
        }
        *substring = '\0';
        count++;
        substring -= count;
        }
*/



/*
// gets the number of * in a string
int calc_asterics(const char* pstr) {
  int astr_num = 0, length = 0;
  while (*pstr != '\0') {
    if (*pstr == '*') {
      astr_num++;
    }
    length++;
  }
  pstr -= length;
  return astr_num;
}

// split the string by dekimeter of asterics
// return array of substrings
char **splitStrings(const char *str) {
    int length, i, j;
    int astr_num = calc_asterics(str);
    length = StrGetLength(str);
    // ininitiate resultant array
    char **substrings = (char **)malloc(sizeof(char *) * (length + 1));
    char ** start = substrings;
    for(i = 0; i < (astr_num + 1); i++) {
        j = 0;
        while(str[j] != '*') j++;
        j++;
        *substrings = (char *)malloc(sizeof(char) * j);
        memcpy(*substrings, str, (j-1));
        (*substrings)[j-1] = '\0';
        str += j;
        substrings++;
    }
    *substrings = '\0';
    return start;
}
*/


 /*--------------------------------------------------------------------*/
 /* PrintUsage()
	print out the usage of the Simple Grep Program                     */
	/*--------------------------------------------------------------------*/
void
PrintUsage(const char* argv0)
{
	const static char* fmt =
		"Simple Grep (sgrep) Usage:\n"
		"%s pattern [stdin]\n";

	printf(fmt, argv0);
}
/*-------------------------------------------------------------------*/
/* SearchPattern()
   Your task:
   1. Do argument validation
   - String or file argument length is no more than 1023
   - If you encounter a command-line argument that's too long,
   print out "Error: pattern is too long"

   2. Read the each line from standard input (stdin)
   - If you encounter a line larger than 1023 bytes,
   print out "Error: input line is too long"
   - Error message should be printed out to standard error (stderr)

   3. Check & print out the line contains a given string (search-string)

   Tips:
   - fgets() is an useful function to read characters from file. Note
   that the fget() reads until newline or the end-of-file is reached.
   - fprintf(sderr, ...) should be useful for printing out error
   message to standard error

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
   /*-------------------------------------------------------------------*/
int
SearchPattern(const char* pattern)
{
  int len = StrGetLength(pattern);
  char buf[MAX_STR_LEN + 2];
  
  if (len> MAX_STR_LEN){
      fprintf(stderr, "Error: argument is too long\n");
      return FALSE;
    }

  /* Read the line by line from stdin, Note that this is an example */
  while (fgets(buf, sizeof(buf), stdin)) {
    // check input line length 
    if (StrGetLength(buf) > MAX_STR_LEN) {
      fprintf(stderr, "Error: input line is too long\n");
      return FALSE;
    }
    // special case with pattern = ""
    if (*pattern == '\0')
      printf("%s",buf);
    // special case with no *
    else if (!StrFindChr(pattern, '*')) {
      if (StrFindStr(buf, pattern)) {
      	printf("%s",buf);
       }
      }
    else {
      if (SearchStarPattern(buf, (char *)pattern))
        printf("%s",buf);
      }
  }
  return TRUE;
}
/*-------------------------------------------------------------------*/
int 
main(const int argc, const char *argv[]) 
{
  /* Do argument check and parsing */
  if (argc < 2) {
	  fprintf(stderr, "Error: argument parsing error\n");
	  PrintUsage(argv[0]);
	  return (EXIT_FAILURE);
  }

  return SearchPattern(argv[1]) ? EXIT_SUCCESS:EXIT_FAILURE;
}