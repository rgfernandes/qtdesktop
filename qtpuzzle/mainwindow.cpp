#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>

//The following constructor was created in case a blockbuttons is needed without passing the number to it.
BlockButton::BlockButton (QWidget *parent)
    : QPushButton (parent)
{}


//The following constructor is used in case only the text is passed
BlockButton::BlockButton (const QString &text, QWidget *parent)
    : QPushButton (text, parent)
{}

//The following constructor is used in case, the number, the text and the parent are passed
BlockButton::BlockButton (int value, const QString &text, QWidget *parent)
    : QPushButton (text, parent)
{
    number = value;
}

//The setter and the getters of the number
void BlockButton::setNumber(int value)
{
    number = value;
}

int BlockButton::getNumber()
{
    return number;
}

//The destructor of the button
BlockButton::~BlockButton()
{}

//The constructor of the main window. It initializes thr window, the grid and the menu bar

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    // Initializing the window and the format is the numeric one with picture = ""

    setFixedSize(1300, 1000);
    setWindowTitle(tr("Fifteen Puzzle"));
    picture = "";

    //Now creating the grid layout and the buttons

    grid = new QGridLayout;


    for(int i = 0;i<16;i++){

        // First, creating the buttons and saving them in a blockbutton* array
        // Here we create the text on the button

        QString num;
        if(i == 15){
        num = tr("");
        }
        else{
        num.setNum (i+1);
        }
     // here the buttons are created and their sizes and fonts are created

     buttons[i] = new BlockButton (i+1, num, this);
     buttons[i]->setFixedSize(100,100);
     buttons[i]->setFont(QFont("Times", 18, QFont::Bold));

     // And they're being connected to the buttonpressed slot

     connect (buttons[i], SIGNAL(clicked()), this, SLOT(buttonPressed()));
    }

    //Now, adding them to the grid layout
    int pointer = 0;
    for(int j = 1; j<=4; j++){

    for (int i = 1 ; i <= 4 ; ++i)
    {

        grid->addWidget(buttons[pointer],j,i,1,1);pointer++;
    }
    }

    // For randomizing, and in addition to the randomize method, I randomize the number of times, the method is called
    // with a maximum of 5

    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));


    int k  = (qrand() % (5)) +1;
    for(int i = 0; i<k;i++){
    randomize();
}
    // Setting the Menu Bar
    m = new QMenuBar(this);


    QAction *quit = new QAction("Quit", this);
    QAction *about = new QAction("About", this);
    QAction *reset = new QAction("Reset", this);
    QAction *mix = new QAction("Mix", this);
    QAction *numbers = new QAction("Number Icons", this);
    QAction *comic = new QAction("Comic Puzzle", this);
    QAction *anime = new QAction("Anime Puzzle", this);
    QAction *help = new QAction("Help", this);

    // File Menu has the quit, reset(putting the puzzle in its solved form) and mix(creates a solvable randomized puzzle)

    QMenu* file;
    file = menuBar()->addMenu(tr("File"));
    file->addAction(quit);
    file ->addAction(reset);
    file ->addAction(mix);

    // The info menu has any actions related to info about the programmer or the program
    // About -> briefly tells the user about the reason for creating the program and the the name of the programmer
    // Help -> basc info of the game and description of the menus

    QMenu* info;
    info = menuBar()->addMenu(tr("Info"));
    info ->addAction(about);
    info ->addAction(help);
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(reset, SIGNAL(triggered()), this, SLOT(reset()));
    connect (about, SIGNAL(triggered()), this, SLOT(aboutClicked()));
    connect (help, SIGNAL(triggered()), this, SLOT(helpClicked()));
    connect (mix, SIGNAL(triggered()), this, SLOT(randomize()));

    // The view menu has all of the options for playing the puzzle with a picture(two options for this) or with numbers
    // each action is connected to its corresponding method slot

    QMenu* view;
    view = menuBar()->addMenu(tr("View"));
    view ->addAction(numbers);
    view ->addAction(comic);
    view ->addAction(anime);
    connect (comic, SIGNAL(triggered()), this, SLOT(cartoonClicked()));
    connect (anime, SIGNAL(triggered()), this, SLOT(animeClicked()));
    connect (numbers, SIGNAL(triggered()), this, SLOT(numberClicked()));

    //Setting the final layout
    lb = new QLabel;
    lb->setLayout(grid);
    lb->setFixedSize(400,400);
    setCentralWidget(lb);

}
MainWindow::~MainWindow()
{

}

// The following method is called when a button is clicked

