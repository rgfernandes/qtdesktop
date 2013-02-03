#!/bin/sh
pyuic4 -o ui/Ui_main.py ui/main.ui
pyrcc4 -o pyqtarc_rc.py pyqtarc.qrc
