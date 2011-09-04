//This file is part of AcetoneISO. Copyright 2006,2007,2008,2009 Marco Di Antonio and Fabrizio Di Marco (acetoneiso@gmail.com)
//
//    AcetoneISO is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    AcetoneISO is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with AcetoneISO.  If not, see <http://www.gnu.org/licenses/>.
void acetoneiso::gmount()
{
QMessageBox::warning(this, "AcetoneISO::Warning!",tr("NOTE: If you want to mount an image in a root folder like /media/cdrom, please launch AcetoneISO as root user. "));
QDir Home = QDir::home();
QFile file(Home.path() + "/.acetoneiso/acetoneiso.conf");
   file.open(QIODevice::ReadOnly);//lo apro in sola lettura
   QTextStream in(&file);
   QString stra = in.readLine();
   QString strb = in.readLine();
   QString strc = in.readLine();

   QFile Mtab("/etc/mtab");//dichiaro un file Mtab che sarebbe /etc/mtab
   Mtab.open(QIODevice::ReadOnly);//lo apro in sola lettura
   QString str1 = (Mtab.readAll());//leggo il contenuto del file mtab
QString isodamontare;

isodamontare = QFileDialog::getOpenFileName(this,tr("Open Image"), Home.path() , tr("Image Files (*.iso *.daa *.bin *.mdf *.ashdisc *.bwi *.b5i *.lcd *.img *.cdi *.cif *.p01 *.pdi *.nrg *.ncd *.pxi *.gi *.fcd *.vcd *.c2d)"));

if ( !isodamontare.isNull() ) {
//mi ricavo l'estensione dell'immagine
   QFileInfo getinfo(isodamontare);
   QString ext = getinfo.suffix();  // ext = "iso?mdf?xyz?"
	if (((ext != "iso") && (ext != "nrg")) && ((ext != "bin") && (ext != "img")) && (ext != "mdf")) {
           QMessageBox msgBox;
           msgBox.setText(tr("The image ") + isodamontare + tr(" can't be mounted. You must convert it to ISO or extract content to a folder.\nPlease choose what to do:") );
           QPushButton *connectButton = msgBox.addButton(tr(" Convert to ISO "), QMessageBox::ActionRole);
           QPushButton *connectButton2 = msgBox.addButton(tr(" Extract image to a folder "), QMessageBox::ActionRole);
//msgBox.setStandardButtons(QMessageBox::("ISO File") | QMessageBox::("CD/DVD"));
           msgBox.exec();
 	      if (msgBox.clickedButton() == connectButton) {
	         converter();
	         }
 	      else if (msgBox.clickedButton() == connectButton2) {
	         extract();
	         }
 	 }
         else {
            QString folder = QFileDialog::getExistingDirectory( this, tr("AcetoneISO::Select where to mount image"), Home.path() );
            if (!folder.isNull()) {
               folder.chop(1);//casini del qt...il chop serve per togliere l'ultimo slash (/) dal percorso altrimenti in mtab non troverà mai il percorso con lo slash finale..
               if(!str1.contains( folder ) ) {
                   FUse.start("fuseiso",QStringList()  << isodamontare << folder << "-o" << "nonempty"  );
		   if(strc.contains("kde", Qt::CaseInsensitive)) {
		      BRowser.startDetached("kfmclient",QStringList() << "exec" << folder );
		    }
		else {
		BRowser.startDetached("nautilus",QStringList() << folder );
		}
               }
              else
	       QMessageBox::warning(this, "AcetoneISO::Warning!",tr("The folder ") + folder + tr(" can't be mounted"));
             }
           }
   }
   file.close();
   Mtab.close();
}

void acetoneiso::gumount()
{
QDir Home = QDir::home();
QFile Mtab("/etc/mtab");//dichiaro un file Mtab che sarebbe /etc/mtab
Mtab.open(QIODevice::ReadOnly);//lo apro in sola lettura
QString str1 = (Mtab.readAll());//leggo il contenuto del file mtab
QString folder = QFileDialog::getExistingDirectory( this, tr("AcetoneISO::Select folder to unmount"), Home.path() );
folder.chop(1);
if(str1.contains( folder ) ) {
   FUsermount.start("fusermount",QStringList() << "-u" << "-z" << folder );
 }
else {
   QMessageBox::warning(this, "AcetoneISO::Warning!",tr("The folder ") + folder + tr(" is not mounted!"));
 }
 Mtab.close();
}

