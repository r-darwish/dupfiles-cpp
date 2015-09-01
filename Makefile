.PHONY: build
build:
	mkdir -p build
	cd build && CC=$(CC) CXX=$(CXX) cmake -G Ninja ..
	cmake --build build

.PHONY: clean
clean:
	rm -rf build
