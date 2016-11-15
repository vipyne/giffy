struct gif_image_descriptor {
  char start_image_descriptor;
  short int x_start;
  short int y_start;
  short int image_width;
  short int image_height;
  char packed_field;
};

struct key_value_ll {
  int key;
  int value[100];
  struct key_value_ll* next;
};

void write_secret_message_symbols(FILE* giffy);
void write_comment_end(FILE* giffy);
void write_header(FILE* giffy);
void write_image_data(FILE* source, FILE* giffy);
int long write_secret_message(FILE* giffy, char* secret_message);
void write_extensions(FILE* giffy, char* secret_message);
int verb_hash(char* key_color_index, int buff_idx, int hash_table_length);
void generate_index_stream(FILE* source, char* index_stream);

void compress_image(FILE* giffy, char* index_stream, struct key_value_ll* dictionary_hash);
void init_dictionary(struct key_value_ll* dictionary_hash);
// void read_input_file(FILE* source);

void parse_out_secret_message(FILE* source);