void MainWindow::buttonPressed()
{

    // The following part makes sure that if the puzzle was originally in the solved mode with a pciture
    // and the user clicks on a button( can only happen when the user clicks reset then clicks on any button),
    // the 16th part of the picture disappears

    if(solved()){
        if(picture !=""){
       QIcon j;
       buttons[15]->setIcon(j);
        }
    }

    // This boolean makes sure the buttons are only moved once per turn

    bool moved = false;

    BlockButton *pb = qobject_cast <BlockButton *> (sender());

    // First, searching for the sender button

    int found= -1;
    if(pb->getNumber()!=16){
        for(int i=0;i<16&&found<0;i++){
            if(buttons[i]->getNumber()==(pb->getNumber())){
                found = i;
            }
        }

        //Once found, we make sure that it has the blank button somewhere around it
        // Then, according to the format, the numbers are exchanged between it and the blank
        // And if we're in the number format, the texts are changed, and if in a picture format, the icons are exchanged

        if(found!=3&&found!=7&&found!=11&&found!=15){
        //Checking to the right
            if(buttons[found+1]->getNumber()==16){
            int num = 16;
            buttons[found+1]->setNumber(buttons[found]->getNumber());
            QString s;

            if(picture =="")
            buttons[found+1]->setText(s.setNum (buttons[found]->getNumber()));

            buttons[found]->setNumber(num);
            if(picture =="")
            buttons[found]->setText(tr(""));
                QIcon i = buttons[found]->icon();
                buttons[found]->setIcon(buttons[found+1]->icon());
                buttons[found+1]->setIcon(i);
            moved = true;
            }
        }
    }
        if(!moved&&found!=0&&found!=4&&found!=8&&found!=12){
            //Checking to the left
                if(buttons[found-1]->getNumber()==16){
                int num = 16;
                buttons[found-1]->setNumber(buttons[found]->getNumber());
                QString s;

                if(picture =="")
                buttons[found-1]->setText(s.setNum (buttons[found]->getNumber()));
                buttons[found]->setNumber(num);
                if(picture =="")
                buttons[found]->setText(tr(""));
                    QIcon i = buttons[found]->icon();
                    buttons[found]->setIcon(buttons[found-1]->icon());
                    buttons[found-1]->setIcon(i);
                moved = true;
                }

        }
        if(!moved&&found!=0&&found!=1&&found!=2&&found!=3){
            //Checking upwards
                if(buttons[found-4]->getNumber()==16){
                int num = 16;
                buttons[found-4]->setNumber(buttons[found]->getNumber());
                QString s;

                if(picture =="")
                buttons[found-4]->setText(s.setNum (buttons[found]->getNumber()));
                buttons[found]->setNumber(num);
                if(picture =="")
                buttons[found]->setText(tr(""));
                    QIcon i = buttons[found]->icon();
                    buttons[found]->setIcon(buttons[found-4]->icon());
                    buttons[found-4]->setIcon(i);
                moved = true;
                }

        }
        if(!moved&&found!=12&&found!=13&&found!=14&&found!=15){
            //Checking downwards
                if(buttons[found+4]->getNumber()==16){
                int num = 16;
                buttons[found+4]->setNumber(buttons[found]->getNumber());
                QString s;

                if(picture =="")
                buttons[found+4]->setText(s.setNum (buttons[found]->getNumber()));
                buttons[found]->setNumber(num);
                if(picture =="")
                buttons[found]->setText(tr(""));
                    QIcon i = buttons[found]->icon();
                    buttons[found]->setIcon(buttons[found+4]->icon());
                    buttons[found+4]->setIcon(i);
                moved = true;
                }

        }
    // checking if we're in the solved format and the button was moved
    // if yes, a dialog appears congratulating the user and asks him/her is they want to play again or quit the game
    // And if we're in the picture format, the 16th part of the picture appears

    if(solved()&&moved){
        if(picture == "cartoon"){
            QPixmap p("./cartoon-sliced_16.jpg");
            buttons[15]->setIcon(p);

        }else{
            if(picture == "anime"){
                QPixmap p("./Daddy_look_by_meago_16.jpg");
                buttons[15]->setIcon(p);

            }
        }
        win = new QDialog(this);
        QPushButton* quit = new QPushButton(tr("Cancel"));
        QPushButton* nue = new QPushButton(tr("OK"));
        QLabel* msg = new QLabel(this);
        msg->setText(tr("Congratulations! You won! \nWould you like to play again? If yes, select \"OK\". If not, select \"Cancel\""));
        connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
        connect(nue, SIGNAL(clicked()), this, SLOT(randomize()));
        connect(nue, SIGNAL(clicked()), win, SLOT(close()));

        QHBoxLayout* h = new QHBoxLayout;
        h->addWidget(nue);
        h->addWidget(quit);
        QVBoxLayout* v = new QVBoxLayout;
        v->addWidget(msg);
        v->addLayout(h);
        win->setLayout(v);
        win->show();
            }
}

