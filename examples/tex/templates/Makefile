CC=pdflatex
CFLAGS=-shell-escape
CONFIG=settings/*.tex
OBJS=../tikz-content.tex
OBJS2=../tikz-graph.tex
JOBNAME=tikz_spatial_graph
JOBNAME2=tikz_temporal_graph

spatial: $(JOBNAME).pdf

temporal: $(JOBNAME2).pdf

$(JOBNAME).pdf: $(JOBNAME).tex $(CONFIG) $(OBJS)
	$(CC) $(CFLAGS) $< ;

$(JOBNAME2).pdf: $(JOBNAME2).tex $(CONFIG) $(OBJS2)
	$(CC) $(CFLAGS) $< ;

clean: 
	rm -rf _minted-$(JOBNAME) *~ $(JOBNAME).aux $(JOBNAME).toc $(JOBNAME).log $(JOBNAME).bbl $(JOBNAME).blg $(JOBNAME).gz $(JOBNAME).out $(JOBNAME)-*.asy $(JOBNAME)-*.pdf $(JOBNAME).pre $(JOBNAME).vrb $(JOBNAME).snm $(JOBNAME).lof $(JOBNAME).lot $(JOBNAME).lol
	rm -rf _minted-$(JOBNAME2) *~ $(JOBNAME2).aux $(JOBNAME2).toc $(JOBNAME2).log $(JOBNAME2).bbl $(JOBNAME2).blg $(JOBNAME2).gz $(JOBNAME2).out $(JOBNAME2)-*.asy $(JOBNAME2)-*.pdf $(JOBNAME2).pre $(JOBNAME2).vrb $(JOBNAME2).snm $(JOBNAME2).lof $(JOBNAME2).lot $(JOBNAME2).lol
