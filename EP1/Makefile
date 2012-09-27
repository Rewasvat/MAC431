CFLAGS= -Wall -ansi -pedantic -march=native
SRCDIR=src
BINDIR=bin

FOLDERENTREGA=$(EXEC)-diogo-e-fernando
PACOTENAME=$(FOLDERENTREGA).tar.gz

OBJECTS=$(BINDIR)/fase1.o $(BINDIR)/fase2.o $(BINDIR)/fase3.o
EXECS=fase1 fase2 fase3
CC=gcc

all: $(BINDIR) $(EXECS)

fase1: $(BINDIR) $(BINDIR)/fase1.o
	$(CC) $(CFLAGS) $(BINDIR)/fase1.o -o fase1

fase2: $(BINDIR) $(BINDIR)/fase2.o
	$(CC) $(CFLAGS) $(BINDIR)/fase2.o -o fase2 

fase3: $(BINDIR) $(BINDIR)/fase3.o
	$(CC) $(CFLAGS) $(BINDIR)/fase3.o -o fase3 

# OBJECTS

$(BINDIR)/fase1.o: $(BINDIR) $(SRCDIR)/fase1.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/fase1.c -o $(BINDIR)/fase1.o

$(BINDIR)/fase2.o: $(BINDIR) $(SRCDIR)/fase2.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/fase2.c -o $(BINDIR)/fase2.o

$(BINDIR)/fase3.o: $(BINDIR) $(SRCDIR)/fase3.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/fase3.c -o $(BINDIR)/fase3.o



$(BINDIR):
	if [ ! -d $(BINDIR) ]; then mkdir $(BINDIR); fi

$(RELATORIODIR):
	if [ ! -d $(RELATORIODIR) ]; then mkdir $(RELATORIODIR); fi

pdf: $(RELATORIODIR)
	cd $(RELATORIODIR); make folderup

entrega: clean $(EXECS) pdf
	rm -f $(PACOTENAME)
	rm -rf $(FOLDERENTREGA)
	mkdir $(FOLDERENTREGA)
	cp -r src $(FOLDERENTREGA)
	cp README $(FOLDERENTREGA)
	cp relatorio.pdf $(FOLDERENTREGA)
	cp Makefile $(FOLDERENTREGA)
	tar -czf $(PACOTENAME) $(FOLDERENTREGA)
	rm -rf $(FOLDERENTREGA)

.PHONY: clean
clean:
	rm -f $(BINDIR)/*
	rm -f *~
	rm -f $(SRCDIR)/*~
	rm -f $(EXECS)
	rm -f $(PACOTENAME)
	rm -rf $(FOLDERENTREGA)
	cd $(RELATORIODIR); make clean