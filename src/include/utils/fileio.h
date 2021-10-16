#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

typedef char* DataPtr;

struct file_ {
  DataPtr data_;
  size_t length_;
};

/**
 * You are able to get access to the
 * file data directly using the struct file_.data_
 * member like this:
 *
 *    File file;
 *    file.data_[100] = 'A';
 *
 * Of course, you can bind a data structure to that memory:
 *
 *    DataPtr data = (file.data_ + sizeof(size_t));
 *    ((BitSet)data)[i] = (uint32_t)0x7fffffff;
 *
 * all data written to the data_ field will be flush
 * to the file on disk after ForceWriteBack() function
 * being called or scheduled by the operating system.
 */
typedef struct file_ File;

/**
 * @brief open a file that is already exist
 *
 * @param fname the name of the file
 * @param file the file structure to store info, must be declared
 *             before call and pass like this: &file
 * @param length the length of the file if you know it, usually be 0.
 * @return int, 0 if success, -1 if fails or any errors occurs
 */
int OpenExistedFile(const char* fname, File* file, size_t length);

/**
 * @brief create a new file
 *
 * @param fname name of file
 * @param file must be declared before call
 *             and pass like this: &file
 * @param length the length of the file to create
 * @return int, 0 if success, -1 if fails or any errors occurs
 */
int OpenNewFile(const char* fname, File* file, size_t length);

/**
 * @brief make sure changes on file would be flush to file itself
 *        (but it won't garantee that).
 * 
 * @param file the file, should pass like this &file
 * @return int, 0 if success, -1 if fails or any errors occurs
 */
int ForceWriteBack(File* file);

/**
 * @brief close a file, after function call, the 
 *        file structure is no longer be valid, if 
 *        get access to the range of address, 
 *        a Segmentation Fault will arise.
 * if you want to remove a file, please call remove()
 * in the <stdio.h>.
 * @param file the file, should pass like this &file 
 * @return int, 0 if success, -1 if fails or any errors occurs
 */
int CloseFile(File* file);