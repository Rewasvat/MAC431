CFLAGS= -Wall -ansi -pedantic -march=native
OPENMP= -fopenmp
PTHREADS= -lpthread
SRCDIR=src
BINDIR=bin
RELATORIODIR=doc

FOLDERENTREGA=FernandoJefferson
PACOTENAME=$(FOLDERENTREGA).tar.gz

OBJECTS=$(BINDIR)/fase1.o $(BINDIR)/fase2.o $(BINDIR)/fase3.o
EXECS=fase1 fase2 fase3
CC=gcc

all: $(BINDIR) $(EXECS)

fase1: $(BINDIR) $(BINDIR)/fase1.o
	$(CC) $(CFLAGS) $(OPENMP) $(BINDIR)/fase1.o -o fase1

fase2: $(BINDIR) $(BINDIR)/fase2.o
	$(CC) $(CFLAGS) $(OPENMP) $(BINDIR)/fase2.o -o fase2 

fase3: $(BINDIR) $(BINDIR)/fase3.o
	$(CC) $(CFLAGS) $(PTHREADS) $(BINDIR)/fase3.o -o fase3 

# OBJECTS

$(BINDIR)/fase1.o: $(BINDIR) $(SRCDIR)/fase1.c
	$(CC) $(CFLAGS) $(OPENMP) -c $(SRCDIR)/fase1.c -o $(BINDIR)/fase1.o

$(BINDIR)/fase2.o: $(BINDIR) $(SRCDIR)/fase2.c
	$(CC) $(CFLAGS) $(OPENMP) -c $(SRCDIR)/fase2.c -o $(BINDIR)/fase2.o

$(BINDIR)/fase3.o: $(BINDIR) $(SRCDIR)/fase3.c
	$(CC) $(CFLAGS) $(PTHREADS) -c $(SRCDIR)/fase3.c -o $(BINDIR)/fase3.o



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
