#include <gtest/gtest.h>
#include <fstream>
#include "filter/bloom_filter.h"

TEST(FilterTest, DISABLED_TestOptimal) {
  // figure it out how to test...
  EXPECT_EQ(optimal_bitset_length(20, 0.3678, UINT32_MAX), 41UL);
  EXPECT_EQ(optimal_bitset_length(20, 0, UINT32_MAX), UINT32_MAX);
  EXPECT_EQ(optimal_hash_function_count(200, 400), (int)(2 * 0.7));
  EXPECT_EQ(optimal_hash_function_count(200, 4000), (int)(20 * 0.7));
}

TEST(FilterTest, DISABLED_TestCreate) {
  BloomFilterPtr p = CreateBloomFilter("test.dat", 20, 1024);
  EXPECT_EQ(p->bits_.length_, 1024);
  for (int i = 0; i < 1024; i++) {
    ASSERT_EQ(GetBit(&p->bits_, i), 0);
    if (i & 1)
      SetBit(&p->bits_, i);
  }
  CloseBloomFilter(p);
  p = ReadBloomFilterFromFile("test.dat");
  EXPECT_EQ(p->k_, 20);
  EXPECT_EQ(p->bits_.length_, 1024);
  for (int i = 0; i < 1024; i++) {
    if (i & 1)
      ASSERT_EQ(GetBit(&p->bits_, i), 1);
    else
      ASSERT_EQ(GetBit(&p->bits_, i), 0);
  }
  CloseBloomFilter(p);
}

TEST(FilterTest, DISABLED_TestInsert) {
  BloomFilterPtr p = CreateBloomFilter(
      "test.dat", (uint)optimal_hash_function_count(724, 15 * 724), 15 * 724);
  ASSERT_EQ(p->bits_.length_, 15 * 724);
  for (int i = 0; i < 1024; i++) {
    ASSERT_EQ(GetBit(&p->bits_, i), 0);
  }
  std::ifstream file("../tests/724urls.txt", std::fstream::in);
  std::string url;
  while (std::getline(file, url)) {
    ASSERT_EQ(Insert(p, url.c_str(), url.length()), 0);
  }
  file.seekg(0L, std::ios::beg);
  while (std::getline(file, url)) {
    ASSERT_EQ(ContainsKey(p, url.c_str(), url.length()), 1);
  }
  file.close();
  std::ifstream another_file("../tests/286urls.txt", std::fstream::in);
  int count = 0;
  int hit_count = 0;
  while (std::getline(another_file, url)) {
    count++;
    int r = ContainsKey(p, url.c_str(), url.length());
    ASSERT_NE(r, -1);
    if (r == 0) {
      hit_count++;
    }
  }
  another_file.close();
  double correct_rate = (double)hit_count / (double)count;
  EXPECT_NEAR(correct_rate, 1.0, 0.05);
  // test integrity
  CloseBloomFilter(p);
  p = ReadBloomFilterFromFile("test.dat");
  std::ifstream file1("../tests/724urls.txt", std::fstream::in);
  while (std::getline(file1, url)) {
    ASSERT_EQ(ContainsKey(p, url.c_str(), url.length()), 1);
  }
  file.close();
  std::ifstream another_file1("../tests/286urls.txt", std::fstream::in);
  count = 0;
  hit_count = 0;
  while (std::getline(another_file1, url)) {
    count++;
    int r = ContainsKey(p, url.c_str(), url.length());
    ASSERT_NE(r, -1);
    if (r == 0) {
      hit_count++;
    }
  }
  another_file.close();
  EXPECT_NEAR((double)hit_count / (double)count, correct_rate, 0.00001);
  CloseBloomFilter(p);
  remove("test.dat");
}

static char base62[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

void test_scale_opt(const char* name, size_t scale) {
  auto p = NewBloomFilter(name, scale, 0.01, UINT32_MAX);
  char buf[20] = "dwz.xyz/";
  buf[13] = 0;
  size_t s;
  size_t inserted_count = scale;
  for (s = 0; s < inserted_count; s++) {
    size_t convert = s;
    size_t i = 8;
    while (convert) {
      buf[i++] = base62[convert % 62];
      convert /= 62;
    }
    Insert(p, buf, i);
  }
  size_t correct_judge = 0;
  for (; s < inserted_count * 2; s++) {
    size_t convert = s;
    size_t i = 8;
    while (convert) {
      buf[i++] = base62[convert % 62];
      convert /= 62;
    }
    if (ContainsKey(p, buf, i) == 0) {
      correct_judge++;
    }
  }
  EXPECT_NEAR((double)correct_judge / (double)inserted_count, 1, 0.05);
  CloseBloomFilter(p);
}

void test_scale(size_t scale) {
  test_scale_opt("test.dat", scale);
  remove("test.dat");
}

TEST(FilterTest, DISABLED_TestScale2M) {
  auto scale = INT32_MAX / 1000UL;
  const char* nm = "test.bff";
  test_scale_opt(nm, scale);
  auto p = ReadBloomFilterFromFile(nm);
  char buf[20] = "dwz.xyz/";
  buf[13] = 0;
  size_t s;
  size_t inserted_count = scale, correct_judge = 0;
  for (; s < inserted_count * 2; s++) {
    size_t convert = s;
    size_t i = 8;
    while (convert) {
      buf[i++] = base62[convert % 62];
      convert /= 62;
    }
    if (ContainsKey(p, buf, i) == 0) {
      correct_judge++;
    }
  }
  EXPECT_NEAR((double)correct_judge / (double)inserted_count, 1, 0.05);
  remove(nm);
}

TEST(FilterTest, DISABLED_TestScale20M) {
  test_scale(INT32_MAX / 100UL);
}

// this will be extremely slow, don't enable it
// until you pass all tests above.
TEST(FilterTest, DISABLED_TestScale200M) {
  test_scale(INT32_MAX / 10UL);
}
