#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "murmurhash.h"
#include "countable_bloomfilter.h"

#define ___BLANK___ 0

/**
 * Structure of struct BloomFilter
 *
 * arr : must point to the first element of Bloomfilter Array
 * size : must be equal to the length of Bloomfilter Array
 * 
 * +-----------------------------------+
 * |         struct BloomFilter        |
 * +-----------+-----------------------+
 * |    size   |          *arr         |
 * +--+--+--+--+--+--+--+--+--+--+--+--+
 * |  |  |  |  |  |  |  |  |  |  |  |  |
 * +--+--+--+--+--+--+--+--+--+--+--+--+
 *
 * +-----------------------------------+
 * |               Array               |
 * +---+---+---+---+---+---+-----+-----+
 * | 0 | 1 | 2 | 3 | 4 | 5 | ... | N-1 |
 * +---+---+---+---+---+---+-----+-----+
 * |   |   |   |   |   |   |     |     |
 * +---+---+---+---+---+---+-----+-----+
 *
 * */

/**
 * Allocate a bloom filter whose array size is 'size'. Returns the pointer of new BloomFilter structure.
 */
struct BloomFilter* bloomfilter_create (unsigned int size, unsigned num_hash) {
  struct BloomFilter* bf;
  char *array;
  if(!size) {
    fprintf(stderr, "Size of bloom filter must be larger than 0!\n");
    return NULL;
  }

  /* Fill in the Blank */
  /* ============================== */
  bf = calloc(1,    sizeof(struct BloomFilter));
  array = (char *)calloc(1, size*sizeof(unsigned int));
  assert(bf);
  assert(array);
 
  bf->arr = (unsigned int *) (array);
  //bf->arr = (unsigned int *) calloc(1, size*sizeof(unsigned int));
  bf->size = size;
  bf->num_hash = num_hash;
  return (bf);
  /* ============================== */
}

/**
 * Destroy a bloom filter.
 */
void bloomfilter_destroy (struct BloomFilter *bf) {
  assert(bf);
  free(bf->arr);
  free(bf);
}

/**
 * Add a key into the bloom filter.
 */
void bloomfilter_add (struct BloomFilter *bf, const char *key) {
  assert(bf && bf->arr);
  if (!key) {
    fprintf(stderr, "String must not be NULL!\n");
    return;
  }

  uint32_t hash1 = murmurhash(key, (uint32_t) strlen(key), 100);
  uint32_t hash2 = murmurhash(key, (uint32_t) strlen(key), 200);
  // uint32_t hash3 = murmurhash(key, (uint32_t) strlen(key), 300);
  /* Fill in the Blank */
  /* Hint: bf->arr[ .... ] ... */  
  /* ============================== */
  uint32_t id1 = hash1 % bf -> size;
  uint32_t id2 = hash2 % bf -> size; 
  assert(id1 < bf -> size);
  assert(id2 < bf -> size);
  bf -> arr[id1] +=1;
  bf -> arr[id2] +=1;  
  
  /* ============================== */

}

/**
 * Remove a key from the bloom filter. Returns 0 if failed to remove. Returns 1 if successfully removed.
 */
int bloomfilter_remove (struct BloomFilter *bf, const char *key) {
  assert(bf && bf->arr);
  if(!bloomfilter_check(bf, key)) {
    return 0;
  }

  uint32_t hash1 = murmurhash(key, (uint32_t) strlen(key), 100);
  uint32_t hash2 = murmurhash(key, (uint32_t) strlen(key), 200);
  // uint32_t hash3 = murmurhash(key, (uint32_t) strlen(key), 300);
  /* Fill in the Blank */
  /* Hint: bf->arr[ .... ] ... */  
  /* ============================== */
  uint32_t id1 = hash1 % bf -> size;
  uint32_t id2 = hash2 % bf -> size; 
  assert(bf -> arr[id1] > 0);
  assert(bf -> arr[id2] > 0);
  bf -> arr[id1] -=1;
  bf -> arr[id2] -=1;  
  
  /* ============================== */

  return 1; 
}

/**
 * Returns the possible number of keys inside the bloom filter. If not exist, return 0. 
 */
int bloomfilter_check (struct BloomFilter *bf, const char *key) {
  assert(bf && bf->arr);
  
  uint32_t hash1 = murmurhash(key, (uint32_t) strlen(key), 100);
  uint32_t hash2 = murmurhash(key, (uint32_t) strlen(key), 200);
  // uint32_t hash3 = murmurhash(key, (uint32_t) strlen(key), 200);
  /* Fill in the Blank */
  /* ============================== */
  uint32_t num1 = bf->arr[hash1 % bf -> size];
  uint32_t num2 = bf->arr[hash2 % bf -> size];
//  uint32_t num3 = ___BLANK___;
 uint32_t min_num = num1 < num2 ? num1 : num2; // not total, but minimum to see if it exists
  
  return ( min_num );
  /* ============================== */

}
