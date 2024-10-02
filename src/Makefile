.PHONY: all install uninstall clean dvi dist tests check memcheck test
cpp = g++
CFLAGS = -Wall -Werror -Wextra -std=c++17
TESTFLAGS := -lgtest -pthread -lgtest_main
OS = $(shell uname -s)
SOURCES = model/*.cc  controller/*.cc
BUILD_DIR = build
all: install open

clean:
	rm -rf view/*.o *.gcda *.gcno
	rm -rf coverage.info
	rm -rf report
	rm -rf $(BUILD_DIR)

rebuild: clean all

tests: clean
	$(cpp) $(CFLAGS) $(SOURCES) tests/tests.cc --coverage $(TESTFLAGS) -o test
	 ./test
test: tests

memcheck: test
ifeq ($(OS), Darwin)
	leaks -atExit -- ./test
else
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./test
endif

report: test
		@lcov --capture  --no-external --directory . --output-file coverage.info
		@genhtml coverage.info --output-directory report
		@open report/index.html

check:
	cp ../materials/linters/.clang-format ./
	clang-format -n ./*/*.cc
	clang-format -n ./*/*.h
	rm -rf .clang-format

build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..
	cd $(BUILD_DIR) && cmake --build . --target CPP4_3D_Viewer_V2_0-1

install: build
	mkdir -p $(BUILD_DIR)/bin
	cp -r $(BUILD_DIR)/CPP4_3D_Viewer_V2_0-1 $(BUILD_DIR)/bin

open:
	./build/view

uninstall:
	rm -rf build*

dist:
	rm -rf Archive_3D_Viewer_v.2.0/
	mkdir Archive_3D_Viewer_v.2.0/
	mkdir Archive_3D_Viewer_v.2.0/src
	mv ./build/viewer.app Archive_3D_Viewer_v.2.0/src/
	tar cvzf Archive_3D_Viewer_v.2.0.tgz Archive_3D_Viewer_v.2.0/
	rm -rf Archive_3D_Viewer_v.2.0/

dvi:
	open readme_viewer.md
