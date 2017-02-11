all: run

clean:
	@rm test/main
build:
	@g++ -Ilib/Note test/main.cpp -o test/main;

run: build
	@test/main

.PHONY: run clean build
