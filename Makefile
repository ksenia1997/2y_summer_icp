# Course ICP @ FIT VUT Brno, 2018
# Blockeditor
# xhasda00 - xbolsh00

PROJ = blockeditor
PACK_ZIP = xhasda00-xbolsh00.zip

.PHONY: run doxygen pack clean

src/$(PROJ): src/Makefile
	$(MAKE) -C src/

src/Makefile: src/blockeditor.pro
	qmake src/blockeditor.pro -o src/Makefile

run: src/$(PROJ)
	src/$(PROJ)

doxygen: src/Doxyfile
	doxygen src/Doxyfile

pack: clean src/ doc/ examples/ README.txt Makefile
	zip -r $(PACK_ZIP) src/ doc/ examples/ README.txt Makefile

clean: src/Makefile
	rm -rf doc/*
	$(MAKE) clean -C src/
	rm -f src/$(PROJ)
	rm -f src/Makefile

