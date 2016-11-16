#include <stdio.h>
#include "giffy.h"

int main(int argc, char* argv[])
{
  FILE* source = fopen(argv[2], "rb");
  FILE* giffy = fopen(argv[3], "wb");
  char* secret_message = argv[4];

  // if (source) CHECK IF INPUT IS GIF

  if (*argv[1] == 'e') {
    copy_gif_file(source, giffy);
    write_entire_comment(giffy, secret_message);
    fclose(giffy);
  } else {
    parse_out_secret_message(source);
    fclose(source);
  }

  return 0;
}