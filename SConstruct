#!/bin/env python
# -*- mode: python; coding: utf-8-sig -*-
#=======================================================================================================================
__author__ = 'hshibuya <goe@fuzz.co.jp>'

env = Environment()
env.Append(
    # CCFLAGS = ['/Od', '/Ob0', '/Zi'],
    CCFLAGS = ['/O2', '/Ob2', '/Zi'],
    LINKFLAGS = ['/DEBUG'],
    )
env.Program('rsa', Glob('*.cpp'))

