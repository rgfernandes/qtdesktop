#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>


namespace
{
    const QString DEFAULT_LCDNUMBER_VALUE = "00:00:00  00:00:00";
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetToolTipsForUI();
    current_play_index_ = -1;
    ui->mainToolBar->setVisible(false);
    ui->toolButton_FullScreen->setVisible(false);

    CreatePhononObjects();
    SetupPhononUI();
    ConnectSlots();
    SetEnabledForToolButtons();

    ui->lcdNumber->display(DEFAULT_LCDNUMBER_VALUE);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}




void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (ui->tableWidget->columnCount() > 0)
    {
        ui->tableWidget->setColumnWidth(0, this->width());
    }
}



void MainWindow::ConnectSlots()
{
    QObject::connect(ui->toolButton_CloseApp, SIGNAL(clicked()), this, SLOT(toolButton_CloseApp_Clicked()));
    QObject::connect(ui->toolButton_Open, SIGNAL(clicked()), this, SLOT(toolButton_Open_Clicked()));
    QObject::connect(ui->toolButton_Stop, SIGNAL(clicked()), this, SLOT(toolButton_Stop_Clicked()));
    QObject::connect(ui->toolButton_DeleteTrack, SIGNAL(clicked()), this, SLOT(toolButton_DeleteTrack_Clicked()));
    QObject::connect(ui->toolButton_Next, SIGNAL(clicked()), this, SLOT(toolButton_Next_Clicked()));
    QObject::connect(ui->toolButton_Prev, SIGNAL(clicked()), this, SLOT(toolButton_Prev_Clicked()));
    QObject::connect(ui->toolButton_PlayPause, SIGNAL(clicked()), this, SLOT(toolButton_PlayPause_Clicked()));
    QObject::connect(ui->toolButton_About, SIGNAL(clicked()), this, SLOT(toolButton_About_Clicked()));
    QObject::connect(ui->toolButton_AddFiles, SIGNAL(clicked()), this, SLOT(toolButton_AddFiles_Clicked()));
    QObject::connect(ui->toolButton_RemoveList, SIGNAL(clicked()), this, SLOT(toolButton_RemoveList_Clicked()));
    QObject::connect(ui->toolButton_UpTrack, SIGNAL(clicked()), this, SLOT(toolButton_UpTrack_Clicked()));
    QObject::connect(ui->toolButton_DownTrack, SIGNAL(clicked()), this, SLOT(toolButton_DownTrack_Clicked()));
    QObject::connect(ui->toolButton_FullScreen, SIGNAL(clicked()), this, SLOT(toolButton_FullScreen_Clicked()));
    QObject::connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(tableWidget_itemDoubleClicked(QTableWidgetItem*)));    
    QObject::connect(ui->tableWidget, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)), this, SLOT(tableWidget_currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)));


    QObject::connect(this->videoPlayer_->mediaObject(), SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(videoPlayer_stateChanged(Phonon::State,Phonon::State)));
    QObject::connect(this->videoPlayer_->mediaObject(), SIGNAL(tick(qint64)), this, SLOT(videoPlayer_tick(qint64)));
    QObject::connect(this->videoPlayer_->mediaObject(), SIGNAL(totalTimeChanged(qint64)), this, SLOT(videoPlayer_totalTimeChanged(qint64)));
    QObject::connect(this->videoPlayer_, SIGNAL(finished()), this, SLOT(videoPlayer_finished()));    
}




void MainWindow::CreatePhononObjects()
{
    videoPlayer_ = new Phonon::VideoPlayer(Phonon::VideoCategory, this);
    videoPlayer_->videoWidget()->setVisible(false);
    videoPlayer_->mediaObject()->setTickInterval(1000);

    seekSlider_ = new Phonon::SeekSlider(this);
    seekSlider_->setMediaObject(videoPlayer_->mediaObject());

    volumeSlider_ = new Phonon::VolumeSlider(this);
    volumeSlider_->setAudioOutput(videoPlayer_->audioOutput());
    volumeSlider_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}




void MainWindow::SetupPhononUI()
{
    ui->horizontalLayout->insertWidget(0, seekSlider_ );
    ui->horizontalLayout_3->addWidget(volumeSlider_);
    ui->verticalLayout->addWidget(videoPlayer_);
}



