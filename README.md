# A simple on-disk Bloom Filter in C

![Author](https://img.shields.io/badge/Author-rzbdz-blue.svg "Author")
[![LICENSE](https://img.shields.io/badge/license-BSD-green)](./LICENSE)

Toy level code for learning purpose.

Use mmap to map file to in memory structure. Only support linux-x64-gcc by now.

It's also been written as a **programing lab** with detailed instructions for learner by me, you can switch the branch to lab branch to see details or click the button below.

[![lab_en](https://img.shields.io/badge/lab-en-red.svg)](./lab.en.md)
[![lab_zh](https://img.shields.io/badge/lab-zh-yellow.svg)](./lab.zh.md)


download and install:

```bash
$ git clone .....
$ sudo ./build_support/packages.sh
$ cd bloom_filter
$ mkdir build
$ cd build
$ cmake ..
```

run tests in build/:
```bash
$ make tests       # Compile all tests (3 sets of tests in total)
$ make bitset_test # Compile the bit test, which runs bitset_test.cc
$ make fileio_test # Same as above
$ make filter_test # the same
```
