#pragma once
#include <stdint.h>
#include <stddef.h>

#define PRIME_MAX 32

/**
 * @brief provided hash function steal from levelDB source code.
 * generate 32 bits hash code, which is capable for
 * a 4,294,967,296 bits bloom filter.
 * (4GB, at least 1 hundred million records if m/n == 42)
 *
 * @param data the c style string.
 * @param n the length of the string.
 * @param seed the initial seed for hashing.
 * @return uint32_t a 32bit hash code.
 */
uint32_t hash(const char* data, size_t n, uint32_t seed);

#define PRIME_ARRAY_SIZE 32
/**
 * @brief a prime_array contains 32 prime numbers for generate different hash
 * codes (as seed). it's fine when you didn't use them all.
 */
uint32_t prime_array[]{72251, 72253, 72269, 72271, 72277, 72287, 72307, 72313,
                       72337, 72341, 72353, 72367, 72379, 72383, 72421, 72431,
                       72461, 72467, 72469, 72481, 72493, 72497, 72503, 72533,
                       72547, 72551, 72559, 72577, 72613, 72617, 72623, 72643};
