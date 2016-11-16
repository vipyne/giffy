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
	fseek(giffy, -1L, SEEK_END); // rewind and start to write over original trailer

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