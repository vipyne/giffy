// vanessa writes a gif maker

#include <stdio.h>

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

void write_metadata(gif_header header, gif_logical_screen_descriptor descriptor, gif_image_descriptor image_descriptor, FILE* giffy ) 
{
	printf("start write metadata");
	//    G I F
	fputc(47, giffy);
	fputc(49, giffy);
	fputc(46, giffy);
	fputc(header.ve, giffy);
	fputc(header.rs, giffy);
	fputc(header.ion, giffy);
	
	printf("middle write metadataaaaaa \n");
	fputc(descriptor.canvas_width, giffy);
	fputc(descriptor.canvas_height, giffy);
	fputc(descriptor.packed_field, giffy);
	fputc(descriptor.pixel_aspect_ratio, giffy);

	fputc(0x2C, giffy);
	fputc(0, giffy);
	fputc(0, giffy);// width
	fputc(0, giffy);
	fputc(0, giffy);// length
	fputc(image_descriptor.image_width, giffy);
	fputc(image_descriptor.image_height, giffy);
	fputc(0, giffy); // image_descriptor packed_field

	// image data

	
	fputc(0x21, giffy);// plain text extension start
	fputc(0x01, giffy);// plain text label
	fputc(0x00, giffy); // block size (let's try 0 for now :shrug_emoji:
	fputc(0x00, giffy); 

	// application extension
	fputc(33, giffy); // start ( 0x21 )
	fputc(255, giffy);
	fputc(11, giffy);
	fputc('N', giffy);
	fputc('E', giffy);
	fputc('T', giffy);
	fputc('S', giffy);
	fputc('C', giffy);
	fputc('P', giffy);
	fputc('E', giffy);
	fputc('2', giffy);
	fputc('.', giffy);
	fputc('0', giffy);
	fputc(3, giffy);
	fputc(1, giffy);
	fputc(18, giffy);
	fputc(0, giffy);
	
	fputc(0x21, giffy); // extension start
	fputc(0xFE, giffy); // comment label
	fputc(6, giffy);
	fputc('v', giffy);
	fputc('i', giffy);
	fputc('p', giffy);
	fputc('y', giffy);
	fputc('n', giffy);
	fputc('e', giffy);
	fputc(0, giffy);
}

int main(int argc, char* argv[])
{
	printf("hellrrr");
	FILE* giffy;

	gif_header header;
	gif_logical_screen_descriptor descriptor;
	gif_image_descriptor image_descriptor;

	giffy = fopen("myfirstgif.gif", "wb");

	header.ve = 38;
	header.rs = 37;
	header.ion = 61;

	descriptor.canvas_width = 100;
	descriptor.canvas_height = 100;
	descriptor.packed_field = 0; // TODO: write bit shifter for color table
	descriptor.pixel_aspect_ratio = 0;
	
	image_descriptor.image_width = 100;
	image_descriptor.image_height = 100;

	printf("hi there\n");
	write_metadata(header, descriptor, image_descriptor, giffy);


	fputc(0x3B, giffy);
	fclose(giffy);

  return 0;
}
