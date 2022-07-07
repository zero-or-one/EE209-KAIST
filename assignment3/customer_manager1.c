//Sabina Abdurakhmanova 20180824

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"



#define UNIT_ARRAY_SIZE 1024
#define GROWTH_FACTOR 2 // for supplementary function
#define MIN_ARR_SIZE 2 
struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
			     // # whether the array should be expanded
			     // # or not
};

/*-------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  /* Uncomment and use the following implementation if you want*/ 
  DB_T d;
  
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements
  d->pArray = (struct UserInfo *)calloc(d->curArrSize,
               sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curArrSize);   
    free(d);
    return NULL;
  }
  return d;
  
  return NULL;
}

/*-------------------------------------------------------------------*/
/* To check if DB is valid */
static int DB_isValid(DB_T oDB)
{
    /* Length cannot be negative */
    if (oDB->curArrSize < 0) 
        return 0;
    /* Actual array size cannot be less than minimum possible */  
    if (oDB->curArrSize < MIN_ARR_SIZE)
        return 0;
    /* Length cannot be larger than array size */
    if (oDB->numItems > oDB->curArrSize)
        return 0;
    /* ppvArray should point to valid memory location */
    if (oDB->pArray == NULL)
        return 0;

    return 1; /* Validation SUCCESS */
}

/*-------------------------------------------------------------------*/
/* To expand currArrSize by new customer*/
static void DB_Expand(DB_T oDB)
{
  assert(oDB != NULL); /* Check oDB points to a valid memory location*/
  int NewLength;
  struct UserInfo *pNewArray;

  NewLength = oDB->curArrSize + UNIT_ARRAY_SIZE;
  pNewArray = (struct UserInfo *)realloc(oDB->pArray,
                                  NewLength * sizeof(struct UserInfo));
  if (pNewArray == NULL){
	  fprintf(stderr, "Error, not enough memory to expand the structure");   
  }
  oDB->curArrSize = NewLength;
  oDB->pArray = pNewArray;
}

/*-------------------------------------------------------------------*/
/* Search customer by ID and returns customer position in our Array
	or -1 if we can't find such customer*/
int
FindCustomerPositionByID(DB_T d, const char *id)
{
	if (d==NULL || id==NULL){
		//fprintf(stderr, "Error, null argument\n");
		return(-1);
	}

	assert(DB_isValid(d)); //check d is valid dynamic array

	for (int i=0; i < (d->numItems); i++){
		if(strcmp(id, d->pArray[i].id) == 0){
			return i; // return customer position
		}
	}

	return(-1); // can't find customer with such id
}

/*-------------------------------------------------------------------*/
/* Search customer by NAME and returns customer position in our Array 
	or -1 if we can't find such customer*/
int
FindCustomerPositionByName(DB_T d, const char *name)
{
	if (d==NULL || name==NULL){
		//fprintf(stderr, "Error, null argument\n");
		return(-1);
	}

	assert(DB_isValid(d)); //check d is valid dynamic array

	for (int i=0; i < (d->numItems); i++){
		if(strcmp(name, d->pArray[i].name) == 0){
			return i; // return customer position
		}
	}

	return(-1); // can't find customer with such name
}

