### tokenize.h

Provides an interface for generating tokens from a string, allowing for the specification
of operators and the inclusion of source information, allowing for better error generation when parsing

``` C
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
```
