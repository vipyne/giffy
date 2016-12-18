void write_secret_message_symbols(FILE* giffy);
void write_comment_end(FILE* giffy);
void write_header(FILE* giffy);
void copy_gif_file(FILE* source, FILE* giffy);
int long write_secret_message(FILE* giffy, char* secret_message);
void write_entire_comment(FILE* giffy, char* secret_message);
void parse_out_secret_message(FILE* source);
void print_directions(void);
void write_compressed_image_data(FILE* source, FILE* giffy);