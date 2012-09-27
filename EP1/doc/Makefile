PDFC=pdflatex
RELATORIONAME=relatorio

AUXILIARTEX=texbase.tex

relatorio:
	$(PDFC) $(RELATORIONAME).tex

folderup: relatorio
	cp $(RELATORIONAME).pdf ..

.PHONY: clean
	
clean:
	rm -f $(RELATORIONAME).aux
	rm -f $(RELATORIONAME).log
	rm -f $(RELATORIONAME).pdf
	rm -f *~
	rm -f ../$(RELATORIONAME).pdf
