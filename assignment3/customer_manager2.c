//Sabina A 20180824 assignment3
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/*-------------------------------------------------------------------*/
#define UNIT_ARRAY_SIZE 1024
#define HASH_MULTIPLIER 65599

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
  struct UserInfo *next_id, *next_name;
  struct UserInfo *prev_id, *prev_name;
};

struct T{
  struct UserInfo *array[UNIT_ARRAY_SIZE]; //user info will be stored
};

struct DB {
  struct UserInfo *pArray;  
  int iBucketCount;            
  int numItems;
  struct T *id;		   //stored either by id
  struct T *name;	   //or name  
};

/*-------------------------------------------------------------------*/
// Return a hash code for pcKey that is between 0 and iBucketCount-1,
//   inclusive. Adapted from the EE209 lecture notes.
static int hash_function(const char *pcKey, int iBucketCount)
{
   int i;
   unsigned int uiHash = 0U;
   for (i = 0; pcKey[i] != '\0'; i++){
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)pcKey[i];
   }
   return (int)(uiHash % (unsigned int)iBucketCount);
}

/*-------------------------------------------------------------------*/
/* Expand the table by twice of its size please*/
void
ExpandTable(DB_T d) {
  return;
}


/*-------------------------------------------------------------------*/
/* Search customer by ID and returns customer node in our HashTable
	 or NULL if we can't find such customer*/
struct UserInfo *
FindCustomerNodeByID(DB_T d, const char *id)
{
	if (d==NULL || id==NULL){
		fprintf(stderr, "Error, invalid input argument\n");
		return NULL;
	}

	int key_id = hash_function(id, d->iBucketCount);

	//find customer with id
	struct UserInfo *p;
	for(p=d->id->array[key_id]; p!=NULL; p=p->next_id){
		if(strcmp(p->id, id)==0){
			return p;
		}    
	}
	return NULL; // can't find customer with such id
}
/*-------------------------------------------------------------------*/
/* Search customer by NAME and returns customer node in our HashTable 
	or NULL if we can't find such customer*/
struct UserInfo *
FindCustomerNodeByName(DB_T d, const char *name)
{
	if (d==NULL || name==NULL){
		fprintf(stderr, "Error,invalid input argument\n");
		return NULL;
	}

	int key_name = hash_function(name, d->iBucketCount);

	//find customer with such name
	struct UserInfo *p;
	for(p=d->name->array[key_name]; p!=NULL; p=p->next_name){
		if(strcmp(p->name, name)==0){
			return p;
		}    
	}
	return NULL; // can't find customer with such name
}

