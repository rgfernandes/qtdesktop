#!/bin/env python
# -*- coding: utf-8 -*-

import os, sys

for root, dirs, files in os.walk(sys.argv[1]):
	print root
	print dirs
	print files