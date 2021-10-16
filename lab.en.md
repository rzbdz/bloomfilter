# Lab: Bloom Filter


## Overview

This is a simple C language learning programming exercise that can be completed in a few hours. Readers will learn to write a Bloom Filter (Bloom Filter), who is widely used in current network applications. This article assumes that the reader is **zero knowledge of network programming**.

Participants should have good basic knowledge of C language programming, including but not limited to the following knowledge points:

- Basic programming concept (branch, loop, etc.)
- Integer data type
- Bit operation
- Structure
- Pointer
- Memory allocation on the heap (`malloc` and `free`)

The following will provide enough necessary pre-knowledge (information retrieval can be performed on the Internet where it is not clear) about Bloom filters to help complete the exercise. All the boldfaced words in this article are important knowledge points in the computer industry.

basic requirements:

- Complete this project on Linux x64 system. (If it is not an Ubuntu release, you need to install the compilation toolchain yourself instead of directly running the script installation provided by us).
- Implement the functions of the bitset source code file in the utils subdirectory. The function prototype has been given in the header files, and hash function has been provided (feel free to replace with your own).
- Use the function interface provided by the module in utils to call the Bloom filter in the filter subdirectory.
- Add custom tests to the corresponding test source code files (help are available in the source code), more details: [gtest](https://google.github.io/googletest/primer.html)
- Pass all non you-defined tests.

Expansion:

- +Limit the number of Bloom filter bits to an integer power of 2 to optimize the hash modulus operation as `a% capacity` => `a & (capacity-1)`. If you don’t know how to do it, you can find the answer in [Rounding up to next power of 2](https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2).
- +Read the paper [Less Hashing, Same Performance:
Building a Better Bloom Filter](https://www.eecs.harvard.edu/~michaelm/postscripts/rsa2008.pdf) in the two hash functions to implement multiple hash functions, and modify the Bloom filter. (You don’t actually need to read it, just use the formula)
- +Expand the number of hash functions on the basis of 2, optimize the construction strategy of the Bloom filter, and improve the selection strategy of k under the given expected false positive rate and limited available space (when the available space is limited, m will be better than actual The optimal solution of the formula is too small, if you use m into the formula to calculate k, it will cause fpp to increase).
- *Use sth like mutex to rewrite the Bloom filter to be thread-safe. (No requirement)

The following are all the prerequisite knowledge required to complete this assignment.

---
## Basic bit operations

For a `uint32_t` (defined in the header file in C99
`stdint.h`), it is a 32-bit wide integer type, which means it has 4 bytes.

For a `uint32_t`, if you need to convert four characters (`char`, one byte width) into a **32-bit integer**, there are two options, one is **Big Endian** One is **Little Endian**. However, this exercise can actually be regarded as not involving big endian and little endian.

If we need to manipulate the third bit of a 32-bit integer (numbered from low to high 0~31), we can use `num & (1<<3)` **select**, use `num |= (1 << 3) `**Set bit**.

---

## Hash table

For computers, memory is **random access** friendly.

Compared with **sequential search**, a faster method is to use a hash table. We use a certain function (hash function) to establish a match between **key** (the basis for search) and the position of the value. The **mapping relationship** between the two, you only need to calculate the hash code of the key once to locate the data we need. It is assumed here that the ideal hash function has no **collision**, but there will actually be some. This exercise will not involve the problem of hash conflicts since bloom filter is not complete accurate.

Imagine an array with a length of `10,000`, suppose we have `5,000` data (the simple reason is that they are integers), whether they are stored out of order or in order in the array, if you want to know that a specific number is no longer in the array In, you need to scan the entire array to know (you can use **binary search** to speed up when ordered).

Using hash table storage, we first use a hash function to map each inserted integer to a certain value. For example, for inserting `250`, `hash(250) = 123456789`, so we only need to put `250` in The `123456789` position of the array. Just like this example, for the hash function, the calculated value does not necessarily fit the range of the array (here is `10,000`), it is necessary to **truncate** the excessively large by the **modulo** operation Value, in order to avoid conflicts, we always want to use **prime number** as the modulus, because when different numbers can be divisible by the same number, and the common factor is also a composite factor, it is easy to modulate the same As a result, it is difficult for prime numbers to conflict with only two factors.

---
## Basic network application structure

To learn computers, you must understand what the actual practitioners are working on.

It is best to look at each module from a **systematic point of view** before learning each part of the professional course, to understand which part of the system is in the whole system, and to know which part of the learning is in practical application.  When it comes to specific subdomain, use the idea of ​​**layered isolation** to learn the details.

First of all, the question of what is being solved in network programming. We know that there is a network, and there are various nodes on the network, such as clients such as computers, phones and server provided by various network service providers. Their communications are just **transmitting various data**.

So programming is either to build software on a computer or mobile phone (**client development**) or web pages (**Web front-end development**), or to build various services running on the server (**server back-end development**) ).

The main thing is to learn how to make various services running on the **server** so as to be well qualified to do **back-end development**. .

The server provides **service** responding to the request, and the client makes **request**. The standard network service is a simple **C/S structure**, the client-side server structure. In order to make the Bloom filter clear, the following content will only analyze the server architechure, that is, the website architechure.

The architecture of a website is the simplest at the beginning, which is **a server**, the client sends a request to this server through the network through some methods (please **abstract** this process), and the server gets the request After doing some calculations, it returns a result to the client.

For example, a client who can't calculate addition sends 1 and 1 to the server, and the server calculates what 1 + 1 is equal to, so he will return 2 to the client.

So our simplest program is to write a program that computes 1 + 1, and then write a loop. To facilitate understanding, if we have some abstract variables (they may actually be files), we can imagine the following **psudo code**:

```python
variables: has_request, client, server, a, b, sum

def server:
  while(True):
    if(has_request == True):
     sum = a + b
     has_request = False

def client_1:
  a = 1
  b = 1
  has_request = True
  print sum

def client_2:
  a = 2
  b = 3
  has_request = True
  print sum
```
Next, let’s make a little leap (like from `1 + 1 = 2` to calculus exercises). In fact, we don’t write a program that helps mathematical geniuses do additions on the server. What we do is far more complicated than this.

Imagine that we need to build a website that buys and sells population. What are we going to do? Analyze from the basic visit:

- Provide registration and login functions to record buyer and seller information, such as address and phone number.
- We need to store personal information of buyers and sellers, and it cannot be stored in memory (such as some arrays or linked lists), because the data in the memory is completely gone after the server is shut down. What we need is called **persistence**.

- We need to provide the function of querying buyer and seller information.
- We want to register the demographic information for the sale. For example, buyer A wants to buy the total population information of the United States in 2021 from a US buyer. We need to let buyer A search for this information to place an order .
- There are a lot more than these.

In short, these are here because we are going to introduce the concept of **persistence** and the concept of **data query**.

If you want to implement what I said about persistence, open a file with `fopen`, then take a pointer to my structure array, and then call the `write` function to write. If you want to read into the memory, it's just some `malloc` jobs, and then call `read`, read it. Simple!

But in fact, this approuch is not good enough. The first one is that there is no compatibility. Different software calls `write` and `read` in different ways, so services cannot be easily migrated. For example, website providers use Java, but he lose his company, and sell it to company B, but company B uses C++. They have to find someone to study how the original Java code reads and writes data, and then translate it.

And it does not conform to the idea of ​​abstract layering. In addition, we have to deal with various strange situations.
 
Let me explain the first situation. If the server is suddenly powered off, how to ensure that the program does not destroy the data in the hard disk? Can't read (Imagine a power failure in the middle of a `write`, maybe a little bit of data has been written to the disk).

For various reasons, people naturally think of making a standardized software **infrastructure** (it should be noted that the actual back-end development is also divided into infrastructure dev and work on bussiness. Here we are mainly explained for business purposes), who provide data storage and query functionalities, the most famous of which is the **relational database** SQL. 

Based on the idea of ​​calling abstract APIs and not caring about the underlying implementation, we only need to know that the database is a standard thing that provides data management interfaces, and it is often called **DBMS**, database management system.

Next, please kepp doing thought experiments like the above analysis of why we need to do a database to figure out what problems these things are solving. The huge Internet What the industry usually does requires so much manpower and material resources, and What repetitive or ~~original~~ work should be done to learn a computer science? But the thought experiment is not always completely correct, but we only need some self-consistent logical thing for the basic understanding.

---
## What is a cache

Here is a recommended easy reading book "[Code](https://book.douban.com/subject/4822685/)" to understand how computers are built. A must-read for programmers belongs to yes. Use this to build before the principles of system composition. A logical rough image is helpful.

Caching is a particularly important method of thinking in modern computers, and it also belongs to layered thinking.

First of all, let me mention the cache in the CPU. The most basic gate circuit is to store a bit. Its access speed is definitely similar to the speed of light, but we can’t make a 1TB CPU. Specifically, we can’t even make a 32MB CPU. The arithmetic unit (ALU) is the circuit that completes the calculation. These circuits cannot support a lot of data (registers), because there are many lines to be laid out for every 32-digit addition. Therefore, the compromise solution is to read some from the memory and load it into the register every time the operation is performed, and then perform the calculation.

But the memory is too slow. Here is a copy of [Latency Numbers Every Programmer Should Know] (https://gist.github.com/jboner/2841832) which was published ten years ago. Times.

The CPU cache (made by SRAM) is a compromise between the memory (made by DRAM) and the registers (SRAM) in the CPU. It is built inside the CPU, but not directly connected to the ALU (that is, there is no need to lay a bunch of lines for calculations). ), when the CPU wants to read data, it always reads the cached data first. If the cache is not available, drag the data in the memory to the cache and then read the cache.

The hard disk is slower, which is why you must first read the hard disk to the memory, to the CPU cache, and then to the ALU in the CPU. This hierarchical storage structure uses the principle of locality. That is, the total outermost CPU cache L3 cache is only 32MB. Why can the program run fast? It is because of the time slice of each program running (usually a few hundred CPU clock cycles, who is 1 divided by the CPU master? Frequency) generally does not involve too large a range of storage access (refers to the manipulated data structure and running code, a few KB can already accommodate a large amount of code).

That's it for the idea of ​​caching. Regarding the above `latency.txt`, please pay attention to the last one. It takes 150ms to send a network packet from California to the Netherlands and then back to California, which is extremely large compared to the L1 Cache access speed of 0.5 nanoseconds.

---
## Database separation site structure

Earlier we talked about the database, now we continue to jump, and now we upgrade our website architecture to multiple servers, so why do we do this? There are many reasons. The thought experiment just agrees with the following.

With multiple servers, we can respond when hundreds of millions (**high concurrency**) users want to buy and sell US 2021 population information, because we have a lot of hardware.

Then we may have to separate our database and service. Originally, there is only one server. The service that responds to the visits of the trading population webpage and the database that saves various demographic information and buyer and seller information are all on the same server, which is very stressful. . Consider that the machine where the server is located needs to handle a large number of requests, consumes a lot of memory and takes up a lot of resources, and the database also takes up a lot of resources, so that the database and the application server must be separated to avoid the machine being unable to resist.

Now we may have many servers, so we can even take the database to other servers separately. This kind of consideration also guarantees security. The application server (trading population website) does not necessarily hang the database. Now we have many application servers. One can be added immediately after one is installed. This is called **High Availability** .

Of course, we can be more radical. We also make hundreds of database servers, which will be replaced immediately.

With so many servers, how to ensure that their content is the same, such as a database, if the content is not the same, it will be troublesome. This is called **consistency**. These three boldfaces are the main research content of back-end infrastructure (infrastructure) development. We will not go further here.

The basic thing for back-end programmers to do is to complete business development by calling various APIs provided by the infrastructure, including writing various database transactions to add, delete and modify query statements, and use various frameworks to complete tasks.

The framework basically does all the dirty work (infrastructure development mentioned above), so that the upper-level programmers in charge of the business can focus on the business logic, such as calculating 1 + 1, you only need to write it in the `main` function , Don’t worry about how the program is scheduled to be executed by the operating system (why does the `main` function have to return 0`? It is actually some convention of the CRT!), you need to use `argc` and `argv` to also pass in, yourself Don't care how he got in.

But this also brings some burdens. The programmer must learn one more thing, that is, the convention of the `main` function. If you don't use the `main` function or do dirty work, the program cannot run.

The framework is something like the `main` function. It does all sorts of dirty work at the bottom of the network (starting programs outside `main`), but you must follow his rules.

Here are a few more sentences to talk about the difference between a library and a framework. A library is a logic written by others, and some operations are encapsulated. For example, `printf` is a library function and an API. You give him input and he gives you the result. , And the framework is something like the `main` function. It is a scaffolding. You only need to cloze and do the most critical things, and it will run it for you instead of writing a complete article.

These contents are very important. And learning network programming, a lot of time to learn how to use the framework (although it is a bit turning the cart before the horse, but if you don't use the framework, it will be even more turning the cart before the cart) and the rules of the framework, and then to the business logic code. The business logic code is just a basic skill, and whether or not to use tools is the fundamental difference between humans and animals.

---
## Database cache

I have talked about so much before, but in fact, it is to talk about Bloom filters. Before that, let's talk about server caching. After reading this, you already have the knowledge of caching and the main content of the current basic website architecture.

So make a bigger jump next.

For the database server, we mentioned above how slow the network is, so we don’t even check the database as a last resort.

The most basic idea is to cache some content on the mobile computer (client), and the website server also caches some content. According to the principle of locality (also mentioned above), there is no need to check the database in many cases.

We don't talk about the client, but the server. The SQL relational database model mentioned earlier is a widely used database. It is a hard disk database. He can perform relational queries. What is a relational query? For example, I have a student table, a course selection table, and a course table, and the students who have selected courses can be found by courses through some relationships between the three (currently abstract the table into a large array).

The hard disk database is definitely slow (the actual DBMS also has a memory cache). Now consider that we build a memory in the memory. If it is in the cache, we don’t need to query the hard disk database (the data in the hard disk database should be treated as tens of millions or even 100 million, and the cache is the most commonly used hundreds to tens of thousands). If the cache is hit, the result will be returned directly. Recalling the content mentioned above, what is suitable for this operation?

It is easy to think of the hash table mentioned above (I may not know the hash table clearly, it is best to deepen understanding by Baidu). In the same way, all kinds of trivial problems have to be dealt with, so it is also made into an infrastructure.

We can run an in-memory database on the same server hardware as a cache, or even open another server to store this hash table.

The essence of a hash table is actually a K-V pair storage structure, so this kind of database is called a KV database. The well-known software of SQL is MySQL (back-end development) and SQLite (mainly client development), while the well-known software of KV database is Redis (remote dictionary server), because the KV table is like a dictionary (a hash table in Python). Just use a dictionary to refer to).

There is a new question, what about the data that is not in the cache? Do we have to query the database? No, there are optimization methods. We must note that most of the queries may not exist at all! There is no need to check the relational database (the query of relational database is not as fast as the KV database) is definitely a waste (this situation is called **cache breakdown**, that is, because there is no cache in the database, it is necessary to check the database).

So that's the Bloom filter we want to implement today.

---
## Bloom filter introduction

Wikipedia has a detailed introduction [Wikipedia: Bloom Filter](https://en.wikipedia.org/wiki/Bloom_filter).

Readers who remember the principle of superposition of independent and irrelevant events, namely the multiplication theorem, and the important limit defined by the natural logarithm e, can make their own reasoning on the false probability to strengthen their understanding, but this is not necessary.

The basic introduction of Bloom filter can be found online

---

## Lab File Structure

Directory Structure:
```
├── CMakeLists.txt # CMake configure file
├── build_support # googletest CMake configure file
│ └── googletest
├── src
│ ├── filter
│ │ └── bloom_filter.cc ## Bloom Filter implementation
│ ├── include
│ │ ├── filter
│ │ │ └── bloom_filter.h
│ │ └── utils
│ │ ├── bitset.h
│ │ ├── fileio.h
│ │ └── hash.h
│ └── utils
│ ├── bitset.cc ## Bit String implementation
│ ├── fileio.cc # File i/o
│ └── hash.cc # implemented hash function
└── tests # googletest folder
    ├── bitset_test.cc ##
    ├── fileio_test.cc ##
    └── filter_test.cc ##
```
In the above directory, the files that need to be work on for this lab are marked with two # signs.

The following is how to download and install this lab.

```bash
$ git clone .....
$ sudo ./build_support/packages.sh
$ cd bloom_filter
$ mkdir build
$ cd build
$ cmake ..
```

For how to compile this project, see README.md. The lab should be done entirely under the lab branch, not the master branch.

If you write it correctly, you will be able to pass all gtest tests.

---
## Checkpoints

### Task 0: File IO

In this part, you will learn the file and IO knowledge under Linux by reading the source code of fileio_test.cc (this part of knowledge is often not covered in C language programming lessons because most of them are spec things, you need to check the manual when you need to use it), and you can learn the how `mmap` works.

But the above is not necessary. What is necessary is that you must read the comments in the interface file provided by fileio.h to ensure that you do not directly call the standard library `fread` and `fwrite` in the following tasks.

This is to practice **abstract layered thinking** and **use existing APIs and libraries to solve problems**.

You also need to learn how to use the interface provided by fileio.h to read and write files by observing the fileio_test.cc source file.

And learn basic gtest test function writing. This is to learn the idea of ​​**TDD Test Driven Development**.
Among them, functions such as `EXPECT_EQ` can be found in the link provided above,
The most basic gtest test function looks like this:

```c
TEST(TestSuitName, TestCaseName){
  ...
  // won't stop at Failure
  EXPECT_EQ(1+1, 2);
  ...
  for(int i = 0;i<100;i++){
    // do stop at Failure
    ASSERT_EQ(i&1, 1);
  }
  ...
}
```
Note that if there is a prefix of `DISABLED_` in front of `TestCaseName`, it will not run, you can think of this as a shortcut comment. The following is a list of commonly used assertions in gtest:
```cpp
// true or false
EXPECT_TRUE(ret): ret == true
EXPECT_FALSE(ret): ret == false

// equal or not
EXPECT_EQ(expected, actual): expected == actual
EXPECT_NE(expected, actual): expected != actual

// compare floats
EXPECT_FLOAT_EQ(expected, actual): (float)expected == actual
EXPECT_DOUBLE_EQ(expected, actual): (double)expected == actual
EXPECT_NEAR(var1, var2, tol): abs(var1-var2) <= tol

// compare strings
EXPECT_STREQ(s1, s2): s1 == s2
EXPECT_STRNE(s1, s2): s1 != s2
EXPECT_STRCASEEQ(s1, s2): s1 == s2
EXPECT_STRCASENE(s1, s2): s1 != s2
```

For more details about unit test, you will learn later (such as learning JUnit testing in the Java language).

The following will briefly introduce how to run the test. It is assumed that the reader does not have the knowledge of C/C++ project building, so the CMake file has been written and does not need to be modified.

CMake is a program that manages C/C++ projects by writing something similar to a script (called **project build**). The cmake command has been used during the previous installation.

CMake basically manages the dependencies between various files, the location of header files, and multiple executable files for the same project. The use of CMake does not require users to write complex **compile,link** commands. It can be considered as a tool for GUI IDE to use code on the command line (in fact, it is also used as a general project building program by various GUI IDE supported).

Since all the Makefile files of the compilation command set have been installed when the project is installed, when you need to compile (equivalent to clicking the IDE’s compile menu), you only need to do this (the following commands are in the build subfolder of the project directory implement):

```bash
$ make tests # Compile all tests (3 sets of tests in total)
$ make bitset_test # Compile the bit string related test, namely bitset_test.cc
$ make fileio_test # Same as above
$ make filter_test # the same
```
Execution is to execute the executable file, which is the `./executable file name` on the Linux system. For example, enter `./tests` to execute all the tests.

### Task 1: Bit Set

In this part, you will write a component that manages the bit array data structure in the Bloom filter. The basic structure is `bitset_`, which has two members, among which length_ records the length of the bit string, and the detailed of zero-length arrays is here: [Zero-Length](https://gcc.gnu.org/ onlinedocs/gcc/Zero-Length.html), this will help facilitate the construction of data structures directly on the memory hard disk.

Based on the principle of independence of each component, bitset implementation cannot rely on fileio. Bitset itself does not provide an interface for serializing to and from files but it provides a method to recognize a biset structure from memory (buffer). 

In order to continue to be independent, we provide an interface that supports other memory allocators, which will help call fileio to directly build a bitset structure on mmap's memory.

You need to implement all functions defined in `bitset.h` in `bitset.cc`. You cannot modify the definition of the structure, but you can add helper functions freely, but don't expose them in the header file.

### Task 2: Bloom Filter

In this part, you will call the interface of Task 0 and Task 1 to implement a Bloom filter. You need to understand the formulas for the parameters of the Bloom filter in order to implement them in the functions `optimal_hash_function_count` and `optimal_bitset_length`. You can freely delete these two helper functions marked as `static` as long as you implemente the filter correctly.

There is a `File` structure in the Bloom filter structure. This embedded structure is only for convenient management. It will not actually participate in the persistence part of the Bloom filter. You must override them when you initialize one Bloom filter no matter by creating a new one or reading one from memory or file.

We identify a Bloom filter use a `const char *`, that is, the name of file. Make sure you call the `remove` function to delete them after you test bf, otherwise you may create too many junk files.

An important part of the Bloom filter is the `Insert` function and the `ContainsKey` function, which should be implemented as the comments in the header file. These two functions have to call the hash function to complete the hash code calculation.

You can add macro definitions freely, but you cannot modify the existing macro definitions for testing purposes. Of course, you can also modify the testing, it is up to you.

When everything is completed, you will see a green post after running gtest, which means you finish it. Congratulations, if you are free, you can try to complete the challenging requirements.