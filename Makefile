
CC := gcc
OBJDIR := obj
SRCDIR := src
TSTDIR := test
LIBDIR := lib
BINDIR := bin
TGTNAME := nanodtypes
INCLUDES := -Iinclude

.DEFAULT_GOAL := $(BINDIR)/test_all

CFLAGS += $(INCLUDES)
CFLAGS += -Wall

OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))
TEST_OBJS = $(patsubst $(TSTDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(TSTDIR)/*.c))

$(TEST_OBJS) $(OBJS): | $(OBJDIR)

$(BINDIR)/test_all: | $(BINDIR)

$(LIBDIR)/$(TGTNAME).a: | $(LIBDIR)

$(OBJDIR) $(BINDIR) $(LIBDIR):
	mkdir $@

$(OBJDIR)/%_test.o: $(TSTDIR)/%_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIBDIR)/$(TGTNAME).a: $(OBJS)
	ar -r $@ $^

$(BINDIR)/test_all:  $(LIBDIR)/$(TGTNAME).a $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY : clean
clean:
	rm -f $(OBJDIR)/*.o $(LIBDIR)/*.a $(BINDIR)/test_all