void MainWindow::SetToolTipsForUI()
{
    ui->toolButton_About->setToolTip("About");
    ui->toolButton_AddFiles->setToolTip("Add files to playlist");
    ui->toolButton_CloseApp->setToolTip("Close");
    ui->toolButton_DeleteTrack->setToolTip("Delete selected files from the playlist");
    ui->toolButton_DownTrack->setToolTip("Move down current file in the playlist");
    ui->toolButton_Next->setToolTip("Play next file in the playlist");
    ui->toolButton_Open->setToolTip("Open files and create new playlist");
    ui->toolButton_PlayPause->setToolTip("Play/Pause");
    ui->toolButton_Prev->setToolTip("Play previous file in the playlist");
    ui->toolButton_RemoveList->setToolTip("Remove playlist");
    ui->toolButton_Stop->setToolTip("Stop");
    ui->toolButton_UpTrack->setToolTip("Move up current file in the playlist");
    ui->toolButton_FullScreen->setToolTip("Full Screen/Press Esc or mouse double click to exit from Full Screen Mode");
}





void MainWindow::toolButton_CloseApp_Clicked()
{
    this->close();
}






void MainWindow::SetEnabledForToolButtons()
{
    bool bEnabled = ui->tableWidget->rowCount() > 0;
    ui->toolButton_DeleteTrack->setEnabled(bEnabled);
    ui->toolButton_PlayPause->setEnabled(bEnabled);
    ui->toolButton_RemoveList->setEnabled(bEnabled);
    ui->toolButton_Stop->setEnabled(bEnabled);


    bEnabled = (ui->tableWidget->rowCount() > 1) && (ui->tableWidget->currentRow() < ui->tableWidget->rowCount() - 1);
    ui->toolButton_DownTrack->setEnabled(bEnabled);
    ui->toolButton_Next->setEnabled(bEnabled);


    bEnabled = (ui->tableWidget->rowCount() > 1) && (ui->tableWidget->currentRow() > 0);
    ui->toolButton_Prev->setEnabled(bEnabled);
    ui->toolButton_UpTrack->setEnabled(bEnabled);
}






void MainWindow::LoadFilesInList(const QStringList& list, bool bClearTable)
{
    if (bClearTable)
    {
        ClearTable();
    }

    if (ui->tableWidget->columnCount() == 0)
    {
        ui->tableWidget->insertColumn(0);
    }

    int size = list.size();
    int currentRow = ui->tableWidget->rowCount();

    for (int i = 0; i < size; ++i)
    {
        ui->tableWidget->insertRow(currentRow);

        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(QFileInfo(list[i]).fileName());
        item->setIcon(QFileIconProvider().icon(QFileInfo(list[i])));
        ui->tableWidget->setItem(currentRow, 0, item);

        ++currentRow;
    }

    ui->tableWidget->setColumnWidth(0, this->width());
}





void MainWindow::AddFilesToMediaSourceList(const QStringList& list)
{
    int count_files = list.size();
    for (int i = 0; i < count_files; ++i)
    {
        Phonon::MediaSource source(list[i]);
        list_sources_.append(source);
    }
}



QStringList MainWindow::GetFilesWillPlay()
{
    return QFileDialog::getOpenFileNames(this, QString("Open music and video files"), QDesktopServices::storageLocation(QDesktopServices::MusicLocation), QString("Media files (*.mp3 *.wav *.wma *.avi *.wmv);;Any files (*)"));
}




bool MainWindow::hasVideo(const QString& fileName) const
{
    QFileInfo fi(fileName);

    QString extens = fi.completeSuffix();

    extens = extens.toLower();

    if (extens == "avi" || extens == "wmv" || extens == "vob")
        return true;

    return false;
}





void MainWindow::videoPlayer_totalTimeChanged(qint64 time)
{
    displayTimeTotal_.setHMS(time / (60000*60), (time / 60000) % 60, (time / 1000) % 60);
    ui->lcdNumber->display("00:00:00" + QString("  ") + displayTimeTotal_.toString("hh:mm:ss"));
}



void MainWindow::videoPlayer_finished()
{
    videoPlayer_->stop();
    videoPlayer_->videoWidget()->setFullScreen(false);
    releaseKeyboard();
    releaseMouse();
    MarkPlayingItem(current_play_index_, false);
    videoPlayer_->videoWidget()->setVisible(false);
    ui->tableWidget->setVisible(true);

    if (current_play_index_ >= ui->tableWidget->rowCount() - 1)
        return;

    ui->tableWidget->setCurrentItem(ui->tableWidget->item(current_play_index_ + 1, 0));

    playNext();

    MarkPlayingItem(current_play_index_);
}








