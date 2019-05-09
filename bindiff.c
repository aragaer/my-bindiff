#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <bindiff.h>

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
  else if (compare(NULL, NULL, 0) == NULL)
    printf("Files %s and %s are equal.\n", file1, file2);

  close(fd1);
  close(fd2);
}
