#include "tokenizer.h"

int tests = 0;

void test_str(token_t *tok, char *string) {
	if (!tok) {
		fprintf(stderr, "Expected %s but got an empty token\n", string);
		exit(1);
	}
	else if (strcmp(tok->string, string)){
		fprintf(stderr, "Token has string %s but expected %s\n", tok->string, string);
		exit(1);
	}
	free_token(tok);
	tests++;
}


void test_full(token_t *tok, char * str, char *location) {
	if (!tok) {
		fprintf(stderr, "Expected a token but got null\n");
		exit(1);
	}
	char buffer[100];
	if (!format_loc(tok, buffer)) {
		fprintf(stderr, "Could not format location of token\n");
		exit(1);
	}
	if (strcmp(buffer, location)){
		fprintf(stderr, "Token %s has location `%s` but expected `%s`\n", tok->string, buffer, location);
		exit(1);
	}
	test_str(tok, str);
}




void test_null(token_t *tok) {
	if (tok) {
		fprintf(stderr, "Expected empty token but got %s\n", tok->string);
		exit(1);
	}
	tests++;
}

void test_basic() {
	char *str = "1+10 * 100+x";
	tok_context_t *ctx = start_context("test_basic", "+*");	
	test_str(stoken(str, ctx), "1");	
	test_str(stoken(str, ctx), "+");		
	test_str(stoken(str, ctx), "10");		
	test_str(stoken(str, ctx), "*");		
	test_str(stoken(str, ctx), "100");		
	test_str(stoken(str, ctx), "+");		
	test_str(stoken(str, ctx), "x");
	test_null(stoken(str, ctx));	
	free(ctx);	
}

void test1() {
	char *str = "(*(+ 10 20)2)";	
	tok_context_t *ctx = start_context("test1", "+*()");
	test_full(stoken(str, ctx), "(", "test1:1:1");
	test_full(stoken(str, ctx), "*", "test1:1:2");
	test_full(stoken(str, ctx), "(", "test1:1:3");
	test_full(stoken(str, ctx), "+", "test1:1:4");
	test_full(stoken(str, ctx), "10", "test1:1:6");
	test_full(stoken(str, ctx), "20", "test1:1:9");
	test_full(stoken(str, ctx), ")", "test1:1:11");
	test_full(stoken(str, ctx), "2", "test1:1:12");
	free(ctx);
}


int main() {
	test_basic();
	test1();
	printf("All %d tests passed\n", tests);
}
