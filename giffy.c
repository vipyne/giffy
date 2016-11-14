
#include <stdio.h>
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
  char signature[7] = "vipyne";
  flockfile(giffy);
  for (int i = 0; signature[i] != '\0'; ++i ) {
    putc_unlocked(signature[i], giffy);
  };
  fputc(0x00, giffy);
}

// global for now because I HAVE NO IDEA WHAT IM DOING
char color_table[6] = {0x20, 0x79, 0x4B, 0x00, 0x00, 0x4B};

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
  fputc(0x00, giffy);// width
  fputc(0xFF, giffy);
  fputc(0x00, giffy);// height
  fputc(0xFF, giffy);
  // _ color table bool | ___ resolution | _ sort flag | ___ size of table
  // size of table == 000 => 2 colors;
  fputc(0x80, giffy);// packed /// color table options - whether or not has
  fputc(0x01, giffy);// background color index
  fputc(0x00, giffy);// pixel aspect ratio

  //    C O L O R   T A B L E
  // global color table
  // TODO: iterate through the color_table you just made
  fputc(0x20, giffy);
  fputc(0x79, giffy);
  fputc(0x4B, giffy);

  fputc(0x00, giffy);
  fputc(0x00, giffy);
  fputc(0x4B, giffy);

  // fputc(0xBB, giffy);
  // fputc(0x13, giffy);
  // fputc(0x2A, giffy);

  // fputc(0x9A, giffy);
  // fputc(0x9A, giffy);
  // fputc(0x75, giffy);
  // end color table

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

void generate_index_stream(FILE* source, char* index_stream)
{
  int length = 500;
  char buffer[length];
  fread(&buffer, 1, length, source);

  printf("18th byte : %c \n", buffer[17]);
  printf("start: 19th byte : %c \n", buffer[18]);

  int output_color_table_index = 0;
	for (int i = 18; output_color_table_index < 10; i += 3) {
  	char rgb_pattern[3];
  	rgb_pattern[0] = buffer[i];
  	rgb_pattern[1] = buffer[i + 1];
  	rgb_pattern[2] = buffer[i + 2];

  	// printf("color_table[0] : %d\n", color_table[0]);
  	// printf("color_table[3] : %d\n", color_table[3]);
  	printf("rgb_pattern[0] : %d\n", rgb_pattern[i]);
  	if (rgb_pattern[0] == color_table[0]) { // first color
  		index_stream[output_color_table_index] = 0;
  	}
  	if (rgb_pattern[0] == color_table[3]) { // second color
  		index_stream[output_color_table_index] = 1;
  	}
  	printf("i_m: %d |", index_stream[output_color_table_index]);
  	output_color_table_index++;
  }
  index_stream[10] = '\0';
  printf("\n");
}

int verb_hash(char* key_input_color, int hash_table_length) {
	int value = 0;
	// colors are are array of 3 chars RGB
	for (int i = 0; i < 3; ++i) {
		value += key_input_color[i];
	}
	// to ensure key is not outside table length;
	return value % hash_table_length;
}

// struct key_value_ll {
// 	int key;
// 	char value[3];
// 	struct key_value_ll* next;
// }

void write_image_data(FILE* source, FILE* giffy)
{
  //    I M A G E   D A T A
  fputc(0x02, giffy); // LZW min code size - 2
  fputc(0x16, giffy); // number of bytes in data sub-block

  char index_stream[11];

  generate_index_stream(source, index_stream);
  // compress_image(giffy, index_stream);

  // // array of indexes to color table
  // char dictionary_array[256] = {0x00, '1', ' ', '_'};
  // char compression_stream[256];
  // char empty_pattern[10];
  // char pattern;

  // for (int i = 0; i < dictionary_array.length; ++i) {
  // 	fputc(dictionary_array[i], giffy);
  // }

  // for (int i = 0; i < compression_stream.length; ++i) {
  //   fputc(compression_stream[i], giffy); // in bits
  // }

  fputc(0x00, giffy); // end image data | end data sub block
}

void compress_image(FILE* giffy, char* index_stream)
{
	// // LZW //////

	// char index_buffer[10];
	// char ch;
	// index_buffer[bi] = index_stream[0];

	// for (int i = 1; index_stream[i] != '\0'; ++i) {
	// 	char new_index_buffer[] = index_buffer;



	// 	index_buffer[i]
	// 	ch = index_stream[i];
	// 	char new_index_buffer = {ch, index_buffer[]};


	// 	if (dictionary contains index_buffer) {
	// 		index_buffer
	// 	} else {
	// 		fputc(s+ch, giffy);
	// 		// add s+ch to dictionary
	// 		s = ch;
	// 	}
	// };
}

// void read_input_file(FILE* source, callback)
// {
// }

int long write_secret_message(FILE* giffy, char* secret_message)
{
  int long i;
  for (i = 0; secret_message[i] != '\0'; ++i) {
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
  char app_extension[19] = {0x21, 0xFF, 0x0B, 'N', 'E', 'T', 'S', 'C', 'A', 'P', 'E', '2', '.', '0', '3', '1', '0', '0', '0'};
  for (int i = 0; i < 19; ++i) {
    fputc(app_extension[i], giffy);
  };

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
  char m;
  char last_c;
  int print = 0;

  while (!feof(source)) {
    c = fgetc(source);
    if (c != '\0') {
      if (last_c == (signed char)0x21 && c == (signed char)0xFE) {
      	int length = fgetc(source);
      	for(int i = 0; i < length; ++i) {
	        c = fgetc(source);
	        // if (c != '_')
          printf("%c", c);
        }
      }
      last_c = c;
    }
  }
  printf("\n");
  fclose(source);
};



