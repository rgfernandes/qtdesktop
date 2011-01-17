NAME	= qen
VERSION = 0.0.1
RELEASE = 0

FULLNAME=$(NAME)-$(VERSION)

prefix	= /usr
datadir = $(prefix)/share
bindir	= $(prefix)/bin
DESTDIR	=

TMPDIR	= /tmp

SUBDIRS = doc source translations ui ui/icons/actions ui/icons/apps ui/icons/mimetypes

FILES = \
	doc/ChangeLog \
	doc/README \
	doc/INSTALL \
	doc/AUTHORS \
	doc/TODO \
	doc/NEWS \
	doc/ISSUES \
	doc/COPYING \
	source/Display.py \
	source/Input.py \
	source/egroupware.py \
	source/Main.py \
	source/Import.py \
	source/qen.py \
	source/Help.py \
	source/Settings.py \
	source/Network.py \
	source/Thread.py \
	translations/_ru.qm \
	translations/_ru.ts \
	ui/Settings.ui \
	ui/Settings.qrc \
	ui/icons/actions/button_ok.png \
	ui/icons/actions/button_cancel.png \
	ui/icons/actions/exit.png \
	ui/icons/actions/led-green.png \
	ui/icons/actions/led-red.png \
	ui/icons/mimetypes/txt.png \
	ui/icons/apps/Quamachi.png \
	ui/icons/apps/pyqt-logo.png \
	ui/icons/apps/ckhome2.png \
	ui/icons/apps/icon-clipboard.png \
	ui/icons/apps/egw.png \
	ui/icons/apps/python-logo.png \
	ui/icons/apps/gnu-head-sm.jpg \
	ui/icons/apps/infolog.png \
	ui/icons/apps/qt4-logo.png \
	ui/icons/apps/package_settings.png \
	ui/Main.ui \
	ui/Main.qrc \
	Makefile \
	install.py \
	uninstall.py \
	QEN.e3p \
	QEN.e3t \
	qen.desktop \
	qen.png \
	qen.spec.in

all:

pkg-all: tar
	rpmbuild -ta --nodeps $(FULLNAME).tar.bz2

pkg-src: tar
	rpmbuild -ts --nodeps $(FULLNAME).tar.bz2

pkg-bin: tar
	rpmbuild -tb --nodeps $(FULLNAME).tar.bz2

tar:
	rm -rf $(TMPDIR)/$(FULLNAME)
	for d in $(SUBDIRS); do mkdir -p $(TMPDIR)/$(FULLNAME)/$$d; done
	for f in $(FILES); do cp -p $$f $(TMPDIR)/$(FULLNAME)/$$f; done
	sed -e "s/^Version:.*$$/Version: $(VERSION)/" 			\
	-e "s/^Release:.*$$/Release: $(RELEASE)/" 			\
	< $(TMPDIR)/$(FULLNAME)/qen.spec.in 			\
	> $(TMPDIR)/$(FULLNAME)/qen.spec
	cat doc/ChangeLog >> $(TMPDIR)/$(FULLNAME)/qen.spec
	cd $(TMPDIR); tar cjvfp $(FULLNAME).tar.bz2 $(FULLNAME)
	mv $(TMPDIR)/$(FULLNAME).tar.bz2 .
	rm -rf $(TMPDIR)/$(FULLNAME)

clean:
	rm -f \
		source/Main_rc* \
		source/Settings_rc* \
		source/Ui_* \
		source/*.pyc \
		source/*.pyo \
		ui/*.py

fclean: clean
	-rm -f $(FULLNAME).tar.bz2

install:
	install -d -m 0755 $(DESTDIR)$(datadir)/$(NAME)/package_mgr
	install -p -m 0644 src/*.py $(DESTDIR)$(datadir)/$(NAME)/
	install -p -m 0644 src/package_mgr/*.py $(DESTDIR)$(datadir)/$(NAME)/package_mgr/
	install -p -m 0755 src/rpm-analyzer.py $(DESTDIR)$(datadir)/$(NAME)/rpm-analyzer.py
	install -Dp -m 0644 man/rpm-analyzer.1 $(DESTDIR)$(mandir)/man1/rpm-analyzer.1
	install -d -m 0755 $(DESTDIR)$(bindir)
	ln -sf $(datadir)/rpm-analyzer/rpm-analyzer.py $(DESTDIR)/$(bindir)/$(NAME)

compile:
	source/Ui_Main.py \
	source/Main_rc.py \
	source/Ui_Settings.py \
	source/Settings_rc.py

source/Ui_Main.py:
	pyuic4 ui/Main.ui -o source/Ui_Main.py

source/Main_rc.py:
	pyrcc4 ui/Main.qrc -o source/Main_rc.py

source/Ui_Settings.py:
	pyuic4 ui/Settings.ui -o source/Ui_Settings.py

source/Settings_rc.py:
	pyrcc4 ui/Settings.qrc -o source/Settings_rc.py

