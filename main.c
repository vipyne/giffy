#include <stdio.h>
#include <errno.h>
#include "giffy.h"

int main(int argc, char* argv[])
{
  FILE* source = fopen(argv[2], "rb");
  FILE* giffy = fopen(argv[3], "wbx");
  char* secret_message = argv[4];

  if (argc != 5) {
    if (argc != 3) {
      printf("\n");
      printf("Please enter correct number of arguments. --\n");
      print_directions();
      return 1;
    }
  } else {
    if (errno != 0) {
      printf("Looks like %s already exists. Please change output filename. --\n", argv[3]);
      print_directions();
      return 4;
    }
  }

  if (*argv[1] != 'e') {
    if (*argv[1] != 'd') {
      printf("\n");
      printf("Please enter `e` or `d` as second argument. --\n");
      print_directions();
      return 2;
    }
  }

  if (source == NULL) {
    printf("\n");
    printf("File %s doesn't seem to exist. Please check filepath and spelling. --\n", argv[2]);
    print_directions();
    return 3;
  }

  if (*argv[1] == 'e') {
    // encode secret message in file
    copy_gif_file(source, giffy);
    write_entire_comment(giffy, secret_message);
    fclose(giffy);
  } else {
    // decode secret message in file
    parse_out_secret_message(source);
    fclose(source);
  }

  return 0;
}