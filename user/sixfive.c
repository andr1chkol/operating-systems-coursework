#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"


void sixfive(int fd){
  char c;
  int n;
  int number = 0;
  int read_number = 0;
  int valid = 1;
  while ((n = read(fd, &c, 1)) > 0 ){
       if (c >= '0' && c <= '9'){
         if (valid){
          number = number * 10 + (c-'0');
          read_number = 1;
          }
       }
       else if(strchr(" -\r\t\n./,", c)) {
        if (read_number && valid) {
          if (number % 5 == 0 || number % 6 == 0){
          printf("%d\n", number);
          }

          number = 0;
          read_number = 0;
          valid = 1;
        }
        else {
        number = 0;
        read_number = 0;
        valid = 0;
        }
      }
  }
  if (read_number && valid){
    if (number % 5 == 0 || number % 6 == 0){
      printf("%d\n", number);
    }
  }
  if (n < 0){
    fprintf(2, "sixfive read error\n");
    exit(1);
  }
}

int main (int argc, char *argv[]){
  int fd;

  if (argc <= 1){
    printf("Error: too few arguments\n");
    printf("Usage %s <file> [<file> ...]\n", argv[0]);
    exit(1);
  }
  for (int i = 1; i < argc;  i++) {
    if ((fd = open(argv[i], O_RDONLY)) < 0){
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      exit(1);
    }
    sixfive(fd);
    close(fd);
  }
  exit(0);

}
