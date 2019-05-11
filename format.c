#include <stdio.h>

#include <bindiff.h>

void format_difference(char *buf, size_t n, diff_t *difference) {
  int position;
  position = snprintf(buf, n, "Difference starting at offset %ld (size %ld):\n",
                      difference->offset,
                      difference->size);
  if (difference->printable)
    snprintf(buf+position, n-position,
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
    snprintf(buf+position, n-position, "\n");
  }
}
