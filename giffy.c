#include <stdio.h>
#include <stdlib.h>
#include "giffy.h"

/////  E N C O D E R  /////

void write_secret_message_symbols(FILE* giffy)
{
  flockfile(giffy);
  for (int i = 0; i < 4; ++i) {
    putc_unlocked('_', giffy);
  }
}

void write_comment_end(FILE* giffy)
{
	// if NETSCAPE 2.0 can do it, i can sign my work too
  char signature[7] = "vipyne";
  flockfile(giffy);
  for (int i = 0; signature[i] != '\0'; ++i ) {
    putc_unlocked(signature[i], giffy);
  };
  fputc(0x00, giffy);
}

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

void write_header(FILE* giffy)
{
  //    G I F   H E A D E R
  char gif_header[6] = {'G', 'I' ,'F' , '8', '9', 'a'};
  for (int i = 0; i < 6; ++i) {
    fputc(gif_header[i], giffy);
  }

  /*short int canvas_width;  // TODO: ensure this is little endian
  short int canvas_height;
  char packed_field;
  char pixel_aspect_ratio;*/
  //    L O G I C A L   S C R E E N   D E S C R I P T O R
  fputc(0x03, giffy);
  fputc(0x00, giffy);// width
  fputc(0x03, giffy);
  fputc(0x00, giffy);// height
  // _ color table bool | ___ resolution | _ sort flag | ___ size of table
  // size of table == 000 => 2 colors;
  fputc(0x81, giffy);// packed /// color table options - whether or not has
  fputc(0x00, giffy);// background color index
  fputc(0x00, giffy);// pixel aspect ratio

  //    C O L O R   T A B L E
  // global color table
  // TODO: iterate through the color_table you just made
  fputc(0x00, giffy);
  fputc(0xFF, giffy);
  fputc(0xFF, giffy);  // 0 Cyan

  fputc(0x88, giffy);
  fputc(0x11, giffy);
  fputc(0x66, giffy);  // 1 Purple

  fputc(0x00, giffy);
  fputc(0x00, giffy);
  fputc(0x00, giffy);  // 2 Black

  fputc(0x00, giffy);
  fputc(0xFF, giffy);
  fputc(0x00, giffy);  // 3 Green
  // end color table

  //    I M A G E   D E S C R I P T O R
  fputc(0x2C, giffy);
  fputc(0x00, giffy);
  fputc(0x00, giffy);
  fputc(0x00, giffy);
  fputc(0x00, giffy);// width
  fputc(0x03, giffy);
  fputc(0x00, giffy);// length
  fputc(0x03, giffy);
  fputc(0x00, giffy);
  fputc(0x00, giffy); // image_descriptor packed_field
   // // not used yet, hard coded for now
  // gif_image_descriptor image_descriptor;
  // image_descriptor.image_width = 0xFF; // need to get image dim... ffmpeg?  vips?
  // image_descriptor.image_height = 0xFF;
}

void write_compressed_image_data(FILE* source, FILE* giffy)
{
  // int pixel_color_indeces[] = {1,3,1,3,2,3,1,3,1};

  // unsigned char *binary_arr = calloc(1,sizeof(char)*9);

  // binary_arr[0] = 1;
  // binary_arr[1] = 3;
  // binary_arr[2] = 1;

  // struct packed_struct {
  //   unsigned char one : 1;
  //   unsigned char two : 1;
  //   unsigned char three : 1;
  //   unsigned char four : 1;
  //   unsigned char five : 1;
  //   unsigned char six : 1;
  //   unsigned char seven : 1;
  //   unsigned char eight : 1;
  // }

  // how to create a binary stream?



  //    I M A G E   D A T A
  fputc(0x02, giffy); // LZW min code size - 2
  fputc(0x0A, giffy); // number of bytes in data sub-block

  fputc(0xCC, giffy); //
  fputc(0xB2, giffy); //
  fputc(0x2C, giffy); //
  fputc(0xCB, giffy); //
  fputc(0xB6, giffy); //

  fputc(0x6D, giffy); //
  fputc(0xDB, giffy); //
  fputc(0xB2, giffy); //
  fputc(0x6C, giffy); //
  fputc(0xCB, giffy); //

  fputc(0xA2, giffy); //

  fputc(0x00, giffy); // end image data | end data sub block
  // fputc(0x00, giffy); // end image data | end data sub block
}

