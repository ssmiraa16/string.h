CC=gcc
FLAGS= -Wall -Werror -Wextra -std=c11
GLFLAGS=--coverage
LIBNAME=s21_string.a
OS := $(shell uname)
FUNC_FILES_C= $(wildcard s21_string/s21_*.c)

TEST_DIR = s21_tests/

rebuild: clean all

all: s21_string.a gcov_report

test: s21_string.a
ifeq ($(OS), Linux)
	$(CC) $(FLAGS) $(TEST_DIR)*.c -L. s21_string.a -o s21_test -lcheck -lsubunit -lm 
else ifeq ($(OS), Darwin)
	$(CC) $(FLAGS) $(TEST_DIR)*.c -L. s21_string.a -o s21_test -lcheck
endif
	./s21_test

s21_string.a: 
	$(CC) $(FLAGS) -c $(FUNC_FILES_C) 
	ar rcs $(LIBNAME) *.o
	ranlib $(LIBNAME) s21_string.a


gcov_report: clean s21_string.a test
ifeq ($(OS), Linux)
	gcc --coverage  $(TEST_DIR)*.c $(FUNC_FILES_C)  -o s21_test -lcheck -lsubunit -lm 
else ifeq ($(OS), Darwin)
	gcc --coverage  $(TEST_DIR)*.c $(FUNC_FILES_C)  -o s21_test -lcheck
endif
	./s21_test
	lcov -t "s21_test" -o s21_test.info -c -d . && 	lcov --remove s21_test.info '/usr/local/lib/*' -o s21_test.info --ignore-errors unused
	genhtml -o report s21_test.info
	open ./report/index.html

clean:
	rm -rf *.o s21_string.a libs21_string.a $(TEST_DIR)s21_tests s21_test  *.gcda *.gcno report s21_test.info

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h $(TEST_DIR)*.c $(TEST_DIR)*.h