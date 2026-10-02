#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define DATASIZE (32 * PGSIZE)
#define MSG "Here it is: "

int
main(int argc, char *argv[])
{
  char *data = sbrk(DATASIZE);
  if (data == SBRK_ERROR)
    exit(1);

  int msglen = strlen(MSG);

  for (int i = 0; i <= DATASIZE - msglen; i++) {
    if (memcmp(data + i, MSG, msglen) == 0) {
      int start = i + msglen;
      int end = start;
      while (end < DATASIZE && data[end] != '\0')
        end++;
      if (end == DATASIZE || end == start)
        continue;
      write(1, data + start, end - start);
      write(1, "\n", 1);
      exit(0);
    }
  }

  exit(1);
}