/*-------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
	DB_T d = (DB_T)calloc(1, sizeof(struct DB));

	if (d==NULL) {
		fprintf(stderr, "Can't allocate a memory for DB_T\n");  
		free(d);
		return NULL;
	}

	d->iBucketCount = UNIT_ARRAY_SIZE;
	d->numItems = 0;
	d->name = (struct T*)calloc(1, sizeof(struct T));
	d->id = (struct T*)calloc(1, sizeof(struct T));
	
	//if either is null-ERROR
	if(d->name==NULL||d->id==NULL){
		fprintf(stderr, "Can't allocate a memory for array of size%d\n",
		d->iBucketCount);  
		free(d);
		return NULL;    
	}

	return d;
}
/*-------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  if (d==NULL)
    exit(0);
  
  free(d->id);
  free(d->name);
  free(d);
}
/*-------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
	if (d==NULL || id==NULL || name==NULL || purchase<=0){
		fprintf(stderr, "Error,invalid input argument\n");
		return (-1);
	}

	int key_id = hash_function(id, d->iBucketCount);
	int key_name = hash_function(name, d->iBucketCount);

	//search if name or id exist
	struct UserInfo *p;

	for(p=d->id->array[key_id]; p!=NULL; p=p->next_id){
		if(strcmp(p->id, id)==0){
			fprintf(stderr, "Error, customer with such id already exists\n");
			return(-1);
		}
	}
	for(p=d->name->array[key_name]; p!=NULL; p=p->next_name){
		if(strcmp(p->name, name)==0){
			fprintf(stderr, "Error, customer with such name already exists\n");
			return(-1);
		}
	}
	//create new user
	struct UserInfo *new=calloc(1, sizeof(struct UserInfo)); 
	new->id = strdup(id);
	new->name = strdup(name);
	new->purchase=purchase;
	//let member next point to
	//#the node of corresponding linked list
	new->next_id = d->id->array[key_id];
	new->prev_id = NULL;
	new->next_name = d->name->array[key_name];
	new->prev_name = NULL;
	
	//let node be first node of linked list
	d->id->array[key_id]=new;
	d->name->array[key_name]=new;
	
	if(d->id->array[key_id]->next_id != NULL){
		d->id->array[key_id]->next_id->prev_id = d->id->array[key_id];
		d->name->array[key_name]->next_id->prev_id = 
		                                     d->name->array[key_name];
	}
	d->numItems++;
  
  //if(d->numItems >= 0.75*(d->HashSizeID) && d->HashSizeID<=1048576)
  //  ExpandTable(d);

	return 0;  
}



/*-------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
	if (d==NULL || id==NULL){
		fprintf(stderr, "Error,invalid input argument\n");
		return (-1);
	}

	int key_id = hash_function(id, d->iBucketCount);

	//find customer with id
	struct UserInfo *id_target = FindCustomerNodeByID(d, id);

	if (id_target == NULL){
		fprintf(stderr,"Error, no customer with such id is found\n");
		return -1;
	}
	//find customer with name
	struct UserInfo *name_target= 
	                           FindCustomerNodeByName(d, id_target->name);
	
	//if target on the top of list - we should change top of the list
	if(d->id->array[key_id] == id_target){
		d->id->array[key_id] = id_target->next_id;
	}
	int key_name = hash_function(name_target->name, d->iBucketCount);
	if(d->name->array[key_name] == name_target){
		d->name->array[key_name] = name_target->next_name;
	}

	//make previious customer point to next one
	if (id_target->prev_id != NULL) {  
		id_target->prev_id->next_id = id_target->next_id;
		id_target->prev_name->next_name = id_target->next_name;
	}
	else if (id_target->next_id != NULL) {
		id_target->next_id->prev_id = id_target->prev_id;
		id_target->next_name->prev_name = id_target->prev_name;
	}

	//remove target
	free (id_target->id);
	free (id_target->name);
	free (id_target);
	
	d->numItems--;

	return 0;
}

/*-------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
	if (d==NULL || name==NULL){
		fprintf(stderr, "Error,invalid input argument\n");
		return (-1);
	}
	
	int key_name = hash_function(name, d->iBucketCount);

	//find customer with such name
	struct UserInfo *name_target = FindCustomerNodeByName(d, name);
	if (name_target==NULL){
		fprintf(stderr,"Error, no customer with such name is found\n");
		return -1;
	}
	//find customer with id
	struct UserInfo *id_target = FindCustomerNodeByID(d, name_target->id);
	
	//if target on the top of list - we should change top of the list
	if(d->name->array[key_name] == name_target){
		d->name->array[key_name] = name_target->next_name;
	}
	int key_id = hash_function(id_target->id, d->iBucketCount);
	if(d->id->array[key_id] == id_target){
		d->id->array[key_id] = id_target->next_id;
	}
	
	//make previious customer point to next one
	if (name_target->prev_name != NULL) {
		name_target->prev_id->next_id = name_target->next_id;
		name_target->prev_name->next_name = name_target->next_name;
	}
	else if (name_target->next_name != NULL) {
		name_target->next_id->prev_id = name_target->prev_id;
		name_target->next_name->prev_name = name_target->prev_name;
	}
	
	//remove target
	free (name_target->id);
	free (name_target->name);
	free (name_target);
	
	d->numItems--;

	return 0;
}

/*-------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
	if (d==NULL || id==NULL){
		fprintf(stderr, "Error,invalid input argument\n");
		return (-1);
	}

	//find customer with given id
	struct UserInfo *target = FindCustomerNodeByID(d, id);

	if (target==NULL){
		fprintf(stderr, "Error, no customer with such id is found\n");
		return -1;
	}
	return target->purchase;
}
/*-------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
	if (d==NULL || name==NULL){
		fprintf(stderr, "Error,invalid input argument\n");
		return (-1);
	}

	//find customer with given name
	struct UserInfo *target = FindCustomerNodeByName(d, name);

	if (target==NULL){
		fprintf(stderr,"Error, no customer with such name is found\n");
		return -1;
	}

	return target->purchase;
}
/*-------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  if (d==NULL || fp==NULL){
    fprintf(stderr, "Error,invalid input argument\n");
    return (-1);}
  unsigned int sum=0;
  struct UserInfo *p;
  
  for (int i = 0; i < d->iBucketCount; i++) {
    //go through each customer by name or and collect data
    for (p=d->name->array[i]; p!=NULL; p=p->next_name) {
      sum += fp(p->id, p->name, p->purchase);}
  }
  
  return sum;
}
