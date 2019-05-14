#include <stdio.h>

#include <bindiff.h>

size_t format_difference(char *buf, size_t n, diff_t *difference) {
  size_t position;
  position = snprintf(buf, n, "Difference starting at offset %ld (size %ld):\n",
                      difference->offset,
                      difference->size);
  if (difference->printable)
    position += snprintf(buf+position, n-position,
                         ">>> str %.*s\n"
                         "<<< str %.*s\n",
                         (int) difference->size,
                         difference->first,
                         (int) difference->size,
                         difference->second);
  else {
    int i;
    position += snprintf(buf+position, n-position, ">>> hex");
    for (i = 0; i < difference->size; i++)
      position += snprintf(buf+position, n-position, " %02X", (unsigned char) difference->first[i]);
    position += snprintf(buf+position, n-position, "\n<<< hex");
    for (i = 0; i < difference->size; i++)
      position += snprintf(buf+position, n-position, " %02X", (unsigned char) difference->second[i]);
    position += snprintf(buf+position, n-position, "\n");
  }
  return position;
}
