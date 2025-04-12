#ifndef JORB_TOKENIZER_H
#define JORB_TOKENIZER_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define TOK_MAX_LEN 1000

typedef struct {
	char *source_name;
	size_t line;
	size_t row;
} source_loc_t;


typedef struct {
	source_loc_t *location;
	char *string;
} token_t;


typedef struct tokens {
	token_t first;
	struct tokens *rest;
} token_list_t; 

typedef struct {
	source_loc_t loc;
	size_t index;
	char *operators;
} tok_context_t;

int format_loc(token_t * token, char *buffer);

tok_context_t *start_context(char *source_name, char *operators);


// Mallocs a new token from the given string
token_t *stoken(char *str, tok_context_t *context);

void free_token(token_t *token);

#endif