void MainWindow::videoPlayer_tick(qint64 time)
{
    QTime displayTime(time / (60000*60), (time / 60000) % 60, (time / 1000) % 60);
    ui->lcdNumber->display(displayTime.toString("hh:mm:ss") + QString("  ") + displayTimeTotal_.toString("hh:mm:ss"));
}





void MainWindow::videoPlayer_stateChanged(Phonon::State newState, Phonon::State oldState)
{
    switch (newState)
    {
        case Phonon::ErrorState:
            if (videoPlayer_->mediaObject()->errorType() == Phonon::FatalError)
            {
                QMessageBox::warning(this, tr("Fatal Error"),
                videoPlayer_->mediaObject()->errorString());
            }
            else
            {
                QMessageBox::warning(this, tr("Error"),
                videoPlayer_->mediaObject()->errorString());
            }
            break;

        case Phonon::PlayingState:
            ui->toolButton_PlayPause->setIcon(QIcon(":Images/pause.png"));
            ShowCurrentPlayingInTaskBar(list_sources_[current_play_index_]);
            if (videoPlayer_->mediaObject()->hasVideo())
            {
                ui->toolButton_FullScreen->setVisible(true);
            }
            else
            {
                ui->toolButton_FullScreen->setVisible(false);
            }
            break;

        case Phonon::StoppedState:
            ui->lcdNumber->display(DEFAULT_LCDNUMBER_VALUE);
            ui->toolButton_PlayPause->setIcon(QIcon(":Images/play.png"));

            videoPlayer_->videoWidget()->setVisible(false);
            ui->tableWidget->setVisible(true);            
            statusBar()->showMessage("");

            ui->toolButton_FullScreen->setVisible(false);
            break;

        case Phonon::PausedState:
            ui->toolButton_PlayPause->setIcon(QIcon(":Images/play.png"));
            break;

        case Phonon::BufferingState:
            break;

        default:
            break;
    }

}












void MainWindow::toolButton_Open_Clicked()
{
    QStringList files = GetFilesWillPlay();

    if (files.isEmpty())
        return;

    list_sources_.clear();
    AddFilesToMediaSourceList(files);
    LoadFilesInList(files, true);

    if (ui->tableWidget->currentRow() < 0)
    {
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(0, 0));        
    }    

    videoPlayer_->mediaObject()->setCurrentSource(list_sources_[ui->tableWidget->currentRow()]);

    current_play_index_ = ui->tableWidget->currentRow();

    SetEnabledForToolButtons();
}



void MainWindow::ClearTable()
{
    ui->tableWidget->clear();

    int row_count = ui->tableWidget->rowCount();
    int column_count = ui->tableWidget->columnCount();

    for (int i = 0; i < row_count; ++i)
    {
        ui->tableWidget->removeRow(0);
    }

    for (int i = 0; i < column_count; ++i)
    {
        ui->tableWidget->removeColumn(0);
    }

}



void MainWindow::toolButton_RemoveList_Clicked()
{
    toolButton_Stop_Clicked();
    ClearTable();
    list_sources_.clear();

    SetEnabledForToolButtons();

    current_play_index_ = -1;
}




void MainWindow::toolButton_AddFiles_Clicked()
{
    QStringList files = GetFilesWillPlay();

    if (files.isEmpty())
        return;

    AddFilesToMediaSourceList(files);
    LoadFilesInList(files);

    if (ui->tableWidget->currentRow() < 0)
    {
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(0, 0));        
    }


    if (!videoPlayer_->isPlaying())
    {
        videoPlayer_->mediaObject()->setCurrentSource(list_sources_[ui->tableWidget->currentRow()]);
    }
    else
    {
        ShowCurrentPlayingInTaskBar(list_sources_[current_play_index_]);
    }

    if (current_play_index_ == -1)
    {
        current_play_index_ = ui->tableWidget->currentRow();
    }


    SetEnabledForToolButtons();
}





