

all:
	cmake -B build/ && make -C build/

clean: builddir
	rm -rf build

test: builddir
	cmake -B build/ && make -C build/ test

builddir:
#	mkdir -p build
