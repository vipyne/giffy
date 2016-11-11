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
typedef struct {
	
} application_extension;

void write_metadata(gif_header header, gif_logical_screen_descriptor descriptor, gif_image_descriptor image_descriptor, FILE* giffy) 
{
	printf("start write metadata");
	//    G I F
	fputc(0x47, giffy);
	fputc(0x49, giffy);
	fputc(0x46, giffy);
	fputc(header.ve, giffy);
	fputc(header.rs, giffy);
	fputc(header.ion, giffy);
	
	printf("middle write metadataaaaaa \n");
	fputc(0xFF, giffy);
	fputc(0x00, giffy);//width
	fputc(0xFF, giffy);
	fputc(0x00, giffy);//height
	fputc(0x91, giffy);// packed
	fputc(0x00, giffy);// background color index
	fputc(0x00, giffy);//pixel aspect ratio
	//fputc(descriptor.canvas_width, giffy);
	//fputc(descriptor.canvas_height, giffy);
	//fputc(descriptor.packed_field, giffy);
	//fputc(descriptor.pixel_aspect_ratio, giffy);

	// global color table
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
	// end color table

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
std::cout << "i: " << i << "writing: " << std::endl;
		
	fputc(0x00, giffy); // end image data
}

void write_extensions(FILE* giffy)
{
	fputc(0x21, giffy);// plain text extension start
	fputc(0x01, giffy);// plain text label
	fputc(0x00, giffy); // block size (let's try 0 for now :shrug_emoji:
	fputc(0x00, giffy); 

	// application extension
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
	
	fputc(0x21, giffy); // extension start
	fputc(0xFE, giffy); // comment label
	fputc(0x06, giffy);
	fputc('v', giffy);
	fputc('i', giffy);
	fputc('p', giffy);
	fputc('y', giffy);
	fputc('n', giffy);
	fputc('e', giffy);
	fputc(0x00, giffy);
}

int main(int argc, char* argv[])
{
	printf("hellrrr");
	FILE* giffy;
	FILE* source;
	source = fopen(argv[1], "rb");

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

	printf("hi there\n");
	write_metadata(header, descriptor, image_descriptor, giffy);
	write_image_data(source, giffy);
	write_extensions(giffy);

	fputc(0x3B, giffy);
	fclose(giffy);

  return 0;
}
