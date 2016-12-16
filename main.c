#include <stdio.h>
#include "giffy.h"

int main(int argc, char* argv[])
{
  FILE* source = fopen(argv[2], "rb");
  FILE* giffy = fopen(argv[3], "wb");
  char* secret_message = argv[4];

  // TODO: stop putting off error handling
   // return 1;
   // printf(" ^^^^ oops and error occured\n");
   printf(" ^^^^ start\n");

  if (*argv[1] == 'e') {
    write_header(giffy);
    printf(" ^^^^ after header\n");
    write_image_data(source, giffy);
    write_extensions(giffy, secret_message);

    fclose(giffy);
  } else if (*argv[1] == 'm') {
    printf(" ^^^^ start m\n");
    write_header(giffy);
    write_image_data(source, giffy);
    write_extensions(giffy, secret_message);
  } else {
    parse_out_secret_message(source);
  }

  return 0;
}