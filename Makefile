BINDIR := $(PREFIX)/bin
CFLAGS := -Os -Wall -Wfatal-errors

mbim: *.[ch] Makefile
	$(CC) $(CFLAGS) -o $@ $(filter %.c,$^)

install: mbim wwan
	mkdir -p $(DESTDIR)$(BINDIR)
	install -s mbim $(DESTDIR)$(BINDIR)
	install wwan $(DESTDIR)$(BINDIR)

clean:
	rm -f mbim

.PHONY: clean install
