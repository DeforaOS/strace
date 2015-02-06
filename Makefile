PACKAGE	= strace
VERSION	= 0.0.0
SUBDIRS	= src
RM	= rm -f
LN	= ln -f
TAR	= tar
MKDIR	= mkdir -m 0755 -p


all: subdirs

subdirs:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		([ -d "$(OBJDIR)$$i" ] || $(MKDIR) -- "$(OBJDIR)$$i") && \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/"; \
		else $(MAKE); fi) || exit; done

clean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) clean) || exit; done

distclean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) distclean) || exit; done

dist:
	$(RM) -r -- $(OBJDIR)$(PACKAGE)-$(VERSION)
	$(LN) -s -- "$$PWD" $(OBJDIR)$(PACKAGE)-$(VERSION)
	@cd $(OBJDIR). && $(TAR) -czvf $(OBJDIR)$(PACKAGE)-$(VERSION).tar.gz -- \
		$(PACKAGE)-$(VERSION)/src/platform/freebsd.c \
		$(PACKAGE)-$(VERSION)/src/platform/linux.c \
		$(PACKAGE)-$(VERSION)/src/platform/netbsd.c \
		$(PACKAGE)-$(VERSION)/src/strace.c \
		$(PACKAGE)-$(VERSION)/src/main.c \
		$(PACKAGE)-$(VERSION)/src/Makefile \
		$(PACKAGE)-$(VERSION)/src/platform.h \
		$(PACKAGE)-$(VERSION)/src/platform/freebsd.h \
		$(PACKAGE)-$(VERSION)/src/platform/linux.h \
		$(PACKAGE)-$(VERSION)/src/platform/netbsd.h \
		$(PACKAGE)-$(VERSION)/src/strace.h \
		$(PACKAGE)-$(VERSION)/src/project.conf \
		$(PACKAGE)-$(VERSION)/Makefile \
		$(PACKAGE)-$(VERSION)/COPYING \
		$(PACKAGE)-$(VERSION)/config.h \
		$(PACKAGE)-$(VERSION)/project.conf
	$(RM) -- $(OBJDIR)$(PACKAGE)-$(VERSION)

distcheck: dist
	$(TAR) -xzvf $(OBJDIR)$(PACKAGE)-$(VERSION).tar.gz
	$(MKDIR) -- $(PACKAGE)-$(VERSION)/objdir
	$(MKDIR) -- $(PACKAGE)-$(VERSION)/destdir
	(cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/")
	(cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/" DESTDIR="$$PWD/destdir" install)
	(cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/" DESTDIR="$$PWD/destdir" uninstall)
	(cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/" distclean)
	(cd "$(PACKAGE)-$(VERSION)" && $(MAKE) dist)
	$(RM) -r -- $(PACKAGE)-$(VERSION)

install:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) install) || exit; done

uninstall:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) uninstall) || exit; done

.PHONY: all subdirs clean distclean dist distcheck install uninstall
