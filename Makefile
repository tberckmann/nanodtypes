
# Usage:

# make                     (build library and tests)
# make DEBUG=true          (build library and tests with debug flags)
# make coverage            (build tests and check code coverage)
# make clean               (remove all artifacts)

CC := gnu-gcc
GCOV := gnu-gcov
OBJDIR := obj
SRCDIR := src
TSTDIR := test
LIBDIR := lib
BINDIR := bin
COVDIR := coverage
TGTNAME := nanodtypes
INCLUDES := -Iinclude
DEBUG := false
COV_MARK := cov

.DEFAULT_GOAL := $(BINDIR)/test_all
.PHONY : clean coverage

CFLAGS += $(INCLUDES)
CFLAGS += -Wall -ansi -Wpedantic -Werror

ifeq ($(DEBUG), true)
CFLAGS += -g
endif

COVFLAGS += --coverage

OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))
COV_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.$(COV_MARK).o,$(wildcard $(SRCDIR)/*.c))
TEST_OBJS = $(patsubst $(TSTDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(TSTDIR)/*.c))

$(TEST_OBJS) $(OBJS): | $(OBJDIR)

$(BINDIR)/test_all: | $(BINDIR)

$(LIBDIR)/$(TGTNAME).a: | $(LIBDIR)

$(OBJDIR) $(BINDIR) $(LIBDIR):
	mkdir $@

$(OBJDIR)/%_test.o: $(TSTDIR)/%_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.$(COV_MARK).o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(COVFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIBDIR)/$(TGTNAME).$(COV_MARK).a: $(COV_OBJS)
	ar -r $@ $^

$(LIBDIR)/$(TGTNAME).a: $(OBJS)
	ar -r $@ $^

$(BINDIR)/test_$(COV_MARK):  $(LIBDIR)/$(TGTNAME).$(COV_MARK).a $(TEST_OBJS)
	$(CC) $(CFLAGS) $(COVFLAGS) -o $@ $^

$(BINDIR)/test_all:  $(LIBDIR)/$(TGTNAME).a $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

coverage: $(BINDIR)/test_$(COV_MARK)
	$<
	sh util/coverage.sh $(GCOV) $(COVDIR)

clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/*.gcda $(OBJDIR)/*.gcno
	rm -f $(LIBDIR)/*.a $(BINDIR)/test_all
	rm -f $(COVDIR)/*.gcov