// This puts the board in its solved format. if we're in the picture format, the 16th part of the picture appears

void MainWindow::reset()
{
    // Emptying the grid

    QString s;
    for(int i=0; i<16;i++){
    grid->removeWidget(buttons[i]);
    }

    // Refilling the grid with the buttons in the correct order

    for(int i = 0;i<16;i++){
        if(picture == ""){
        QString num;
        if(i == 15){
        num = tr("");
        }
        else{
        num.setNum (i+1);
        }

     buttons[i] = new BlockButton (i+1, num, this);
     buttons[i]->setFixedSize(100,100);
     buttons[i]->setFont(QFont("Times", 18, QFont::Bold));
     connect (buttons[i], SIGNAL(clicked()), this, SLOT(buttonPressed()));
    }
        else{

            buttons[i] = new BlockButton (i+1,tr(""), this);
            if(picture == "cartoon"){
            switch(buttons[i]->getNumber()){
            case 1:s ="./cartoon-sliced_1.jpg";break;
            case 2:s ="./cartoon-sliced_2.jpg";break;
            case 3:s ="./cartoon-sliced_3.jpg";break;
            case 4:s ="./cartoon-sliced_4.jpg";break;
            case 5:s ="./cartoon-sliced_5.jpg";break;
            case 6:s ="./cartoon-sliced_6.jpg";break;
            case 7:s ="./cartoon-sliced_7.jpg";break;
            case 8:s ="./cartoon-sliced_8.jpg";break;
            case 9:s ="./cartoon-sliced_9.jpg";break;
            case 10:s ="./cartoon-sliced_10.jpg";break;
            case 11:s ="./cartoon-sliced_11.jpg";break;
            case 12:s ="./cartoon-sliced_12.jpg";break;
            case 13:s ="./cartoon-sliced_13.jpg";break;
            case 14:s ="./cartoon-sliced_14.jpg";break;
            case 15:s ="./cartoon-sliced_15.jpg";break;
            case 16:s ="./cartoon-sliced_16.jpg";break;

            }

          buttons[i]->setFixedSize(201,158);
            buttons[i]->setIconSize(QSize(buttons[i]->width(), buttons[i]->height()));
            QPixmap image(s);
            QIcon icon(image);
            connect (buttons[i], SIGNAL(clicked()), this, SLOT(buttonPressed()));

            buttons[i]->setIcon(icon);
            lb->setFixedSize(QSize(buttons[i]->width()*4, buttons[i]->height()*4));





        }else{
            if(picture == "anime"){
            switch(buttons[i]->getNumber()){
            case 1:s ="./Daddy_look_by_meago_1.jpg";break;
            case 2:s ="./Daddy_look_by_meago_2.jpg";break;
            case 3:s ="./Daddy_look_by_meago_3.jpg";break;
            case 4:s ="./Daddy_look_by_meago_4.jpg";break;
            case 5:s ="./Daddy_look_by_meago_5.jpg";break;
            case 6:s ="./Daddy_look_by_meago_6.jpg";break;
            case 7:s ="./Daddy_look_by_meago_7.jpg";break;
            case 8:s ="./Daddy_look_by_meago_8.jpg";break;
            case 9:s ="./Daddy_look_by_meago_9.jpg";break;
            case 10:s ="./Daddy_look_by_meago_10.jpg";break;
            case 11:s ="./Daddy_look_by_meago_11.jpg";break;
            case 12:s ="./Daddy_look_by_meago_12.jpg";break;
            case 13:s ="./Daddy_look_by_meago_13.jpg";break;
            case 14:s ="./Daddy_look_by_meago_14.jpg";break;
            case 15:s ="./Daddy_look_by_meago_15.jpg";break;
            case 16:s ="./Daddy_look_by_meago_16.jpg";break;

            }

          buttons[i]->setFixedSize(217,157);
            buttons[i]->setIconSize(QSize(buttons[i]->width(), buttons[i]->height()));
            QPixmap image(s);
            QIcon icon(image);
            connect (buttons[i], SIGNAL(clicked()), this, SLOT(buttonPressed()));

            buttons[i]->setIcon(icon);
            lb->setFixedSize(QSize(buttons[i]->width()*4, buttons[i]->height()*4));





        }
        }

        }
    }
    int pointer = 0;
    for(int j = 1; j<=4; j++){

    for (int i = 1 ; i <= 4 ; ++i)
    {

        grid->addWidget(buttons[pointer],j,i,1,1);pointer++;
    }
    }

}


