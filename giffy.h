struct gif_image_descriptor {
  char start_image_descriptor;
  short int x_start;
  short int y_start;
  short int image_width;
  short int image_height;
  char packed_field;
};


void write_secret_message_symbols(FILE* giffy);
void write_comment_end(FILE* giffy);
void write_header(FILE* giffy);
void write_image_data(FILE* source, FILE* giffy);
int long write_secret_message(FILE* giffy, char* secret_message);
void write_extensions(FILE* giffy, char* secret_message);

// void read_input_file(FILE* source);

void parse_out_secret_message(FILE* source);
