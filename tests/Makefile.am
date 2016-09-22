TESTS = test_database test_record test_args

check_PROGRAMS = $(TESTS)

test_database_SOURCES = test_database.c test_database.h \
                        ../src/database.h ../src/database.c \
                        ../src/record.h ../src/record.c \
                        ../src/array.h ../src/array.c \
                        ../src/errors.h ../src/errors.c
test_database_LDADD   = $(CMOCKA_LIBS)
test_database_CFLAGS  = $(CMOCKA_CFLAGS)

test_record_SOURCES = test_record.c test_record.h \
                      ../src/record.h ../src/record.c \
                      ../src/array.h ../src/array.c \
                      ../src/errors.h ../src/errors.c
test_record_LDADD   = $(CMOCKA_LIBS)
test_record_CFLAGS  = $(CMOCKA_CFLAGS)

test_args_SOURCES = test_args.c test_args.h \
                    ../src/args.h ../src/args.c \
                    ../src/errors.h ../src/errors.c
test_args_LDADD   = $(CMOCKA_LIBS)
test_args_CFLAGS  = $(CMOCKA_CFLAGS)