// the following function checks if the board was solved or not, by checking the number os the buttons in order
// If they're in the correct order, a true is returned, toherwise it's a false

bool MainWindow::solved()
{
bool order = true;
for(int i =0;i<=15;i++){
    if(buttons[i]->getNumber()!=(i+1)){
        order = false;
    }
}
//The code commented out below was used to determine if solved is correct
/*
QString s;
s.setNum(order);
QMessageBox::about (this, tr("Is it solved?"),s);
*/
return order;
}

// This method creates the about dialog box when the about actions is chosen from the menu

void MainWindow::aboutClicked()
{
    statusBar()->showMessage(tr("About was pressed"), 2000);
    QMessageBox::about (this, tr("About the Program"),
                        tr("This program was created as the fourth project for the class CS 340 - Spring 2011. \n Created by: Heba Basiony."));

}

// This is the randomize function and it's called when we first create the board
// or when the user chooses mix from the File menu

void MainWindow::randomize()
{

    // First of all, we look for the 16th button
    int found = -1;
    for(int i = 0; i<16;i++){
    if(buttons[i]->getNumber()==16)
        found = i;
    }
    //-----------------------------------------------------------------------------------

    // When found, we make sure it's in the last position in the corner of the grid
    if(found!=15){
    buttons[found]->setNumber(buttons[15]->getNumber());
    QString s;
 if(picture =="")
    buttons[found]->setText(s.setNum (buttons[15]->getNumber()));
    buttons[15]->setNumber(16);
    if(picture =="")
    buttons[15]->setText(tr(""));
        QIcon i = buttons[found]->icon();
        buttons[found]->setIcon(buttons[15]->icon());
        buttons[15]->setIcon(i);
    }
    //-----------------------------------------------------------------------------------

    // Then, using qrand, looping through the grid we get the new position for the current button and exchange it
    // with the button in that cell

    for(int i=0; i<15; i++){
    int k  = qrand() % (14);
    int num = buttons[k]->getNumber();
    buttons[k]->setNumber(buttons[i]->getNumber());
    buttons[i]->setNumber(num);


        QString s = buttons[k]->text();
        buttons[k]->setText(buttons[i]->text());
        buttons[i]->setText(s);


        if(buttons[i]->getNumber()!=16){
    QIcon j = buttons[k]->icon();
    buttons[k]->setIcon(buttons[i]->icon());
    buttons[i]->setIcon(j);
    }

    }

    if(picture!= ""){
        // making sure that the 16th position doesn't have an icon, if we're in the picture format

    QIcon j;
    buttons[15]->setIcon(j);
        }
    // If by chance,we come back to the solved form, we call randomize again

    if(solved())
        randomize();

    // if the form we get is not solvable,we call randomize again

    if(!solvable())
    randomize();
}

//This creates the help dialog when the user clicks on help from the menu

void MainWindow::helpClicked()
{
    statusBar()->showMessage(tr("Help was pressed"), 2000);
    QMessageBox::about (this, tr("Program Instructions"),
                        tr("The Fifteen Puzzle game is a variation of the N-Puzzle general game. \nTo play the game, click on any block next to the blank block, to exchange it with that block. \nThe goal is to put the numbers in the correct order or to put the picture in the correct order, according to your view. \n\n About the Menus:\n File Menu:\n  Quit -> quits the program\n  Reset -> puts the puzzle in its solved format\n  Mix -> puts the puzzle in a random solvable puzzle.\n\n Info Menu:\n  About -> about the author and the creation of the game.\n  Help -> Provides instructions about the game and the menus\n\n View menu:\n  Numbers Icons -> The blocks willl have numbers on top of them.\n  Comic Puzzle -> You'll try to order a comic script using the blocks.\n  Anime Puzzle -> You'll try to order an anime picture using the blocks. \n \nThanks for playing the game."));

}

// This makes sure that the grid is solvable using the technique described in the project description
// called when randomizing

bool MainWindow::solvable(){
    int inversions = 0;
    // Going through the grid(not counting the 16th button(we make sure of that by checking the number insied the grid))
    // Counting the inversions
    for(int i = 0; i<16; i++){
    int n = buttons[i]->getNumber();
    if(n!=16){
    for(int j = i; j<16; j++){

        int m = buttons[j]->getNumber();
        if(m<n){
        inversions++;
        }
    }
    }
    }
    // The code commented out down there was used for testing the randomizing problem
/*
    QString s;
    s.setNum(inversions);
    QMessageBox::about (this, tr("Number of inversions"),s);
*/
    // Checking if the inversion is even or odd
    if(inversions%2==0)
        return true;
    else return false;
}

