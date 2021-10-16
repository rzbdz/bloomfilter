#include <gtest/gtest.h>
#include "utils/bitset.h"

void test_api(BitSetPtr bits) {
  for (int i = 0; i < 1000; i++) {
    EXPECT_EQ(GetBit(bits, i), 0);
  }
  EXPECT_EQ(SetBit(bits, 0), 0);
  EXPECT_EQ(SetBit(bits, 2), 0);
  EXPECT_EQ(SetBit(bits, 4), 0);
  EXPECT_EQ(SetBit(bits, 100), 0);
  EXPECT_EQ(SetBit(bits, 999), 0);
  EXPECT_EQ(GetBit(bits, 0), 1);
  EXPECT_EQ(GetBit(bits, 2), 1);
  EXPECT_EQ(GetBit(bits, 4), 1);
  EXPECT_EQ(GetBit(bits, 100), 1);
  EXPECT_EQ(GetBit(bits, 999), 1);
}

TEST(BitSetTest, DISABLED_TestNew) {
  char* buffer = (char*)malloc(BitSetBufferSize(1000));
  BitSetPtr bits = NewBitSet(1000, buffer);
  EXPECT_EQ(bits->length_, 1000);
  test_api(bits);
  free(buffer);
}
TEST(BitSetTest, DISABLED_TestAllocate) {
  BitSetPtr bits = AllocateBitSet(1000);
  EXPECT_EQ(bits->length_, 1000);
  test_api(bits);
  free(bits);
}

TEST(BitSetTest, DISABLED_TestBufferSize) {
  int s = LEN_FIELD_SIZE;
  int size = BitSetBufferSize(31);
  EXPECT_EQ(size, 4 + s);
  size = BitSetBufferSize(32);
  EXPECT_EQ(size, 4 + s);
  size = BitSetBufferSize(99);
  EXPECT_EQ(size, 4 * 4 + s);
  size = BitSetBufferSize(180);
  EXPECT_EQ(size, 4 * 6 + s);
  size = BitSetBufferSize(878798);
  EXPECT_EQ(size, 4 * 27463 + s);
}

TEST(BitSetTest, DISABLED_TestSet1) {
  int size = BitSetBufferSize(31);
  char* buffer = (char*)malloc(size);
  BitSetPtr bits = NewBitSet(31, buffer);
  EXPECT_EQ(bits->length_, 31);
  for (int i = 0; i < 32; i++) {
    if (i & 1) {
      SetBit(bits, i);
    }
  }
  // didn't test the big-endian/litte-endian
  uint32_t* myint = (uint32_t*)(buffer + sizeof(size_t));
  EXPECT_EQ(*myint, 0xaaaaaaaa);
  free(buffer);
}
TEST(BitSetTest, DISABLED_TestSet2) {
  int size = BitSetBufferSize(1000);
  char* buffer = (char*)malloc(size);
  BitSetPtr bits = NewBitSet(1000, buffer);
  EXPECT_EQ(bits->length_, 1000);
  for (int i = 0; i < 1000; i++) {
    if (i & 1) {
      SetBit(bits, i);
    }
  }
  // due to the little endian problem, I don't know how to verify it...
  for (int i = 0; i < 1000; i++) {
    if (i & 1) {
      EXPECT_EQ(GetBit(bits, i), 1);
    } else {
      EXPECT_EQ(GetBit(bits, i), 0);
    }
  }
  free(buffer);
}
TEST(BitSetTest, DISABLED_TestReadFrom) {
  int size = BitSetBufferSize(1000);
  char* buffer = (char*)malloc(size);
  memset(buffer, 0x55, size);
  *((size_t*)buffer) = 1000;
  BitSetPtr bits;
  ReadBitSetFromBuffer(buffer, &bits);
  EXPECT_EQ(bits->length_, 1000);
  for (int i = 0; i < 1000; i++) {
    if (i & 1) {
      EXPECT_EQ(GetBit(bits, i), 0);
    } else {
      EXPECT_EQ(GetBit(bits, i), 1);
    }
  }
  free(buffer);
}