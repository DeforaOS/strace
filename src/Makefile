TARGETS	= strace
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF=
CPPFLAGS=
CFLAGSF	= -W
CFLAGS	= -Wall -g -O2 -pedantic
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

strace_OBJS = freebsd.o linux.o netbsd.o strace.o
strace_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
strace_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

strace: $(strace_OBJS)
	$(CC) -o strace $(strace_OBJS) $(strace_LDFLAGS)

freebsd.o: freebsd.c
	$(CC) $(strace_CFLAGS) -c freebsd.c

linux.o: linux.c linux.h
	$(CC) $(strace_CFLAGS) -c linux.c

netbsd.o: netbsd.c netbsd.h
	$(CC) $(strace_CFLAGS) -c netbsd.c

strace.o: strace.c freebsd.h linux.h netbsd.h
	$(CC) $(strace_CFLAGS) -c strace.c

clean:
	$(RM) -- $(strace_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 strace $(DESTDIR)$(BINDIR)/strace

uninstall:
	$(RM) -- $(DESTDIR)$(BINDIR)/strace

.PHONY: all clean distclean install uninstall
