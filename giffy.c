// vanessa writes a gif maker

#include <stdio.h>
#include <iostream>

typedef struct {
	char g;
	char i;
	char f;
	char ve;
	char rs;
	char ion;
	// char version; TODO: do this way better
} gif_header;

typedef struct {
	short int canvas_width;  // TODO: ensure this is little endian
	short int canvas_height;
	char packed_field;
	char pixel_aspect_ratio;
} gif_logical_screen_descriptor; 

typedef struct {
	char start_image_descriptor;
	short int x_start;
	short int y_start;
	short int image_width; 
	short int image_height; 
	char packed_field;
} gif_image_descriptor;

typedef struct {
	char start; // 0x21
} plain_text_extension;

typedef struct {
} global_color_table;

typedef struct {
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
} application_extension;

void write_metadata(gif_header header, gif_logical_screen_descriptor descriptor, gif_image_descriptor image_descriptor, FILE* giffy) 
{
	//    G I F   H E A D E R
	fputc(0x47, giffy);
	fputc(0x49, giffy);
	fputc(0x46, giffy);
	fputc(header.ve, giffy);
	fputc(header.rs, giffy);
	fputc(header.ion, giffy);
	
	//    L O G I C A L   S C R E E N   D E S C R I P T O R
	fputc(0xFF, giffy);
	fputc(0x00, giffy);// width
	fputc(0xFF, giffy);
	fputc(0x00, giffy);// height
	fputc(0x00, giffy);// packed ///////////////////////////// color table options - whether or not has
	fputc(0x00, giffy);// background color index
	fputc(0x00, giffy);// pixel aspect ratio
	// TODO: use struct to write this... i mean this comment applies to entire file
	//fputc(descriptor.canvas_width, giffy);
	//fputc(descriptor.canvas_height, giffy);
	//fputc(descriptor.packed_field, giffy);
	//fputc(descriptor.pixel_aspect_ratio, giffy);

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
	//fputc(image_descriptor.image_width, giffy);
	//fputc(image_descriptor.image_height, giffy);
	fputc(0xFF, giffy);
	fputc(0x00, giffy);
	fputc(0xFF, giffy);
	fputc(0x00, giffy);
	fputc(0x00, giffy); // image_descriptor packed_field
}

void write_image_data(FILE* source, FILE* giffy)
{
	//    I M A G E   D A T A
	int length = 10;
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

int long write_secret_message(FILE* giffy, char* secret_message) 
{
  int long i;
	for (i = 0; secret_message[i] != '\0'; ++i ) {
		fputc(secret_message[i], giffy);
	}
	std::cout << "%% i:" << i << std::endl;
	std::cout << "%% secret_message:" << secret_message << std::endl;
	return i;
}

void write_extensions(FILE* giffy, char* secret_message)
{
	/*//    P L A I N   T E X T   E X T E N S I O N
	fputc(0x21, giffy);// plain text extension start
	fputc(0x01, giffy);// plain text label
	fputc(0x00, giffy); // block size (let's try 0 for now :shrug_emoji:
	fputc(0x00, giffy); */

	//    A P P L I C A T I O N   E X T E N S I O N
	fputc(0x21, giffy); // start ( 0x21 )
	fputc(0xFF, giffy);
	fputc(0x0B, giffy);
	fputc('N', giffy);
	fputc('E', giffy);
	fputc('T', giffy);
	fputc('S', giffy);
	fputc('C', giffy);
	fputc('A', giffy);
	fputc('P', giffy);
	fputc('E', giffy);
	fputc('2', giffy);
	fputc('.', giffy);
	fputc('0', giffy);
	fputc(0x03, giffy);
	fputc(0x01, giffy);
	fputc(0x0A, giffy);
	fputc(0x00, giffy);
	
	//    C O M M E N T   E X T E N S I O N
	fputc(0x21, giffy); // extension start
	fputc(0xFE, giffy); // comment label
	fputc(0x00, giffy); // will be overwritten with length of secret message
	fputc('_', giffy);
	fputc('_', giffy);
	fputc('_', giffy);
	fputc('_', giffy);
	int long length = write_secret_message(giffy, secret_message);
	fputc('_', giffy);
	fputc('_', giffy);
	fputc('_', giffy);
	fputc('_', giffy);
	fputc('v', giffy);
	fputc('i', giffy);
	fputc('p', giffy);
	fputc('y', giffy);
	fputc('n', giffy);
	fputc('e', giffy);
	fputc(0x00, giffy);

	//  the -16 is to account for `____` && `____vipyne0x00`
	fseek(giffy, -16 - length, SEEK_CUR); // rewind to length char
	fputc(length, giffy);
	fseek(giffy, 0L, SEEK_END);
}

int main(int argc, char* argv[])
{
	FILE* giffy;
	FILE* source;
	source = fopen(argv[1], "rb");
	char* secret_message = argv[3];

	gif_header header;
	gif_logical_screen_descriptor descriptor;
	gif_image_descriptor image_descriptor;

	giffy = fopen(argv[2], "wb");

	header.ve = 0x38;
	header.rs = 0x39;
	header.ion = 0x61;

	descriptor.canvas_width = 0xFF;
	descriptor.canvas_width = 0x00;

	descriptor.canvas_height = 0xFF;
	descriptor.canvas_height = 0x00;
	descriptor.packed_field = 0x00; // TODO: write bit shifter for color table
	descriptor.pixel_aspect_ratio = 0x00;
	
	image_descriptor.image_width = 0xFF;
	image_descriptor.image_height = 0xFF;

	write_metadata(header, descriptor, image_descriptor, giffy);
	write_image_data(source, giffy);
	write_extensions(giffy, secret_message);

	//    T R A I L E R
	fputc(0x3B, giffy);

	fclose(giffy);

  return 0;
}
