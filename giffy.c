
#include <stdio.h>
#include "giffy.h"


/////  E N C O D E R  /////


void write_secret_message_symbols(FILE* giffy)
{
  // flockfile(giffy);
  for (int i = 0; i < 4; ++i) {
    fputc('_', giffy);
  }
}

void write_comment_end(FILE* giffy)
{
  char signature[7] = "vipyne";
  // flockfile(giffy);
  for (int i = 0; signature[i] != '\0'; ++i ) {
    // putc_unlocked(signature[i], giffy);
    fputc(signature[i], giffy);
  };
  fputc(0x00, giffy);
}

void write_header(FILE* giffy)
{
  //    G I F   H E A D E R
	printf("wtfasdfasdfasdf\n");
  char gif_header[6] = {'G', 'I' ,'F' , '8', '9', 'a'};
  for (int i = 0; i < 6; ++i) {
  	printf("wtf\n");
    fputc(gif_header[i], giffy);
  }

  /*short int canvas_width;  // TODO: ensure this is little endian
  short int canvas_height;
  char packed_field;
  char pixel_aspect_ratio;*/
  //    L O G I C A L   S C R E E N   D E S C R I P T O R
  fputc(0xFF, giffy);
  fputc(0x00, giffy);// width
  fputc(0xFF, giffy);
  fputc(0x00, giffy);// height
  fputc(0x00, giffy);// packed /// color table options - whether or not has
  fputc(0x00, giffy);// background color index
  fputc(0x00, giffy);// pixel aspect ratio

  //    C O L O R   T A B L E
  /*// global color table
  fputc(0xB0, giffy);
  fputc(0x99, giffy);
  fputc(0x4B, giffy);
  fputc(0x00, giffy);
  fputc(0x00, giffy);
  fputc(0x4B, giffy);
  fputc(0xBB, giffy);
  fputc(0x13, giffy);
  fputc(0x9A, giffy);
  fputc(0x9A, giffy);
  fputc(0x9A, giffy);
  fputc(0x75, giffy);
  // end color table*/

  //    I M A G E   D E S C R I P T O R
  fputc(0x2C, giffy);
  fputc(0x00, giffy);
  fputc(0x00, giffy);// width
  fputc(0x00, giffy);
  fputc(0x00, giffy);// length
  fputc(0xFF, giffy);
  fputc(0x00, giffy);
  fputc(0xFF, giffy);
  fputc(0x00, giffy);
  fputc(0x00, giffy); // image_descriptor packed_field
   // // not used yet, hard coded for now
  // gif_image_descriptor image_descriptor;
  // image_descriptor.image_width = 0xFF; // need to get image dim... ffmpeg?  vips?
  // image_descriptor.image_height = 0xFF;

}

void write_image_data(FILE* source, FILE* giffy)
{
  //    I M A G E   D A T A
  int length = 100;
  fputc(0x02, giffy); // LZW min code size - 2
  fputc(0xFF, giffy); // number of bytes in data sub-block

  char buffer[length];
  fread(&buffer, 1, length, source);

  int i = 0;
  int writing = 0;

  while (writing < length) {
    if (buffer[i] > 0x00) {
      fputc(buffer[i], giffy);
      ++writing;
    }
    ++i;
  }
  fputc(0x00, giffy); // end image data
}

// void read_input_file(FILE* source, callback)
// {
// }

int long write_secret_message(FILE* giffy, char* secret_message)
{
  printf("derp88888      8888\n");
  int long i;
  for (i = 0; secret_message[i] != '\0'; ++i) {
        printf("poop");

    fputc(secret_message[i], giffy);
  }
  printf("%s\n", secret_message);

  return i;
}

void write_extensions(FILE* giffy, char* secret_message)
{

  /*//    P L A I N   T E X T   E X T E N S I O N
  fputc(0x21, giffy);// plain text extension start
  fputc(0x01, giffy);// plain text label
  fputc(0x00, giffy); // block size (let's try 0 for now but could add something later:shrug_emoji:)
  fputc(0x00, giffy); */

  //    A P P L I C A T I O N   E X T E N S I O N
  /*byte  1        : 33 (hex 0x21) GIF Extension code
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
    byte  19       : 0 (hex 0x00) a Data Sub-Block Terminator. */
        printf("$ $ WHAAH\n");
  char app_extension[19] = {0x21, 0xFF, 0x0B, 'N', 'E', 'T', 'S', 'C', 'A', 'P', 'E', '2', '.', '0', '3', '1', '0', '0', '0'};
  for (int i = 0; i < 19; ++i) {
        printf("$ $ poop\n");

    fputc(app_extension[i], giffy);
  };

  //    C O M M E N T   E X T E N S I O N
  fputc(0x21, giffy); // extension start
  fputc(0xFE, giffy); // comment label
  fputc(0x00, giffy); // will be overwritten with length of secret message
  // write_secret_message_symbols(giffy);
  int long length = write_secret_message(giffy, secret_message);
  // write_secret_message_symbols(giffy);
  write_comment_end(giffy);


  //  the -16 is to account for `____` && `____vipyne0x00`
  fseek(giffy, -16 - length, SEEK_CUR); // rewind to length char
  fputc(length, giffy);
  fseek(giffy, 0L, SEEK_END);

  //    T R A I L E R
  fputc(0x3B, giffy);
  printf("nerrdd\n" );

}

/////  D E C O D E R  /////

void parse_out_secret_message(FILE* source)
{
  char c;
  char m;
  char last_c;
  int print = 0;

  while (!feof(source)) {
    c = fgetc(source);
    // if (c != '\0') {
          printf(":%c", c);
      if (last_c == (signed char)0xFE && c == (signed char)0x1C) {
        while (m != '_') {
          m = fgetc(source);
          printf("%c", m);
        }
        printf("poop");
      }
      last_c = c;
        // printf("pooasfdp");

    // }
  }

  printf("\n");
  fclose(source);

  // int length = 1000;
  // char buffer[length];
  // rewind(source);
  // fread(&buffer, 16, length, source);

  // int i = 0;
  // int done_reading = 0;

  // while (i < length) {
  // printf("%x", buffer[i]);
  //  if (buffer[i] == 'f') {
  //    if (buffer[i] == 0x1C) {
  //      printf("%c", buffer[i]);
  //    }
  //  }
  //  ++i;
  // }
};



