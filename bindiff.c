#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <bindiff.h>

diff_t *compare_data(int fd1, int fd2, size_t size) {
  char *data1 = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd1, 0);
  char *data2 = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd2, 0);

  diff_t *result = compare(data1, data2, size);

  munmap(data1, size);
  munmap(data2, size);

  return result;
}

int main(int argc, char *argv[]) {
  char *file1 = argv[1];
  char *file2 = argv[2];
  // Not handling any errors for now
  int fd1 = open(file1, O_RDONLY);
  int fd2 = open(file2, O_RDONLY);

  off_t size1 = lseek(fd1, 0, SEEK_END);
  off_t size2 = lseek(fd2, 0, SEEK_END);

  if (size1 != size2)
    printf("Files %s and %s have different size (%ld and %ld).\n", file1, file2, size1, size2);
  else {
    diff_t *result = compare_data(fd1, fd2, size1);
    if (result == NULL)
      printf("Files %s and %s are equal.\n", file1, file2);
    else
      printf("Files %s and %s are different.\n", file1, file2);
    while (result) {
      diff_t *to_delete = result;
      result = result->next;
      free(to_delete);
    }
  }

  close(fd1);
  close(fd2);
}
