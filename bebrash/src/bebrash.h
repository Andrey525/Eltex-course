#pragma once

#define BEBRASH_TOK_BUFSIZE 64
#define BEBRASH_COMMAND_BUFSIZE 16
#define BEBRASH_TOK_DELIM " \t\r\n\a"
#define BEBRASH_COMMAND_DELIM "|"

void greeting();
void bebrash_loop();
char *bebrash_read_String();
char **bebrash_split_String_to_commands(char *String, int *count_commands);
char **bebrash_split_command(char *command);
int bebrash_execute(char **tokens);
int bebrash_launch(char ***tokens, int count_commands);

int bebrash_cd(char **tokens);
int bebrash_help(char **tokens);
int bebrash_exit(char **tokens);

int bebrash_num_builtins();
