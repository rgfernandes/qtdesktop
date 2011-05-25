#include "architemmodel.h"

ArchItemModel::ArchItemModel()
{
	lastSortColumn = 0;
	lastSortOrder = Qt::AscendingOrder;
	#ifdef Q_OS_WIN // Как много нам открытий чудных...
	rootItem = new ArchItem("", true); // А все потому, что в windows нет корня как такового
	QFileInfoList drives = QDir::drives(); // Зато есть диски
	for (QFileInfoList::iterator driveIt = drives.begin(); driveIt != drives.end(); ++driveIt) // Здесь то мы их и создаем
	{
		ArchItem *drive = new ArchItem((*driveIt).absolutePath(), true);
		rootItem->addChild(drive);
	}
	#else
	QString path = QDir::rootPath();
	rootItem = new ArchItem(path, true);
	readDir(rootItem); //читаем содержимое корня сразу же
	#endif
}

void ArchItemModel::readDir(ArchItem *item)
{
	item->setChildren(getFileList(item)); // Получаем детей и сразу связывем их с предком
	// Здесь опущен вызов сортировки
}

void ArchItemModel::fetchMore(const QModelIndex &index)
{
	if (index.isValid() && !item(index)->getFetched()) // Если еще ни разу детей не загружали то загружаем.
	{
		readDir(item(index));
		item(index)->setFetched(true);
		refresh();
	}
}