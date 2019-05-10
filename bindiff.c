#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
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
  else {
    char *data1 = mmap(NULL, size1, PROT_READ, MAP_PRIVATE, fd1, 0);
    char *data2 = mmap(NULL, size2, PROT_READ, MAP_PRIVATE, fd2, 0);
    diff_t *result = compare(data1, data2, size1);

    if (result == NULL)
      printf("Files %s and %s are equal.\n", file1, file2);
    else
      printf("Files %s and %s are different.\n", file1, file2);

    while (result) {
      diff_t *current = result;
      fprintf(stderr, "Result: %p\n", current);
      result = result->next;
      printf("Difference starting at offset %ld (size %ld):\n",
             current->offset, current->size);
      if (current->printable) {
        printf(">>> str %.*s\n", (int) current->size, current->first);
        printf("<<< str %.*s\n", (int) current->size, current->second);
      } else {
        int i;
        printf(">>> hex");
        for (i = 0; i < current->size; i++)
          printf(" %02X", (unsigned char) current->first[i]);
        printf("\n<<< hex");
        for (i = 0; i < current->size; i++)
          printf(" %02X", (unsigned char) current->second[i]);
        printf("\n");
      }
      free(current);
    }

    munmap(data1, size1);
    munmap(data2, size2);
  }

  close(fd1);
  close(fd2);
}