// This changes the icon of the buttons to the cartoon format when the user chooses cartoon puzzle from the menu

void MainWindow::cartoonClicked()
{
    QString s;
    for(int i = 0; i<16;i++){
        switch(buttons[i]->getNumber()){
        case 1:s ="./cartoon-sliced_1.jpg";break;
        case 2:s ="./cartoon-sliced_2.jpg";break;
        case 3:s ="./cartoon-sliced_3.jpg";break;
        case 4:s ="./cartoon-sliced_4.jpg";break;
        case 5:s ="./cartoon-sliced_5.jpg";break;
        case 6:s ="./cartoon-sliced_6.jpg";break;
        case 7:s ="./cartoon-sliced_7.jpg";break;
        case 8:s ="./cartoon-sliced_8.jpg";break;
        case 9:s ="./cartoon-sliced_9.jpg";break;
        case 10:s ="./cartoon-sliced_10.jpg";break;
        case 11:s ="./cartoon-sliced_11.jpg";break;
        case 12:s ="./cartoon-sliced_12.jpg";break;
        case 13:s ="./cartoon-sliced_13.jpg";break;
        case 14:s ="./cartoon-sliced_14.jpg";break;
        case 15:s ="./cartoon-sliced_15.jpg";break;

        }
        if((buttons[i]->getNumber()==16) && solved()){
       if(picture == "cartoon")
        s ="./cartoon-sliced_16.jpg";
        QPixmap image(s);
        QIcon icon(image);

        buttons[i]->setIcon(icon);
    }
        BlockButton* b = buttons[i];
        b->setText("");
        b->setFixedSize(201,158);
        b->setIconSize(QSize(b->width(), b->height()));
        if(b->getNumber()!=16){
        QPixmap image(s);
        QIcon icon(image);
        b->setIcon(icon);
        lb->setFixedSize(QSize(b->width()*4, b->height()*4));
        picture = "cartoon";
    }
    }
}

// This changes the icons to the anme format when the suer chooses anime puzzle from the menu

void MainWindow::animeClicked()
{
    QString s;
    for(int i = 0; i<16;i++){
        switch(buttons[i]->getNumber()){
        case 1:s ="./Daddy_look_by_meago_1.jpg";break;
        case 2:s ="./Daddy_look_by_meago_2.jpg";break;
        case 3:s ="./Daddy_look_by_meago_3.jpg";break;
        case 4:s ="./Daddy_look_by_meago_4.jpg";break;
        case 5:s ="./Daddy_look_by_meago_5.jpg";break;
        case 6:s ="./Daddy_look_by_meago_6.jpg";break;
        case 7:s ="./Daddy_look_by_meago_7.jpg";break;
        case 8:s ="./Daddy_look_by_meago_8.jpg";break;
        case 9:s ="./Daddy_look_by_meago_9.jpg";break;
        case 10:s ="./Daddy_look_by_meago_10.jpg";break;
        case 11:s ="./Daddy_look_by_meago_11.jpg";break;
        case 12:s ="./Daddy_look_by_meago_12.jpg";break;
        case 13:s ="./Daddy_look_by_meago_13.jpg";break;
        case 14:s ="./Daddy_look_by_meago_14.jpg";break;
        case 15:s ="./Daddy_look_by_meago_15.jpg";break;

        }
        if((buttons[i]->getNumber()==16) && solved()){
        s ="./Daddy_look_by_meago_16.jpg";
        QPixmap image(s);
        QIcon icon(image);

        buttons[i]->setIcon(icon);

    }
        BlockButton* b = buttons[i];
        b->setText("");
        b->setFixedSize(217,157);
        b->setIconSize(QSize(b->width(), b->height()));
        if(b->getNumber()!=16){
        QPixmap image(s);;
        QIcon icon(image);

        b->setIcon(icon);
        lb->setFixedSize(QSize(b->width()*4, b->height()*4));

    picture = "anime";
    }
    }
}

// This method changes the format of the puzzle to the number format when the user chooses numbers puzzle from the menu
void MainWindow::numberClicked(){
    for(int i=0;i<16;i++){
    QIcon m;
    buttons[i]->setIcon(m);
    QString s;
    if(buttons[i]->getNumber()!=16)
    buttons[i]->setText(s.setNum(buttons[i]->getNumber()));
    picture = "";
    buttons[i]->setFixedSize(110,110);
    lb->setFixedSize(QSize(buttons[i]->width()*4, buttons[i]->height()*4));
 buttons[i]->setFont(QFont("Times", 18, QFont::Bold));
}
}
