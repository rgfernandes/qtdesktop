#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <phonon/audiooutput.h>
#include <phonon/seekslider.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <phonon/VideoWidget>
#include <phonon/VideoPlayer>
#include <phonon/Path>
#include <phonon/backendcapabilities.h>
#include <QList>
#include <QTableWidgetItem>
#include <QTime>
#include <QtMultimedia>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void toolButton_CloseApp_Clicked();
    void toolButton_Open_Clicked();
    void toolButton_Stop_Clicked();
    void toolButton_DeleteTrack_Clicked();
    void toolButton_Next_Clicked();
    void toolButton_Prev_Clicked();
    void toolButton_PlayPause_Clicked();
    void toolButton_About_Clicked();
    void toolButton_AddFiles_Clicked();
    void toolButton_RemoveList_Clicked();
    void toolButton_UpTrack_Clicked();
    void toolButton_DownTrack_Clicked();
    void toolButton_FullScreen_Clicked();

    void tableWidget_itemDoubleClicked(QTableWidgetItem* item);
    void tableWidget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void videoPlayer_stateChanged(Phonon::State newState, Phonon::State oldState);
    void videoPlayer_tick(qint64 time);
    void videoPlayer_totalTimeChanged(qint64 time);
    void videoPlayer_finished();

private:
    void ConnectSlots();
    void CreatePhononObjects();
    void LoadFilesInList(const QStringList& list, bool bClearTable = false);
    void ClearTable();
    void AddFilesToMediaSourceList(const QStringList& list);
    QStringList GetFilesWillPlay();

    void SetupPhononUI();

    bool hasVideo(const QString& fileName) const;
    void playNext();
    void ShowCurrentPlayingInTaskBar(const Phonon::MediaSource& source);
    void MarkPlayingItem(int index, bool bMark = true);
    void SetToolTipsForUI();    
    void SetEnabledForToolButtons();

private:
    Ui::MainWindow *ui;

    Phonon::SeekSlider *seekSlider_;    
    Phonon::VolumeSlider *volumeSlider_;
    QList<Phonon::MediaSource> list_sources_;

    Phonon::VideoPlayer *videoPlayer_;
    QTime displayTimeTotal_;


    int current_play_index_;
};

#endif // MAINWINDOW_H
