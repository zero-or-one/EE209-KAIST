// Assignment2 part1 Sabina Abdurakhmanova 20180824

#include <assert.h> /* to use assert() */
#include <stdio.h>
#include <stdlib.h> /* for strtol() */
#include <string.h>
#include "str.h"

#include <limits.h>
#define IN 1  // for StrSearch function 
#define OUT 0

/* Your task is: 
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the corresponding C standard library
      function.
   2. Write appropriate comment per each function
*/

/* Part 1 */
/*------------------------------------------------------------------------*/

// returns character number of the string except \0 character 
size_t StrGetLength(const char* pcSrc)
{
  const char *pcEnd;
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
  pcEnd = pcSrc;
	
  while (*pcEnd) /* null character and FALSE are identical. */
    pcEnd++;

  return (size_t)(pcEnd - pcSrc);
}

/*------------------------------------------------------------------------*/

// copies source string into destination string
char *StrCopy(char *pcDest, const char* pcSrc)
{
  assert(pcSrc);
  assert(pcDest);
  const char *start; 
  start = pcDest;   // keep address of Dest string
  while (*pcSrc != '\0')
    {
    *pcDest = *pcSrc;
    pcDest++;
    pcSrc++;
   }
  *pcDest='\0';   // End string with \0
  return (char*) start;
}

/*------------------------------------------------------------------------*/

// compares  string1 and string 2. Return 0 if they match,
// positive integer if string1 is larger (by length or 1s unmached character asc2 code),
// negative number if string1 is smaller 
int StrCompare(const char* pcS1, const char* pcS2)
{
	assert(pcS1);
	assert(pcS2);
	size_t Str1Length = StrGetLength(pcS1);
	size_t Str2Length = StrGetLength(pcS2);
	if (Str1Length > Str2Length)  
	  return 1;                
	else if (Str1Length < Str2Length)
	  return -1;               
	else { 
    int i = 0;
    int j = 0;
	  while(*(pcS1+i) != '\0')
	    {   
	      if (*(pcS1+i) > *(pcS2+j))  // first unmatched character's ASC2 code is larger 
	      return 1;
	    else if(*(pcS1+i) < *(pcS2+j)) // 1st unmatched character is smaller
	      return -1;
	    i++;
	    j++;
	    }
	  return 0; //return match if all above is satisfied
	}
}

/*------------------------------------------------------------------------*/

// returns a pointer to the first occurrence
// of the character c in the string pcHaystack.
char *StrFindChr(const char* pcHaystack, int c)
{
  assert(pcHaystack);
  // assert((c < 255) || (c >= 0));
  
  // handle special case
  if (c==0){
    return (char*)pcHaystack + StrGetLength(pcHaystack);
  }
  int i = 0;
  while (*(pcHaystack + i) != '\0') {
    if (*(pcHaystack + i) == c) {
      return (char*) (pcHaystack + i);
    }
    i++;
  }
  return NULL;
}

/*------------------------------------------------------------------------*/

// returns substring in Haystack starting from Needle till end
// or NULL if no substring is found
char *StrFindStr(const char* pcHaystack, const char *pcNeedle)
{
	assert(pcHaystack);
	assert(pcNeedle);
  int hay_len = StrGetLength(pcHaystack);
  int need_len = StrGetLength(pcNeedle);
  
	if(*pcNeedle == '\0'){  //special case: Neddle string is null
		return (char*) pcHaystack;
	}
	const char *start;
	int state;
	while (*pcHaystack != '\0' && *pcNeedle != '\0') {
		state = OUT;
		if (*pcHaystack == *pcNeedle) {
			state=IN;
			start=pcHaystack;
			const char *pcNeedleBackUp = pcNeedle; 
	      				
			while (*pcNeedle && *pcHaystack){ 
				if (*pcHaystack != *pcNeedle) {
					state = OUT;
					break;
				}
				pcHaystack++;
				pcNeedle++;
			}
			if(state == IN) break; //Substring is found, go tu return value
			pcNeedle = pcNeedleBackUp;
			//to bring pcNeedle and pcHaystack 
      // to initial state after while loop
			pcHaystack = start; 
		}
		pcHaystack++;
	}
  // bring back
  pcHaystack -= (hay_len - StrGetLength(pcHaystack));
  pcNeedle -= (need_len - StrGetLength(pcNeedle));
  
	if (state == IN) {
	  return (char*)start;
	} else {
	  return NULL; // no Needle in Haystack
	}
}

/*------------------------------------------------------------------------*/

// joins Src to the end of Dest string
char *StrConcat(char *pcDest, const char* pcSrc)
{
  assert(pcDest);
  assert(pcSrc);
    size_t DestLength = StrGetLength(pcDest);
    char *pcEnd = pcDest + DestLength; //go to end of string
	while (*pcSrc){
	  *pcEnd = *pcSrc; 
		pcEnd++;
		pcSrc++;
	}
	*pcEnd='\0';
	return pcDest;
}

/*------------------------------------------------------------------------*/

// converts a string to a long integer
long int StrToLong(const char *nptr, char **endptr, int base)
{
  /* handle only when base is 10 */
  if (base != 10) return 0;
  
  const char *p = nptr;
  const char *strp = nptr;
  int is_negative = 0;
  int is_overflow = 0;
  unsigned long number = 0UL;
  // unsigned long digit;
  int digit;
  
  while (*p && (*p == ' ' || *p == '\n' || *p == '\t' \
        || *p == '\v' || *p == '\f' || *p == '\r')) p++;
  if (*p == '+') {
      p++;
  } 
  else if (*p == '-') {
      is_negative = 1; // True
      p++;
  }
  // define overflow limits
  unsigned long dec_limit = (is_negative) ? -(LONG_MIN / 10) : LONG_MAX / 10;
  int dig_limit = (is_negative) ? -(LONG_MIN % 10) : LONG_MAX % 10;
  
  while (*p && ((*p>='0') && (*p<='9')))
  { 
		digit = *p - '0';
    if (number > dec_limit || (number == dec_limit && digit > dig_limit)) {
      is_overflow = 1;
      break;
    }
		number *= 10;
    number += digit; 
  	p++;
    strp = p;
  }
  // find the beginning of string in case of overflow
  while (*p && ((*p>='0') && (*p<='9'))) {
    strp = ++p;
  }
  if (endptr) *endptr = (char *)strp;
  
  if (is_overflow) {
    return ((is_negative) ? LONG_MIN : LONG_MAX);
  }
  
  return (long)((is_negative) ? -number : number);
}

/*------------------------------------------------------------------------*/

// lowers all characters in the string
char *StrToLower(char *str)
{
  //char* start = str;
  int len = StrGetLength(str);
  assert(str);
  while (*str != '\0') {
    if (*str >= 'A' && *str <= 'Z') {
      *str += 32;
    }
    str++;
  }
   return str - len;
}