void MainWindow::toolButton_DeleteTrack_Clicked()
{
    if (ui->tableWidget->rowCount() == 0)
        return;

    QList<QTableWidgetItem*> listSelectedItems = ui->tableWidget->selectedItems();
    int count_items = listSelectedItems.count();    
    if (count_items == 0)
        return;
    int row = listSelectedItems[0]->row();

    if (current_play_index_  >= row && current_play_index_ < row + count_items)
    {
       toolButton_Stop_Clicked();
    }

    for (int i = 0; i < count_items; ++i)
    {
        ui->tableWidget->removeRow(row);
        list_sources_.removeAt(row);
    }

    SetEnabledForToolButtons();

    if (ui->tableWidget->rowCount() == 0)
    {
        current_play_index_ = -1;
    }

}


void MainWindow::toolButton_UpTrack_Clicked()
{
    int current_row = ui->tableWidget->currentRow();
    if (current_row <= 0)
        return;

    QTableWidgetItem* item_up  = ui->tableWidget->takeItem(current_row, 0);
    QTableWidgetItem* item_down  = ui->tableWidget->takeItem(current_row - 1, 0);
    ui->tableWidget->setItem(current_row - 1, 0, item_up);
    ui->tableWidget->setItem(current_row, 0, item_down);

    bool wasPlaying = videoPlayer_->isPlaying() || videoPlayer_->isPaused();

    if (wasPlaying)
    {
        if (current_play_index_ == current_row)
        {
            --current_play_index_;
        }
        else
        {
            if (current_play_index_ == current_row - 1)
            {
                current_play_index_ = current_row;
            }
        }
    }

    list_sources_.swap(current_row, current_row - 1);
    ui->tableWidget->setCurrentItem(ui->tableWidget->item(current_row - 1, 0));
}




void MainWindow::toolButton_DownTrack_Clicked()
{
    int current_row = ui->tableWidget->currentRow();
    if (current_row < 0 || current_row >= ui->tableWidget->rowCount() - 1)
        return;

    QTableWidgetItem* item_down  = ui->tableWidget->takeItem(current_row, 0);
    QTableWidgetItem* item_up  = ui->tableWidget->takeItem(current_row + 1, 0);

    ui->tableWidget->setItem(current_row + 1, 0, item_down);
    ui->tableWidget->setItem(current_row, 0, item_up);

    bool wasPlaying = videoPlayer_->isPlaying() || videoPlayer_->isPaused();

    if (wasPlaying)
    {
        if (current_play_index_ == current_row)
        {
            ++current_play_index_;
        }
        else
        {
            if (current_play_index_ == current_row + 1)
            {
                current_play_index_ = current_row;
            }
        }

    }


    list_sources_.swap(current_row, current_row + 1);
    ui->tableWidget->setCurrentItem(ui->tableWidget->item(current_row + 1, 0));
}


void MainWindow::toolButton_Stop_Clicked()
{    
    videoPlayer_->stop();

    if (videoPlayer_->videoWidget()->isVisible())
    {
        videoPlayer_->videoWidget()->setVisible(false);
        ui->tableWidget->setVisible(true);
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(current_play_index_, 0));
    }

    MarkPlayingItem(current_play_index_, false);

    SetEnabledForToolButtons();
}



void MainWindow::toolButton_Next_Clicked()
{
    bool wasPlaying = videoPlayer_->isPlaying() || videoPlayer_->isPaused();

    if (wasPlaying)
    {
        videoPlayer_->stop();
        MarkPlayingItem(current_play_index_, false);
        videoPlayer_->videoWidget()->setVisible(false);
        ui->tableWidget->setVisible(true);
        ++current_play_index_;
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(current_play_index_, 0));
        playNext();
        MarkPlayingItem(current_play_index_);
    }
    else
    {
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(ui->tableWidget->currentRow() + 1, 0));
    }



}

void MainWindow::toolButton_Prev_Clicked()
{
    bool wasPlaying = videoPlayer_->isPlaying() || videoPlayer_->isPaused();

    if (wasPlaying)
    {
        videoPlayer_->stop();
        MarkPlayingItem(current_play_index_, false);
        videoPlayer_->videoWidget()->setVisible(false);
        ui->tableWidget->setVisible(true);
        --current_play_index_;
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(current_play_index_, 0));
        playNext();
        MarkPlayingItem(current_play_index_);
    }
    else
    {
       ui->tableWidget->setCurrentItem(ui->tableWidget->item(ui->tableWidget->currentRow() - 1, 0));
    }
}






