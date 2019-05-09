all: my-bindiff

test: bindiff_test
	./bindiff_test

clean:
	-rm -rf my-bindiff bindiff_test

bindiff_test: test/test.o
	$(CC) -o $@ $^

.PHONY: all test clean
