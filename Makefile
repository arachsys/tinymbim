BINDIR := $(PREFIX)/bin
CFLAGS := -Os -Wall -Wfatal-errors

mbim: *.[ch] Makefile
	$(CC) $(CFLAGS) -o $@ $(filter %.c,$^)

install: mbim
	mkdir -p $(DESTDIR)$(BINDIR)
	install -s $^ $(DESTDIR)$(BINDIR)

clean:
	rm -f mbim

.PHONY: clean install
