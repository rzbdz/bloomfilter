#include "utils/bitset.h"

size_t BitSetBufferSize(size_t length) {
  return ARRAY_LENGTH(length) * CELL_SIZE + sizeof(size_t);
}

BitSetPtr NewBitSet(size_t length, void* buffer) {
  return NULL;
}

BitSetPtr AllocateBitSet(size_t length) {
  return NULL;
}

int ReadBitSetFromBuffer(const void* buffer, BitSetPtr* set_ptr) {
  return -1;
}

int WriteBitSetToBuffer(char* buffer, BitSetPtr set) {
  if (buffer == NULL) {
    return -1;
  }
  return -1;
}

int GetBit(BitSetPtr const set, size_t index) {
  return -1;
}
int SetBit(BitSetPtr set, size_t index) {
  if (set == NULL) {
    return -1;
  }
  return -1;
}
int UnsetBit(BitSetPtr set, size_t index) {
  return -1;
}
