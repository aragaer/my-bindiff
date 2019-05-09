CFLAGS += -Wall -Werror

OFILES = bindiff.o
TEST_OFILES = test/test.o

all: my-bindiff

test: bindiff_test
	./bindiff_test

behave: my-bindiff
	behave

clean:
	-rm -rf my-bindiff bindiff_test $(OFILES) $(TEST_OFILES)

bindiff_test: $(TEST_OFILES)

my-bindiff: $(OFILES)

my-bindiff bindiff_test:
	$(CC) -o $@ $^

.PHONY: all test clean behave
