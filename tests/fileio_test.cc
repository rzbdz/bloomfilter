#include <gtest/gtest.h>
#include "utils/fileio.h"

TEST(FileIOTest, TestExistedFile) {
  // create a file in my own way
  FILE* f = fopen("test.dat", "wb");
  char buf[100];

  // write some garbage to it
  for (int i = 0; i < 100; i++) {
    buf[i] = i;
  }
  fwrite(buf, 1, 100, f);
  fclose(f);

  // test the fileio.h interface
  File file;
  OpenExistedFile("test.dat", &file, 100);
  // check the garbage data
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(file.data_[i], i);
    file.data_[i] = 100 - i;
  }

  // do something here
  file.data_[50] = 'a';

  ForceWriteBack(&file);

  CloseFile(&file);

  // test the file in my own way
  f = fopen("test.dat", "rb");
  fread(&buf, 1, 100, f);
  for (int i = 0; i < 100; i++) {
    if (i == 50)
      ASSERT_EQ(buf[i], 'a');
    else
      ASSERT_EQ(buf[i], 100 - i);
  }
  fclose(f);
  remove("test.dat");
}

typedef struct {
  uint32_t len;
  char zarray[0];
} ZARRAY;

TEST(FileIOTest, TestNewFile) {
  // in this test, you will learn
  // how we can use zero-length array to
  // use in memory data structure more
  // efficiently
  // (notice: it's not a standard usage of C,
  //  but it's supported by the gcc/g++ compiler)
  File myfile;
  OpenNewFile("test.dat", &myfile, 200);
  EXPECT_EQ(myfile.length_, 4096);
  ZARRAY* zr;
  zr = (ZARRAY*)myfile.data_;
  zr->len = 4000;
  for (int i = 0; i < 4000; i++) {
    zr->zarray[i] = i % (1 << 7);
  }
  ForceWriteBack(&myfile);
  CloseFile(&myfile);
  // make sure we won't access the invalid memory address
  memset(&myfile, 0, sizeof(File));
  FILE* file = fopen("test.dat", "rb");
  char* buf = (char*)malloc(4096);
  // skip the first uint32
  fseek(file, 4, SEEK_SET);
  int read_count = fread(buf, 1, 4096, file);
  EXPECT_EQ(read_count, 4092);
  for (int i = 0; i < 4000; i++) {
    ASSERT_EQ(buf[i], i % (1 << 7));
  }
  remove("test.dat");
}
