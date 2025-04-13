#include "tokenizer.h"


tok_context_t *start_def_ctx(char *source_name) {
	return start_context(source_name, 0, 0, 0);
}

tok_context_t *start_op_ctx(char *source_name, char *operators) {
	return start_context(source_name, operators, 0, 0);
}

tok_context_t *start_delim_ctx(char *source_name, char *delims, char escape) {
	return start_context(source_name, 0, delims, escape);
}

tok_context_t *start_context(char *source_name, char *operators, char * delims, char escape) {
	tok_context_t *context = malloc(sizeof(tok_context_t));
	context->loc = (source_loc_t){.source_name = source_name, .line = 1, .row = 1}; 
	context->operators = operators;
	context->delims = delims;
	context->index = 0;
	context->cur_str = 0;
	context->escape = escape;
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

bool is_oneof(char c, char *set) {
	if (!set) return false;
	for (int i = 0; set[i]; i++) {
		if (c == set[i]) return true; 
	}
	return false;
}

bool use_whitespace(char c, tok_context_t *context) {
		switch (c) {
			case ' ': 
			case '\t':
			context->loc.row++;
			context->index++;
			return true;
			
			case '\v': 
			case '\f':
			context->loc.line++;
			context->index++;
			return true;
			
			case '\n':
			context->loc.line++;
			case '\r':
			context->loc.row = 1;
			context->index++;
			return true;
		}
		return false;
}


void skip_whitespace(char *str, tok_context_t *context) {
	for (size_t i = context->index; str[i]; i++) 
		if (!use_whitespace(str[i], context)) break;
}


source_loc_t *copy_loc(tok_context_t *context) {
	source_loc_t *beginning = malloc(sizeof(source_loc_t));
	memcpy(beginning, &context->loc, sizeof(source_loc_t));
	return beginning;	
}


token_t *delim_tok(char *str, tok_context_t *context) {
	char delim = str[context->index++];
	char buffer[TOK_MAX_LEN];
	buffer[0] = delim;
	bool escaped = false;
	source_loc_t *beginning = copy_loc(context);
	context->loc.row++;
	size_t tok_i = 1;
	for (size_t i = context->index; str[i]; i++) {
		if (!escaped && str[i] == '\\') {
				escaped = true;
				context->index++;
				context->loc.row++;
		}
		else {	
			buffer[tok_i++] = str[i];
			if (!use_whitespace(str[i], context)) {
				context->index++;
				context->loc.row++;
				if (!escaped && str[i] == delim){
					break;
				
				}
			}
			escaped = false;
		}
	}
	return new_token(buffer, tok_i, beginning);
}


token_t *stoken(char *str, tok_context_t *context) {
	if (!str || !strlen(str)) return 0;
	if (context->cur_str != str) {
		context->cur_str = str;
		context->index = 0;
	}
	char buffer[TOK_MAX_LEN];
	size_t tok_i = 0;
	skip_whitespace(str, context);	
	source_loc_t *beginning = copy_loc(context);
	for (size_t i = context->index; str[i]; i++) {
		if (is_oneof(str[i], context->delims)) {
			if (tok_i) goto new_tok;
			else return delim_tok(str, context);	
		}	
		if (is_oneof(str[i], context->operators)) {
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
