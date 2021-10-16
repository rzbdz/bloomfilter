#include "filter/bloom_filter.h"

#include "math.h"
#include "utils/hash.h"

#define MIN(a, b) ((a < b) ? (a) : (b))
#define MAX(a, b) ((a > b) ? (a) : (b))

#define PACK_2_POW(n) ()

typedef struct bloom_filter_* BloomFilterPtr;

// static
uint optimal_hash_function_count(size_t n, size_t m) {
  if ((size_t)n > m) {
    return PRIME_ARRAY_SIZE;
  }
  // @see: wiki k = (m / n) * ln(2)
  return MIN((uint)((double)m / (double)n * log(2) + 0.5), PRIME_ARRAY_SIZE);
}

// static
size_t optimal_bitset_length(size_t n, double p, size_t disk_size) {
  if (disk_size == 0) {
    return 0;
  }
  size_t max_m = MIN(disk_size * 8, UINT32_MAX);
  if (p == 0 || n >= -((size_t)INT32_MIN)) {
    return max_m;
  } else {
    // @see: wiki m = -nlnp/(ln2*ln2)
    // in cmath: ::log == ln, ::log10 == log, ::log2 == log_2
    // printf("max_m %lu \n", max_m);
    // printf("double: %lu \n",
    //        (size_t)((double)(-n) * log(p) / (log(2) * log(2))));
    size_t should_m = (size_t)(-((ssize_t)n) * log(p) / (log(2) * log(2)));
    return MIN(should_m, max_m);
  }
}

// static
BloomFilterPtr CreateBloomFilter(const char* name, uint k, size_t m) {
  File a_file;
  size_t biset_size = BitSetBufferSize(m);
  size_t total_size = biset_size + BLOOM_FILTER_HEADER_SZ;
  if (OpenNewFile(name, &a_file, total_size) == -1) {
    return NULL;
  }
  BloomFilterPtr bf = (BloomFilterPtr)a_file.data_;
  bf->inner_file_ = a_file;  // install the file
  bf->k_ = k;
  NewBitSet(m, (char*)(&bf->bits_));
  return bf;
}

BloomFilterPtr ReadBloomFilterFromFile(const char* name) {
  File a_file;
  if (OpenExistedFile(name, &a_file, 0) == -1) {
    return NULL;
  }
  BloomFilterPtr bf = (BloomFilterPtr)a_file.data_;
  bf->inner_file_ = a_file;
  return bf;
}

BloomFilterPtr NewBloomFilter(const char* name,
                              size_t expect_insertion,
                              double fpp,
                              size_t disk_size) {
  if (disk_size < 4096) {
    return NULL;
  }
  size_t m = optimal_bitset_length(expect_insertion, fpp, disk_size);
  uint k = (uint)optimal_hash_function_count(expect_insertion, m);
  return CreateBloomFilter(name, k, m);
}

static uint32_t get_hash(const char* record, size_t len, int ith, int cap) {
  return hash(record, len, prime_array[ith]) % cap;
}

int Insert(BloomFilterPtr bf, const char* key, size_t len) {
  for (uint i = 0; i < bf->k_; i++) {
    uint32_t bit_index = get_hash(key, len, i, bf->bits_.length_);
    if (SetBit(&bf->bits_, bit_index) == -1) {
      return -1;
    }
  }
  return 0;
}

int ContainsKey(BloomFilterPtr bf, const char* key, size_t len) {
  for (uint i = 0; i < bf->k_; i++) {
    uint32_t bit_index = get_hash(key, len, i, bf->bits_.length_);
    int flag;
    if ((flag = GetBit(&bf->bits_, bit_index)) != 1) {
      return 0;
    }
  }
  return 1;
}

int CloseBloomFilter(BloomFilterPtr bf) {
  ForceWriteBack(&bf->inner_file_);
  return CloseFile(&bf->inner_file_);
}