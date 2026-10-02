#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

static void
sixfive(int fd)
{
  char c;
  int n;
  int number = 0;
  int digits = 0;
  int valid = 1;

  while ((n = read(fd, &c, 1)) > 0) {
    if (strchr(" -\r\t\n./,", c) != 0 && c != '\0') {
      if (digits && valid && (number % 5 == 0 || number % 6 == 0))
        printf("%d\n", number);
      number = 0;
      digits = 0;
      valid = 1;
    } else if (c >= '0' && c <= '9') {
      int digit = c - '0';
      digits = 1;
      if (valid) {
        if (number > (2147483647 - digit) / 10)
          valid = 0;
        else
          number = number * 10 + digit;
      }
    } else {
      valid = 0;
    }
  }
  if (n < 0) {
    fprintf(2, "sixfive: read error\n");
    exit(1);
  }
  if (digits && valid && (number % 5 == 0 || number % 6 == 0))
    printf("%d\n", number);
}

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    sixfive(0);
    exit(0);
  }
  for (int i = 1; i < argc; i++) {
    int fd = open(argv[i], O_RDONLY);
    if (fd < 0) {
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      exit(1);
    }
    sixfive(fd);
    close(fd);
  }
  exit(0);
}
