/// The mathematics analysis to choose k and m/n
/// is available in wikipedia
/// https://en.wikipedia.org/wiki/Bloom_filter

#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "utils/bitset.h"
#include "utils/fileio.h"

struct bloom_filter_ {
  // length of the bit string
  // determine which length_(m in the formular) and
  // k_ (k) to use is a trade off btw Spatial and Time.
  // how many hash functions are used
  File inner_file_;
  uint k_;
  struct bitset_ bits_;
};

#define BLOOM_FILTER_SIZE (sizeof(struct bloom_filter_))
#define BLOOM_FILTER_HEADER_SZ (sizeof(struct bloom_filter_) - LEN_FIELD_SIZE)

typedef struct bloom_filter_* BloomFilterPtr;

uint optimal_hash_function_count(size_t n, size_t m);
size_t optimal_bitset_length(size_t n, double p, size_t disk_size);
BloomFilterPtr CreateBloomFilter(const char* name, uint k, size_t m);

/**
 * @brief as name
 *
 * @param name basically use for the file name
 * @param expect_insertion to calculate the length of a bf
 * @param fpp false positive possiblity, lower is better, but larger comsumption
 *            on space.
 * @param disk_size the maximun space size, must specific one, maximum value
 *                  should be 4GB.
 * @return BloomFilterPtr
 */
BloomFilterPtr NewBloomFilter(const char* name,
                              size_t expect_insertion,
                              double fpp,
                              size_t disk_size);

/**
 * @brief open an existed bloom filter
 *
 * @param name the file name
 * @return BloomFilterPtr
 */
BloomFilterPtr ReadBloomFilterFromFile(const char* name);

/**
 * @brief as named
 *
 * @param bf
 * @param key
 * @param len
 * @return int , 0 if success -1 if fails
 */
int Insert(BloomFilterPtr bf, const char* key, size_t len);

/**
 * @brief as named
 *
 * @param bf
 * @param key
 * @param len
 * @return int , -1 if fails, 1 if yes, 0 if no
 */
int ContainsKey(BloomFilterPtr bf, const char* key, size_t len);

/**
 * @brief for user to install a signal handler
 *        to make sure file is durable after
 *        events like 'Ctrl+C' occurs.
 *
 * @param bf
 * @return int
 */
int CloseBloomFilter(BloomFilterPtr bf);