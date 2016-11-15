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

void generate_index_stream(FILE* source, int* index_stream)
{
	printf("generate_index_stream\n");
  int length = 500;
  char buffer[length];
  fread(&buffer, 1, length, source);

  int output_color_table_index = 0;
	for (int i = 18; output_color_table_index < 101; i += 3) {
  	char rgb_pattern[3];
  	rgb_pattern[0] = buffer[i];
  	rgb_pattern[1] = buffer[i + 1];
  	rgb_pattern[2] = buffer[i + 2];

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
  index_stream[101] = '\0';
  printf("\n");
}

int verb_hash(char* index_buffer, int buff_idx, int hash_table_length)
{
	int value = 0;
	// colors are are array of 3 chars RGB
	for (int i = 0; i < buff_idx; ++i) {
		value += index_buffer[i];
	}
	// to ensure key is not outside table length;
	return value % hash_table_length;
}

void init_dictionary(struct Dictionary* dictionary_hash)
{
	printf("init_dictionary\n");
	// struct key_value_ll first_color;
 //  first_color.key = 0;
 //  first_color.value[0] = 0;
 //  first_color.next = NULL;

  // struct key_value_ll second_color;
  // second_color.key = 1;
  // second_color.value[0] = 1;
  // second_color.next = NULL;

  // struct key_value_ll clear_color;
  // clear_color.key = 2;
  // clear_color.value[0] = ' ';
  // clear_color.next = NULL;

  // struct key_value_ll end_of_data;
  // end_of_data.key = 3;
  // end_of_data.value[0] = '_';
  // end_of_data.next = NULL;


	struct key_value_ll* first_color = (struct key_value_ll*)malloc(sizeof (struct key_value_ll) );
  first_color->key = 0;
  first_color->value[0] = 0;

	printf("hashy\n");
	first_color->next = dictionary_hash->hash_table[0];
	printf("hashy22\n");
	dictionary_hash->hash_table[0] = first_color;

	printf("hashy33\n");
	dictionary_hash->total_number++;

	// e->next = d->table[h];
 // 	d->table[h] = e;
 // 	d->n++;


	printf(" firrrst init_dictionary\n");
  // dictionary_hash[1] = second_color;
  // dictionary_hash[2] = clear_color;
  // dictionary_hash[3] = end_of_data;
//   dictionary_hash = {first_color, second_color, clear_color, end_of_data};
}

void compress_image(FILE* giffy, int* index_stream, struct Dictionary* dictionary_hash)
{
	printf("start compress_image\n");
	////// LZW //////
	int hash_table_length = 4;
	char index_buffer[102];
	struct key_value_ll dict;
	int buff_idx = 0;
	fputc(0x02, giffy); // clear code
	index_buffer[0] = index_stream[0];
	printf("before loop\n");

	for (int i = 1; index_stream[i] != '\0'; ++i) {
		printf("start loop\n");
		int k = index_stream[i];

		index_buffer[buff_idx + 1] = k;

		int hash_key = verb_hash(index_buffer, buff_idx, hash_table_length);

		// if (dictionary_hash[hash_key].key > -1) {
		// 	printf("if yes\n");
		// 	index_buffer[buff_idx + 1] = k;
		// 	buff_idx++;
		// } else {
		// 	printf("if no\n");
		// 	fputc(hash_key, giffy);

		// 	// create new pattern struct entry for dictionary
		// 	struct key_value_ll* new_entry = (struct key_value_ll*)malloc(sizeof (struct key_value_ll) );
		// 	new_entry->key = hash_key;
		// 	index_buffer[buff_idx + 1] = '\0';
		// 	for (int i = 0; index_buffer[i] != '\0'; ++i) {
		// 		new_entry->value[i] = index_buffer[i];
		// 	}
		// 	new_entry->next = NULL;

		// 	// add new pattern struct to dictionary
		// 	dictionary_hash[hash_key] = *new_entry;

		// 	hash_table_length++;
		// 	buff_idx = 0;
		// 	index_buffer[buff_idx] = k;
		// 	free (new_entry);
		// 	// k = NULL;
		// }

	};
	// fputc(index_buffer, giffy); // need to do this but ug
}

void write_image_data(FILE* source, FILE* giffy)
{
	printf("start write_image_data\n");
  //    I M A G E   D A T A
  fputc(0x02, giffy); // LZW min code size - 2
  fputc(0x16, giffy); // number of bytes in data sub-block

  int index_stream[102];

  struct Dictionary dictionary_hash;

  init_dictionary(&dictionary_hash);
	printf("i predict this wont print out\n");
  generate_index_stream(source, index_stream);
	printf("before compress_image\n");

  compress_image(giffy, index_stream, &dictionary_hash);

  fputc(0x00, giffy); // end image data | end data sub block
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
          printf("%c", c);
        }
      }
      last_c = c;
    }
  }
  printf("\n");
  fclose(source);
};



