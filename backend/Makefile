all: build

build-deps:
	@esy install

server:
	@esy start

build:
	@esy build

test:
	@esy dune runtest

clean:
	@esy dune clean

.PHONY: all test build-deps clean
