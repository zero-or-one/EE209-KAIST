/* 
Assignment 1
wc209.c
written: 03/20
updated: 03/22
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#define TRUE 1   // no magic numbers
#define FALSE 0

enum DFAState {  // define state explicitly
  FIRST_CHAR,
  SECOND_CHAR,
  SLASH_COMMENT,
  STAR_COMMENT,
  NOT_COMMENT
};

/* is_space: check for special character */
int is_space(char e){
	switch(e){
	case ' ':
	case '\n':
	case '\t':
	case '\f':
	case '\r':
	case '\v':
	return TRUE;
	}
	return FALSE;
}

/* comment_error: declare error to err stream  
				  if comment is unfinished */
int comment_error(int line_number){   
	fprintf(stderr,
   "Error: line %d: unterminated comment\n", line_number);
	exit(EXIT_FAILURE);
}

/* main: switch between DFA states and make counts */
int main (void) {  
	int nWords = 0, nLines = 0, nChar = 0, enter_num = 0;
	int comment_begin_line;
	char cur_char = ' ', last_char = ' ', char_before_comment = ' ';
	enum DFAState state = FIRST_CHAR;

	do {
	switch(state){
		case FIRST_CHAR: // directly go to other
    	state = SECOND_CHAR;
    	break;
		case SECOND_CHAR:
    	nChar++;
    	if(cur_char == '\n'){
    		nLines++;
	    }
  		state = NOT_COMMENT;
  		//printf(" "); // Some notes for internal representation
  		break;
		case STAR_COMMENT:
			if((last_char == '*') && (cur_char == '/')){
				state = NOT_COMMENT;
				nChar++;
				nLines += enter_num;
				nChar += enter_num;
				if(is_space(char_before_comment) == FALSE){
				  nWords++;
				}
				cur_char = ' ';
			}
			else if(cur_char == '\n') {
				enter_num++; // it counts new lines inside comments
			}
			break;
		case SLASH_COMMENT:
			if (cur_char == '\n') {
				state = NOT_COMMENT;
				nLines++;
				nChar++;
        nWords++;
			}
			break;
		case NOT_COMMENT:
			if((last_char == '/') && (cur_char == '*')){
				cur_char = '-'; // put to cur_char any  non star symbol
				nChar--;
				state = STAR_COMMENT;
				comment_begin_line = nLines;
				enter_num = 0;
			}
			else if ((last_char == '/') && (cur_char == '/')) {
				cur_char = '-';
				nChar--;
        //nWords++;
				state = SLASH_COMMENT;
			}
			else {
				nChar++;
				char_before_comment = last_char;
				if((is_space(cur_char) == TRUE) && (is_space(last_char) == FALSE)){
				nWords++;
				}
				if(cur_char == '\n'){
				nLines++;
				}
			}
			break;
	}
	last_char = cur_char;
	cur_char = getchar();
	} while (cur_char != EOF);
  
	if(is_space(last_char) == FALSE){ // update last
		nWords++;
	}
	
	if(state == STAR_COMMENT){ // unfinished comment
		comment_error(comment_begin_line+1); }
	else {
		printf("%d %d %d\n", nLines, nWords, nChar);
		return EXIT_SUCCESS;
	}
}


