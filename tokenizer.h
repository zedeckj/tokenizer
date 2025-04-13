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
	size_t col;
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
	void *pntr;
	char escape;
	char *operators;
	char *delims;
} tok_context_t;


// Formats the source location of the given token into the form
// <source-name>:<line>:<col>, ex. "stdin:2:5"
int format_loc(token_t * token, char *buffer);

// Mallocs a tokenizing context with no special conditions
tok_context_t *start_def_ctx(char *source_name);

// Mallocs a tokenizing context where the operators in the speicifed
// string will be seperated out
tok_context_t *start_op_ctx(char *source_name, char *operators);

// Mallocs a tokenizing context where the delimiters in the given string
// can be used to signify a single token, and can be escaped from within 
// with the provided escape character.
tok_context_t *start_delim_ctx(char *source_name, char *delims, char escape);

// Mallocs a tokenizing context with all given paramters. A value of 0 can be used
// for any argument after source_name that should be ignored
tok_context_t *start_context(char *source_name, char * operators, char *delims, char escape);

// Mallocs a new token from the given string
token_t *stoken(char *str, tok_context_t *context);

// Mallocs a new token from the given file
token_t *ftoken(FILE *file, tok_context_t *context);

// Frees the given token
void free_token(token_t *token);

#endif
