bin_PROGRAMS = cpareia

cpareia_SOURCES = args.c args.h array.c array.h block.c block.h blocking.c   \
                  blocking.h classifier.c classifier.h colors.c colors.h     \
                  comparator.c comparator.h conjunction.c conjunction.h      \
                  cpareia.c cpareia.h csv.c csv.h database.c database.h      \
                  errors.c errors.h field_comparator.c field_comparator.h    \
                  hash.c hash.h khash.h kvec.h Makefile.am output.c output.h \
                  project.c project.h record.c record.h uint_array.c         \
                  uint_array.h buscabr.cc buscabr.h

cpareia_LDADD   = $(ZLIB_LIBS) $(LIBXML_LIBS) $(TCMALLOC_LIBS)
cpareia_CFLAGS  = $(ZLIB_CFLAGS) $(LIBXML_CFLAGS) $(TCMALLOC_CFLAGS)
cpareia_LDFLAGS =

if TCMALLOC
cpareia_LDFLAGS += -fno-builtin-malloc -fno-builtin-calloc \
                   -fno-builtin-realloc -fno-builtin-free
endif

if SSE2
cpareia_LDFLAGS += -msse2
cpareia_CFLAGS  += -DHAVE_SSE2
endif
