TARGETS	= $(OBJDIR)strace
OBJDIR	=
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

strace_OBJS = $(OBJDIR)platform/freebsd.o $(OBJDIR)platform/linux.o $(OBJDIR)platform/netbsd.o $(OBJDIR)strace.o $(OBJDIR)main.o
strace_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
strace_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)strace: $(strace_OBJS)
	$(CC) -o $(OBJDIR)strace $(strace_OBJS) $(strace_LDFLAGS)

$(OBJDIR)platform/freebsd.o: platform/freebsd.c platform/freebsd.h platform.h
	@[ -d "$(OBJDIR)platform" ] || $(MKDIR) -- "$(OBJDIR)platform"
	$(CC) $(strace_CFLAGS) -o $(OBJDIR)platform/freebsd.o -c platform/freebsd.c

$(OBJDIR)platform/linux.o: platform/linux.c platform/linux.h platform.h
	@[ -d "$(OBJDIR)platform" ] || $(MKDIR) -- "$(OBJDIR)platform"
	$(CC) $(strace_CFLAGS) -o $(OBJDIR)platform/linux.o -c platform/linux.c

$(OBJDIR)platform/netbsd.o: platform/netbsd.c platform/netbsd.h platform.h
	@[ -d "$(OBJDIR)platform" ] || $(MKDIR) -- "$(OBJDIR)platform"
	$(CC) $(strace_CFLAGS) -o $(OBJDIR)platform/netbsd.o -c platform/netbsd.c

$(OBJDIR)strace.o: strace.c platform.h strace.h ../config.h
	$(CC) $(strace_CFLAGS) -o $(OBJDIR)strace.o -c strace.c

$(OBJDIR)main.o: main.c strace.h ../config.h
	$(CC) $(strace_CFLAGS) -o $(OBJDIR)main.o -c main.c

clean:
	$(RM) -- $(strace_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(OBJDIR)strace $(DESTDIR)$(BINDIR)/strace

uninstall:
	$(RM) -- $(DESTDIR)$(BINDIR)/strace

.PHONY: all clean distclean install uninstall
