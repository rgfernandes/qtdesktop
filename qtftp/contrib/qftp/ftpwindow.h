#ifndef FTPWINDOW_H
#define FTPWINDOW_H

#include <QDialog>
#include <QHash>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFile;
class QFtp;
class QLabel;
class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QProgressDialog;
class QPushButton;
class QUrlInfo;
class QNetworkSession;
QT_END_NAMESPACE

class FtpWindow : public QDialog
{
    Q_OBJECT

public:
    FtpWindow(QWidget *parent = 0);
    QSize sizeHint() const;

//![0]
private slots:
    void connectOrDisconnect();
    void downloadFile();
    void cancelDownload();

    void ftpCommandFinished(int commandId, bool error);
    void addToList(const QUrlInfo &urlInfo);
    void processItem(QTreeWidgetItem *item, int column);
    void cdToParent();
    void updateDataTransferProgress(qint64 readBytes,
                                    qint64 totalBytes);
    void enableDownloadButton();
    void enableConnectButton();
//![0]

private:
    QLabel *ftpServerLabel;
    QLineEdit *ftpServerLineEdit;
    QLabel *statusLabel;
    QTreeWidget *fileList;
    QPushButton *cdToParentButton;
    QPushButton *connectButton;
    QPushButton *downloadButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QProgressDialog *progressDialog;

//![1]
    QHash<QString, bool> isDirectory;
    QString currentPath;
    QFtp *ftp;
    QFile *file;

    QNetworkSession *networkSession;
//![1]
};

#endif
