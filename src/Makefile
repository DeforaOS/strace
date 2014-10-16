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

strace_OBJS = platform/freebsd.o platform/linux.o platform/netbsd.o strace.o main.o
strace_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
strace_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

strace: $(strace_OBJS)
	$(CC) -o strace $(strace_OBJS) $(strace_LDFLAGS)

platform/freebsd.o: platform/freebsd.c platform/freebsd.h platform.h
	$(CC) $(strace_CFLAGS) -o platform/freebsd.o -c platform/freebsd.c

platform/linux.o: platform/linux.c platform/linux.h platform.h
	$(CC) $(strace_CFLAGS) -o platform/linux.o -c platform/linux.c

platform/netbsd.o: platform/netbsd.c platform/netbsd.h platform.h
	$(CC) $(strace_CFLAGS) -o platform/netbsd.o -c platform/netbsd.c

strace.o: strace.c platform.h strace.h ../config.h
	$(CC) $(strace_CFLAGS) -c strace.c

main.o: main.c strace.h ../config.h
	$(CC) $(strace_CFLAGS) -c main.c

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
