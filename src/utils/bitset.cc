#include "utils/bitset.h"

size_t BitSetBufferSize(size_t length) {
  return ARRAY_LENGTH(length) * CELL_SIZE + sizeof(size_t);
}

BitSetPtr NewBitSet(size_t length, void* buffer) {
  size_t size = BitSetBufferSize(length);
  memset(buffer, 0, size);
  BitSetPtr p = (BitSetPtr)buffer;
  p->length_ = length;
  return p;
}

BitSetPtr AllocateBitSet(size_t length) {
  size_t size = BitSetBufferSize(length);
  BitSetPtr p = (BitSetPtr)malloc(size);
  memset(p, 0, size);
  p->length_ = length;
  return p;
}

int ReadBitSetFromBuffer(const void* buffer, BitSetPtr* set_ptr) {
  if (buffer == NULL) {
    return -1;
  }
  *set_ptr = (BitSetPtr)buffer;
  return 0;
}

int WriteBitSetToBuffer(char* buffer, BitSetPtr set) {
  if (buffer == NULL) {
    return -1;
  }
  uint32_t* buf = (uint32_t*)buffer;
  for (size_t i = 0; i < set->length_ / 32; i++) {
    buf[i] = set->data_[i];
  }
  return 0;
}

static inline int down(int a) {
  return a == 0 ? 0 : 1;
}

int GetBit(BitSetPtr const set, size_t index) {
  return set == NULL ? -1 : down(set->data_[index / 32] & (1 << (index & 31)));
}
int SetBit(BitSetPtr set, size_t index) {
  if (set == NULL) {
    return -1;
  }
  int ret = down(set->data_[index / 32] & (1 << (index & 31)));
  set->data_[index / 32] |= (1 << (index & 31));
  return ret;
}
int UnsetBit(BitSetPtr set, size_t index) {
  if (set == NULL) {
    return -1;
  }
  int ret = down(set->data_[index / 32] & (1 << (index & 31)));
  set->data_[index / 32] &= ~(1 << (index & 31));
  return ret;
}
