#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QPushButton>
#include <QApplication>
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMenuBar>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QTime>
#include <QDialog>
#include <QIcon>

//This is the BlockButton class. I created it to be able to have an extra parameter which is the number
//to avoid depending on the text or the icon on the button.

class BlockButton : public QPushButton
{
    Q_OBJECT

public:
    BlockButton (QWidget *parent = 0);
    BlockButton (const QString &text, QWidget *parent = 0);
    BlockButton (int value, const QString &text, QWidget *parent = 0);
    ~BlockButton ();

    //The following two methods deal with the number parameter

    void setNumber (int value);
    int getNumber ();

private:
    int number;

};

//Here, starts the Main Window class with all of the methods used.

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void buttonPressed();
    void reset();
    void aboutClicked();
    void helpClicked();
    void randomize();
    void animeClicked();
    void cartoonClicked();
    void numberClicked();

private:
    const static int NUMBUTTONS = 16;
    BlockButton *buttons[NUMBUTTONS];
    QMenuBar* m;
    QGridLayout* grid;
    QLabel* lb;
    //The following dialog appears when the user wins
    QDialog* win;
    //The following string indicates which form we're currently on
    QString picture;

    //The following two methods are helper methods, so they are private

    bool solved();
    bool solvable();

};


#endif // MAINWINDOW_H


