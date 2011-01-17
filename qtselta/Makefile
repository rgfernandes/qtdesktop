RUN = run

clean:
	rm -rf run
	rm -f i18n/*.qm
	rm -f ui/*.py*
	rm -f src/*.pyc

exec:
	# 0. clean all
	rm -rf run
	mkdir $(RUN)
	mkdir $(RUN)/help
	mkdir $(RUN)/sql
	mkdir $(RUN)/i18n
	# 1. compile metadata into pyc
	# 1.1. compile ui and qrc
	for i in `ls *.qrc`; do pyrcc4 $$i > `dirname $$i`/`basename $$i .qrc`_rc.py; done
	for i in `ls ui/*.ui`; do pyuic4 $$i > `dirname $$i`/Ui_`basename $$i .ui`.py; done
	# 1.2. compile to bytecode
	python -c "import compileall; compileall.compile_dir('src', force=1)"
	python -c "import compileall; compileall.compile_dir('ui', force=1)"
	# 1.3. clean
	#rm -f ui/*.py
	# 2. translations
	lrelease-qt4 i18n/*.ts
	# 3. the end
	cp help/* $(RUN)/help
	cp sql/* $(RUN)/sql
	cp src/*.pyc $(RUN)
	cp src/qtselta.py $(RUN)
	cp ui/*.pyc $(RUN)
	cp i18n/*.qm $(RUN)/i18n

ln:
	rm -f $(RUN)/*
	for i in `ls src/*py`; do ln -s ../$$i run/`basename $$i`; done
	for i in `ls ui/*py`; do ln -s ../$$i run/`basename $$i`; done
	for i in `ls i18n/*qm`; do ln -s ../$$i run/`basename $$i`; done

tar:
	tar zcf ../qtselta.`date +%y%m%d%H%M%S`.tar.gz insupol.py *.pyc *.qm *.html && cd ..

zip:
	cd $(RUN) && zip ../qtselta.`date +%y%m%d%H%M%S`.zip insupol.py *.pyc *.qm *.html && cd ..

backup:
	find $(RUN) -type f -delete
	find . -type f -iname \*pyc -delete
	tar zcf ../qtselta.`cat ver`.`date +%y%m%d%H%M%S`.tar.gz .
