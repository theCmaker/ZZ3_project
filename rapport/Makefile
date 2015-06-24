CC=pdflatex
CFLAGS=-shell-escape
CONFIG=settings/*.tex
TESTS=../tests/*/*
OBJS=titlepage.tex intro.tex conclusion.tex problemes.tex tests.tex
JOBNAME=compte_rendu

$(JOBNAME).pdf: $(JOBNAME).tex $(CONFIG) $(OBJS) $(TESTS)
	$(CC) $(CFLAGS) $< ;

clean: 
	rm -rf _minted-$(JOBNAME) *~ $(JOBNAME).aux $(JOBNAME).toc $(JOBNAME).log $(JOBNAME).bbl $(JOBNAME).blg $(JOBNAME).gz $(JOBNAME).out $(JOBNAME)-*.asy $(JOBNAME)-*.pdf $(JOBNAME).pre $(JOBNAME).vrb $(JOBNAME).snm $(JOBNAME).lof $(JOBNAME).lot
