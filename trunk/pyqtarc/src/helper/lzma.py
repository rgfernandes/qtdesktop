'''
7z helper
'''

from PyQt4 import import 

class	ArchHelper7z:
	exts = (
	        '7z', 
	)
	mimes = (
	        'application/x-7z-compressed', 
	)

	@classmethod
	def	get_ext():
		return self.exts

	@classmethod
	def	get_mime():
		return self.mimes

	def	__init__(self):
		self.__path = None

	def	list(self,  path):
		'''
		@param path: 
		'''
		QProcess arch;
		// 7z start
		arch.start("7za", QStringList() << "l" << archname);
		// 7z end
		arch.waitForFinished();
		QString out = QString(arch.readAllStandardOutput());
		//QString err = QString(arch.readAllStandardError());
		root->getChildren()->clear();
		QStringList outlist = out.split('\n');
		// 7z start
		QRegExp rx("^\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2} [D.][R.][H.][S.][A.] [ 0-9]{12} [ 0-9]{12}  .*$");
		// 7z end
		QStringList::const_iterator ci;
		for (ci = outlist.constBegin(); ci != outlist.constEnd(); ++ci) {
			if (rx.indexIn(*ci) == 0) {
		// 7z start
				fi.type =	(*ci).mid(20, 5).startsWith('D');
				fi.path =	(*ci).mid(53);
				fi.size =	(*ci).mid(26, 12).trimmed().toLong();
				fi.datetime =	QDateTime::fromString((*ci).mid(0, 19), "yyyy-MM-dd HH:mm:ss");
		// 7z end
				QStringList pathlist = fi.path.split('/');
				root->addChildRecursive(pathlist, fi.type, fi.size, fi.datetime);
			}
		}
		sort();
		return true;
