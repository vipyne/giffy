# if 0
emcc \
-O1 \
main.c giffy.c -o giffy.wasm \
-s WASM=1 \

cp giffy.wasm '../getUserMedia_play/'
exit;
# endif
// -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"], ["ccall"]' \
// -s STANDALONE_WASM \

// -O3 \
// -s EXPORT_ALL=1 \
// -s EXIT_RUNTIME=0 \
// -o giffyGlue.html
// --js-opts 0 \

#include <stdio.h>
#include <stdlib.h>
// #include <errno.h>
#include <emscripten.h>
// #include "giffy.h"


EMSCRIPTEN_KEEPALIVE
void copy_gif_file(FILE* source, FILE* giffy)
{
  fseek(source, 0, SEEK_END);
  int source_size = ftell(source);
  rewind(source);

  char *source_buffer = (char*)malloc(sizeof(char) * source_size);

  for (int i = 0; i < source_size; ++i) {
    fread(source_buffer, sizeof(char), source_size, source);
    fputc(source_buffer[i], giffy);
  }
}


EMSCRIPTEN_KEEPALIVE
int main(int argc, char* argv[])
{
  FILE* source = fopen(argv[2], "rb");
  // errno = 0;
  FILE* giffy = fopen(argv[3], "wbx");
  // int overwrite_warning = errno;
  char* secret_message = argv[4];

  // if (*argv[1] == 'm') {
  //   write_header(giffy);
  //   write_compressed_image_data(source, giffy);
  //   write_entire_comment(giffy, secret_message);
  //   return 0;
  // }

  // if (argc != 5) {
  //   if (argc != 3) {
  //     printf("\n");
  //     printf("Please enter correct number of arguments. --\n");
  //     print_directions();
  //     return 1;
  //   }
  // } else {
  //   if (source == NULL) {
  //     printf("\n");
  //     printf("File %s doesn't seem to exist. Please check filepath and spelling. --\n", argv[2]);
  //     print_directions();
  //     return 3;
  //   }
  //   if (overwrite_warning != 0) {
  //     printf("Looks like %s already exists. Please change output filename. --\n", argv[3]);
  //     print_directions();
  //     return 4;
  //   }
  // }

  // if (*argv[1] != 'e') {
  //   if (*argv[1] != 'd') {
  //     printf("\n");
  //     printf("Please enter `e` or `d` as second argument. --\n");
  //     print_directions();
  //     return 2;
  //   }
  // }

  // if (*argv[1] == 'e') {
    // encode secret message in file
    copy_gif_file(source, giffy);
    // write_entire_comment(giffy, secret_message);
    // fclose(giffy);
  // } else if (*argv[1] == 'm') {
  //   printf(" ^^^^ start m\n");
  //   write_header(giffy);
  //   write_compressed_image_data(source, giffy);
  //   write_entire_comment(giffy, secret_message);
  // } else {
  //   // decode secret message in file
  //   parse_out_secret_message(source);
  //   fclose(source);
  // }

  return 0;
}