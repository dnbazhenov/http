

all:
	cmake -B build/ && make -C build/

clean: builddir
	rm -rf build

test: builddir
	cmake -B build/ && ARGS=-V make -C build/ test

clang-format:
	find include -name *.h -exec clang-format -i '{}' \;
	find httputil -name *.cpp -exec clang-format -i '{}' \;

builddir:
#	mkdir -p build