void MainWindow::playNext()
{
    current_play_index_ = ui->tableWidget->currentRow();

    if (hasVideo(list_sources_[current_play_index_].fileName()))
    {
        ui->tableWidget->setVisible(false);
        videoPlayer_->videoWidget()->setVisible(true);        
    }
    else
    {
        videoPlayer_->videoWidget()->setVisible(false);
        ui->tableWidget->setVisible(true);
    }

    if (!videoPlayer_->isPaused())
    {
        videoPlayer_->play(list_sources_[current_play_index_].fileName());
    }
}




void MainWindow::toolButton_PlayPause_Clicked()
{
    if (videoPlayer_->isPlaying())
    {        
        videoPlayer_->pause();
    }
    else
    {
        if (!videoPlayer_->isPaused())
        {
            current_play_index_ = ui->tableWidget->currentRow();

            if (hasVideo(list_sources_[current_play_index_].fileName()))
            {
                ui->tableWidget->setVisible(false);                
                videoPlayer_->videoWidget()->setVisible(true);
            }
            else
            {               
                videoPlayer_->videoWidget()->setVisible(false);
                ui->tableWidget->setVisible(true);
            }

            videoPlayer_->play(list_sources_[current_play_index_]);
        }
        else
        {
            videoPlayer_->play();
        }

        MarkPlayingItem(current_play_index_);
    }
}




void MainWindow::ShowCurrentPlayingInTaskBar(const Phonon::MediaSource& source)
{
    statusBar()->showMessage("Now playing: " + QFileInfo(source.fileName()).fileName());
}






void MainWindow::toolButton_About_Clicked()
{
    QMessageBox::information(this, tr("About Qt Media Player"),
                                   tr("The Qt Media Player is very simple media player."
                                      "This player uses Qt Phonon multimedia library."));
}




void MainWindow::tableWidget_itemDoubleClicked(QTableWidgetItem* item)
{
    if (videoPlayer_->isPlaying() || videoPlayer_->isPaused())
    {
        toolButton_Stop_Clicked();
        MarkPlayingItem(current_play_index_, false);
    }

    playNext();
    MarkPlayingItem(current_play_index_);
}






void MainWindow::tableWidget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    if (current == 0)
        return;

    int index = current->row();

    if (index == 0)
    {
        ui->toolButton_UpTrack->setEnabled(false);
    }
    else
    {
        ui->toolButton_UpTrack->setEnabled(true);
    }


    if (index == ui->tableWidget->rowCount() - 1)
    {
        ui->toolButton_DownTrack->setEnabled(false);
    }
    else
    {
        ui->toolButton_DownTrack->setEnabled(true);
    }


    bool wasPlaying = videoPlayer_->isPlaying() || videoPlayer_->isPaused();

    if (wasPlaying)
    {
        index = current_play_index_;
    }


    if (index == 0)
    {
        ui->toolButton_Prev->setEnabled(false);        
    }
    else
    {
        ui->toolButton_Prev->setEnabled(true);        
    }


    if (index == ui->tableWidget->rowCount() - 1)
    {        
        ui->toolButton_Next->setEnabled(false);
    }
    else
    {        
        ui->toolButton_Next->setEnabled(true);
    }


}





void MainWindow::MarkPlayingItem(int index, bool bMark)
{
    QTableWidgetItem* item = ui->tableWidget->item(index, 0);
    if (item == 0)
        return;

    if (bMark)
    {
        item->setBackgroundColor(QColor(Qt::lightGray));
        item->setForeground(QBrush(Qt::blue));
    }
    else
    {
        item->setBackgroundColor(QColor(Qt::white));
        item->setForeground(QBrush(Qt::black));
    }
}



void MainWindow::toolButton_FullScreen_Clicked()
{
    if (videoPlayer_->isPlaying() && videoPlayer_->mediaObject()->hasVideo())
    {
        grabKeyboard();
        grabMouse();
        videoPlayer_->videoWidget()->setFullScreen(true);
    }
}



void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    videoPlayer_->videoWidget()->setFullScreen(false);
    releaseKeyboard();
    releaseMouse();
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Escape:
            videoPlayer_->videoWidget()->setFullScreen(false);
            releaseMouse();
            releaseKeyboard();
            break;

        default:
            break;
    }
}
