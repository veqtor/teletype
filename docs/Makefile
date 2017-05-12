.PHONY: build clean

build: teletype.pdf teletype.html

clean:
	rm -f teletype.pdf && rm -f teletype.html

teletype.pdf: $(wildcard *.md ops/*.md ops/*.toml)
	../utils/docs.py teletype.pdf

teletype.html: $(wildcard *.md ops/*.md ops/*.toml)
	../utils/docs.py teletype.html