void compress_image(FILE* giffy, char* index_stream)
{
	// // LZW //////
}

int long write_secret_message(FILE* giffy, char* secret_message)
{
  int long i;
  for (i = 0; secret_message[i] != '\0'; ++i) {
    fputc(secret_message[i], giffy);
  }
  return i;
}

void write_entire_comment(FILE* giffy, char* secret_message)
{
	// fseek(giffy, -1L, SEEK_END); // rewind and start to write over original trailer

  /*//    P L A I N   T E X T   E X T E N S I O N
  fputc(0x21, giffy);// plain text extension start
  fputc(0x01, giffy);// plain text label
  fputc(0x00, giffy); // block size (let's try 0 for now but could add something later:shrug_emoji:)
  fputc(0x00, giffy);

  //    A P P L I C A T I O N   E X T E N S I O N
  / *byte  1        : 33 (hex 0x21) GIF Extension code
    byte  2        : 255 (hex 0xFF) Application Extension Label
    byte  3        : 11 (hex 0x0B) Length of Application Block
                   (eleven bytes of data to follow)
    bytes 4 to 11  : "NETSCAPE"
    bytes 12 to 14 : "2.0"
    byte  15       : 3 (hex 0x03) Length of Data Sub-Block
                   (three bytes of data to follow)
    byte  16       : 1 (hex 0x01)
    bytes 17 to 18 : 0 to 65535, an unsigned integer in
                   little-endian byte format. This specifies the
                   number of times the loop should
                   be executed.
    byte  19       : 0 (hex 0x00) a Data Sub-Block Terminator. * /
  char app_extension[19] = {0x21, 0xFF, 0x0B, 'N', 'E', 'T', 'S', 'C', 'A', 'P', 'E', '2', '.', '0', '3', '1', '0', '0', '0'};
  for (int i = 0; i < 19; ++i) {
    fputc(app_extension[i], giffy);
  };*/

  //    C O M M E N T   E X T E N S I O N
  fputc(0x21, giffy); // extension start
  fputc(0xFE, giffy); // comment label
  fputc(0x00, giffy); // will be overwritten with length of secret message
  int long length = write_secret_message(giffy, secret_message);
  write_secret_message_symbols(giffy);
  write_comment_end(giffy);

  //  the -16 is to account for `____vipyne0x00`
  fseek(giffy, -12 - length, SEEK_CUR); // rewind to length char
  fputc(length, giffy);
  fseek(giffy, 0L, SEEK_END);

  //    T R A I L E R
  fputc(0x3B, giffy);
}

/////  D E C O D E R  /////

void parse_out_secret_message(FILE* source)
{
  char c;
  char last_c;
  char *message_buffer;

  while (!feof(source)) {
    c = fgetc(source);
    if (c != '\0') {
      if (last_c == (signed char)0x21 && c == (signed char)0xFE) {
      	int length = fgetc(source); // this byte tells us length of message
      	message_buffer = (char*)malloc(sizeof(char) * length);
      	for(int i = 0; i < length; ++i) {
	        c = fgetc(source);
	        message_buffer[i] = c;
        }
      }
      last_c = c;
    }
  }
  printf("\n");

  printf("%s", message_buffer);  // only print the last comment - that is the one we added

  printf("\n");
  printf("\n");
};

/////  U T I L  /////

void print_directions(void) {
  printf("\n");
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("To encode a message:\n");
  printf("$ ./giffy.exe e [input-image-file] [output-filename] [message-string]\n");
  printf("Ensure `message-string` is wrapped in quotes\n");
  printf("\n");
  printf("To decode an image:\n");
  printf("$ ./giffy.exe d [input-image-file]\n");
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\n");
}