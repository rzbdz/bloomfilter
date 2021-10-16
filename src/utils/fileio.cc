#include "utils/fileio.h"

#define ALIGN_4K(n) ((n + 4095) & ~(4096 - 1))

static int open_file(const char* fname,
                     File* file,
                     size_t length,
                     int prot,
                     int oflag,
                     int s) {
  if (fname == NULL) {
    printf("fileio: null filename! abort()\n");
    // abort();
    return -1;
  }
  int fd = open(fname, oflag, s);
  struct stat buf;
  if (fd == -1 || fstat(fd, &buf) == -1) {
    printf("fileio: file %s is not exist! abort()\n", fname);
    // abort();
    return -1;
  }
  if (oflag & O_TRUNC) {
    length = ALIGN_4K(length);
    ftruncate(fd, length);
  } else {
    length = buf.st_size;
  }
  file->length_ = length;
  file->data_ = (char*)mmap(NULL, length, prot, MAP_SHARED, fd, 0);
  if (file->data_ == NULL) {
    printf("fileio: mmap failed! abort()");
    return -1;
  }
  close(fd);
  return 0;
}

int OpenExistedFile(const char* fname, File* file, size_t length) {
  return open_file(fname, file, length, PROT_READ | PROT_WRITE, O_RDWR, 0);
}

int OpenNewFile(const char* fname, File* file, size_t length) {
  return open_file(fname, file, length, PROT_READ | PROT_WRITE,
                   O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
}

int ForceWriteBack(File* file) {
  return msync(file->data_, file->length_, MS_ASYNC);
}

int CloseFile(File* file) {
  return munmap(file->data_, file->length_);
}