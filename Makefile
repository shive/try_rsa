# -*- mode: makefile-gmake; coding: euc-jp-unix -*-
#==============================================================================
## @author hshibuya <goe@fuzz.co.jp>

.PHONY: run build clean keygen

run: build
	./rsa

build:
	scons -Q

keygen:
	python keygen.py

clean:
	scons -Qc

