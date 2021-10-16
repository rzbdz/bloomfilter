#include "filter/bloom_filter.h"

#include "math.h"
#include "utils/hash.h"

#define MIN(a, b) ((a < b) ? (a) : (b))
#define MAX(a, b) ((a > b) ? (a) : (b))

#define PACK_2_POW(n) ()

typedef struct bloom_filter_* BloomFilterPtr;

// remember to uncomment static when you finish testing these 
// to functions. also do the same to the header file.
// static
uint optimal_hash_function_count(size_t n, size_t m) {
  return 0;
}

// static
size_t optimal_bitset_length(size_t n, double p, size_t disk_size) {
  // @see: wiki m = -nlnp/(ln2*ln2)
  return 0;
}

// static
BloomFilterPtr CreateBloomFilter(const char* name, uint k, size_t m) {
  return NULL;
}

BloomFilterPtr ReadBloomFilterFromFile(const char* name) {
  return NULL;
}

BloomFilterPtr NewBloomFilter(const char* name,
                              size_t expect_insertion,
                              double fpp,
                              size_t disk_size) {
  return NULL;
}

int Insert(BloomFilterPtr bf, const char* key, size_t len) {
  return -1;
}

int ContainsKey(BloomFilterPtr bf, const char* key, size_t len) {
  return -1;
}

int CloseBloomFilter(BloomFilterPtr bf) {
  return -1;
}