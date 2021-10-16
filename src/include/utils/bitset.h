#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define CELL_SIZE (sizeof(uint32_t))
#define ARRAY_LENGTH(target) (((target) + 31) / 32)
#define LEN_FIELD_SIZE (sizeof(size_t))

struct bitset_ {
  size_t length_;
  uint32_t data_[0];
};

typedef struct bitset_* BitSetPtr;

/**
 * @brief calculate size in bytes
 * expands to "ARRAY_LENGTH(length) * CELL_SIZE + sizeof(size_t)""
 * @param length the bit string length in bits
 * @return size_t return how many bytes your need to create for a bitset
 */
size_t BitSetBufferSize(size_t length);

/**
 * @brief allocate a memory and set all bits to zero
 * caller should free it after use. If use mmap, pass the buffer into
 * this function.
 *
 * @param length the length of the bit string.
 * @return BitSetPtr, a pointer to struct bitset_ whose bits are all 0s.
 */
BitSetPtr NewBitSet(size_t length, void* buffer);

/**
 * @brief allocate a memory and set all bits to zero
 * caller should free it after use.
 *
 * @param length the length of the bit string.
 * @return BitSetPtr, a pointer to struct bitset_ whose bits are all 0s.
 */
BitSetPtr AllocateBitSet(size_t length);

/**
 * @brief read a bit set from file, it should use
 * malloc to allocate memory and then correctly read binary data into
 * struct.
 *
 * @param buffer
 * @param set_ptr when space for set is malloced, it should return
 * the set pointer(a.k.a BitSetPtr) to the struct bitset_** set
 * @return int, return -1 if I/O errors, return 0 if success
 */
int ReadBitSetFromBuffer(const void* buffer, BitSetPtr* set_ptr);

/**
 * @brief as name, if use mmap and keep the mapped region in
 * memory, there is no need to call this function.
 *
 * @param buffer
 * @param set
 * @return int, return 0 if success, -1 if fails
 */
int WriteBitSetToBuffer(const char* buffer, BitSetPtr set);

/**
 * @brief check specific bit in the bit set.
 *
 * @param set
 * @param index
 * @return int -1 if fails
 */
int GetBit(BitSetPtr const set, size_t index);

/**
 * @brief Set a bit in a bit set
 *
 * @param set the bit set.
 * @param index the bit index.
 * @return int, the bit value before set by this function. -1 if fails
 */
int SetBit(BitSetPtr set, size_t index);

/**
 * @brief UnSet a bit in a bit set
 *
 * @param set the bit set.
 * @param index the bit index.
 * @return int, the bit value before set by this function. -1 if fails
 */
int UnsetBit(BitSetPtr set, size_t index);