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
	short int canvas_width;
	short int canvas_length;
	char packed_field;
	char pixel_aspect_ratio;
} gif_logical_screen_descriptor; 


void write_metadata(gif_header header, gif_logical_screen_descriptor descriptor, FILE* giffy ) 
{
	printf("start write metadata");
	//    G I F
	fputc(47, giffy);
	fputc(49, giffy);
	fputc(46, giffy);
	fputc(header.ve, giffy);
	fputc(header.rs, giffy);
	fputc(header.ion, giffy);
	
	printf("middle write metadata");
	fputc(descriptor.canvas_width, giffy);
	fputc(descriptor.canvas_length, giffy);
	fputc(descriptor.packed_field, giffy);
	fputc(descriptor.pixel_aspect_ratio, giffy);


	//fputc(descriptor.canvas_width, giffy);
}


int main(int argc, char* argv[])
{
	printf("hellrrr");
	FILE* giffy;

	gif_header header;
	gif_logical_screen_descriptor descriptor;

	giffy = fopen("myfirstgif.gif", "wb");

	header.ve = 38;
	header.rs = 37;
	header.ion = 61;

	descriptor.canvas_width = 100;
	descriptor.canvas_length = 100;
	descriptor.packed_field = 0; // TODO: write bit shifter for color table
	descriptor.pixel_aspect_ratio = 0;
	
	printf("hi there");
	write_metadata(header, descriptor, giffy);

	fclose(giffy);

  return 0;
}