/*-------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  if (d == NULL)   //do nothing if d is NULL
    exit(0);
  
  assert(DB_isValid(d)); //check d is valid dynamic array

  if(d -> pArray !=NULL)   //free pArr if exists
    free(d -> pArray);
  
  free(d);
}
/*-------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
	if (d==NULL || id==NULL || name==NULL || purchase<=0){
		return (-1);
	}

	assert(DB_isValid(d)); //check d is valid dynamic array

	struct UserInfo* inf = d->pArray; //use new pointer to access pArray

	//check for repeated name or id
	for (size_t i = 0; i < (d->numItems); i++){
		if (strcmp(name, inf[i].name) == 0){
			fprintf(stderr, "Error, such name already exists\n");
			return(-1);
		}
		else if(strcmp(id, inf[i].id) == 0){
			fprintf(stderr, "Error, such id already exists\n");
			return(-1);
		}
	}

	//expand size of array if it is full
	if ((d->curArrSize) == (d->numItems))
	DB_Expand(d);

	//create new customer
	struct UserInfo *cust;
	cust = calloc(1, sizeof(struct UserInfo));
	cust->id = strdup(id);
	cust->name = strdup(name);
	cust->purchase = purchase;
	
	//add created customer to our array
	d->pArray[d->numItems] = (*cust);
	d->numItems++;

	return (0);
}

/*-------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
	if (d==NULL || id==NULL){
		fprintf(stderr, "Error, null argument\n");
		return(-1);
	}

	assert(DB_isValid(d)); //check d is valid dynamic array
	
	int customerPosition = FindCustomerPositionByID(d, id);
	printf(" customer position = %d \n", customerPosition);
	if(customerPosition == -1){
		fprintf(stderr, "Error, no customer with such id is found");
		return(-1);
	}
	else {
		//swap founded customer with last customer
		struct UserInfo buff = d->pArray[customerPosition];
		d->pArray[customerPosition] = d->pArray[d->numItems - 1];
		d->pArray[d->numItems - 1] = buff;
		printf(" swaping was good \n");

		//TODO: implement RemoveLastCustomer function
		d->numItems--;
		free (d->pArray[d->numItems].id); //free all customer information
		free (d->pArray[d->numItems].name);
		d->pArray[d->numItems].name = NULL; // put cuntomer name to NULL
		return(0);
	}

	return(0);
}

/*-------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
	if (d==NULL || name==NULL){
		fprintf(stderr, "Error, null argument\n");
		return(-1);
	}

	assert(DB_isValid(d)); //check d is valid dynamic array
	
	int customerPosition = FindCustomerPositionByName(d, name);
	if(customerPosition == -1){
		fprintf(stderr, "Error, no customer with such name is found");
		return(-1);
	}
	else {
		//swap founded customer with last customer
		struct UserInfo buff = d->pArray[customerPosition];
		d->pArray[customerPosition] = d->pArray[d->numItems - 1];
		d->pArray[d->numItems - 1] = buff;

		//TODO: implement RemoveLastCustomer function
		d->numItems--;
		free (d->pArray[d->numItems].id);   //free all customer information
		free (d->pArray[d->numItems].name);
		d->pArray[d->numItems].name = NULL; // put cuntomer name to NULL
		return(0);
	}

	return(0);
}

/*-------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
	if (d==NULL || id==NULL){
		fprintf(stderr, "Error, null argument\n");
		return(-1);
	}

	assert(DB_isValid(d)); //check d is valid dynamic array  

	int customerPosition = FindCustomerPositionByID(d, id);
	if(customerPosition == -1){ //if no such id in d
		fprintf(stderr, "Error, no customer with such id is found");
		return(-1);
	}
	
	return(d->pArray[customerPosition].purchase);
}
 
/*-------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
	if (d==NULL || name==NULL){
		fprintf(stderr, "Error, null argument\n");
		return(-1);
	}

	assert(DB_isValid(d)); //check d is valid dynamic array  
	
	int customerPosition = FindCustomerPositionByName(d, name);
	if(customerPosition == -1){ //if no such name in d
		fprintf(stderr, "Error, no customer with such name is found");
		return(-1);
	}
	
	return(d->pArray[customerPosition].purchase);
}
 
/*-------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
	if(d==NULL || fp==NULL){
		fprintf(stderr, "Error, null argument\n");
		return(-1);
	}

	assert(DB_isValid(d)); //check d is valid dynamic array
	unsigned int sum=0;
	for (int i=0; i < (d->numItems); i++) {
		if (d->pArray[i].name != NULL){
			sum +=fp(d->pArray[i].id, d->pArray[i].name, d->pArray[i].purchase);   
		}
	}
	return(sum);
}
