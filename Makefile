all: 
	make -C build
test:
	make test -C build

.PHONY: test all
