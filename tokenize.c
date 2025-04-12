#include "tokenizer.h"

tok_context_t *start_context(char *source_name, char *operators) {
	tok_context_t *context = malloc(sizeof(tok_context_t));
	context->loc = (source_loc_t){.source_name = source_name, .line = 1, .row = 1}; 
	context->operators = operators;
	context->index = 0;
	return context;
}

int format_loc(token_t * token, char *buffer){
	if (!token || !buffer) return 0;
	source_loc_t *loc = token->location;
	if (!loc) return 0;
	return sprintf(buffer, "%s:%ld:%ld", loc->source_name, loc->line, loc->row);
}

token_t *new_token(char *buffer, size_t len, source_loc_t *current_loc) {
	if (len == 0) return 0;
	buffer[len] = 0;
	token_t * tok = malloc(sizeof(token_t));
	tok->string = malloc(len + 1);
	tok->location = current_loc;
	strcpy(tok->string, buffer);
	return tok;
}

void free_token(token_t *token) {
	free(token->string);
	free(token->location);
}

bool is_operator(char c, char *operators) {
	for (int i = 0; operators[i]; i++) {
		if (c == operators[i]) return true; 
	}
	return false;
}

void skip_whitespace(char *str, tok_context_t *context) {
	for (size_t i = context->index; str[i]; i++) {
		context->index++;
		switch (str[i]) {
			case ' ': case '\t':
			context->loc.row++;
			break;
			case '\v': case '\f':
			context->loc.line++;
			break;
			case '\n':
			context->loc.line++;
			case '\r':
			context->loc.row = 1;
			break;
			default:
			context->index--;
			return;
		}
	}
}

token_t *stoken(char *str, tok_context_t *context) {
	if (!str || !strlen(str)) return 0;
	char buffer[TOK_MAX_LEN];
	size_t tok_i = 0;
	skip_whitespace(str, context);	
	source_loc_t *beginning = malloc(sizeof(source_loc_t));
	memcpy(beginning, &context->loc, sizeof(source_loc_t));
	for (size_t i = context->index; str[i]; i++) {
		if (is_operator(str[i], context->operators)) {
			if (tok_i) goto new_tok;
			else {
				context->index++;
				context->loc.row++;
				buffer[0] = str[i];
				tok_i = 1;
				goto new_tok;
			}
		}	
		else if (isspace(str[i])) goto new_tok;
		else {
			context->index++;
			context->loc.row ++;			
			buffer[tok_i++] = str[i];
		}
		
	}
	new_tok: {
		token_t *out = new_token(buffer, tok_i, beginning);
		return out;
	}
}
