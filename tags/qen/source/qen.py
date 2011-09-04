#!/usr/bin/env python

"""
QEN:		Qt eGroupWare Notifier
Designed by Eugene A. Pivnev

File:	QEN.py
Purpose:	Entry point
"""

### Imports

__progname__ = 'QEN'
__version__ = '0.0.1'
__author__ = 'Eugene Pivnev'

translationPath = 'translations'

# QEN
import	Main


### Execution

# Event Loop
Main.aMain.exec_()
