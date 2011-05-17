/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#ifndef __PCOMMON_H__
#define __PCOMMON_H__

#include <QString>
#include <QStringList>

//======== процедуры и функции общего назначения ============================
//---------------------------------------------------------------------------
QStringList SplitNumString(QString Str, QString Deviders);
int pSign(int x);                //сигнум. Возвращает 1 if x>0; -1 if x<0; 0 if x=0
//int pRound(double x);             //округляет вещ.число до целого;
double pRound(double x, int Accuracy); //округляет вещ.число до указанного знака;
QString pNumToStr(int N, QString One, QString Two, QString Five); //определяем склонение слова (день,дня,дней)
QString pBoolToStr(bool B);
bool pStrToBool(QString S);
QString pQString(char str[65000]);  //преобразуем тип убирая символ перевода строки
//дополняет строку до длины Len, выравнивая по Align (Left,Center,Right)
QString pFillString(QString Str, QString Chr, int Len, QString Align);
//QString FontStyleToString(TFont *FS);
//TFontStyles StringToFontStyle(QString FS);
//QString AlignmentToString(TAlignment A);
//TAlignment StringToAlignment(QString S);
QString GetOptionName(QString OptN);
QString GetOptionValue(QString OptN);
QString pMonthName(int n); //Полное название месяца из его номера
QString pMonthName(int n, QString format); //Название месяца из его номера в формате format
QString ZodiakName(int n);    //Название зодиакального зозвездия из его номера
int DaysInYear(long cYear);      //кол-во дней в году
int DaysInYear_Jul(long cYear);  //кол-во дней в году по Юлианскому календарю
bool LeapYear(long cYear);	//високосный ли год
bool LeapYear_Jul(long cYear);
int HoursInYear(long cYear);   //кол-во часов в году
int MinutesInYear(long cYear); //кол-во минут в году
int SecondsInYear(long cYear); //кол-во секунд в году
int Digit(int Number);  //ссумирует цифры числа между собой до получения одной цифры
QString IntToLetter(int n); //возвращает букву латинского алфавита
QString IntToLetter(int n, QString str); //возвращает букву из строки
int LetterToInt(QString letter); //возвращает номер буквы латинского алфавита
int LetterToInt(QString letter, QString srcAlphabet);//возвращает номер буквы в указанном алфавите
int r2i(QChar letter); //вспомогательная функция для RomanToInt()
int RomanToInt(QString romanInt); //переводит латинские числа в арабские
QString IntToRoman(int n); //переводит арабские числа в латинские
int max(int x1, int x2, int x3); //возвращает максимальный из аргументов
bool SameDigits(int x); //true если все цифры числа одинаковы, напр. 777
double Proporsion(double x, double y);//вычисляет пропорцию

//======= работа с файлами =======
//--------------------------------
QString unurlifyFileName(const QString &fileName);		//remove "file:"
QString urlifyFileName(const QString &fileName);		//add "file:"
QString relatifyFileName(QString url, QString path);	//returns path like ../../images/pict.jpg Difference from QDir::relativeFilePath() in: it adds "./" and removes "file:"
QStringList relatifyFileList(QStringList urls, QString path); 
QString absolutifyFileName(QString fn, QString path);	//returns absolute file path
QStringList absolutifyFileList(QStringList fns, QString path);	
QString removeAnchorFromLink(const QString &link);		//returns link without anchor
bool verifyDirectory(const QString &str);	//create dir if not exist and if there is no such file name
bool pathIsRelative(QString path);	//return true if path starts with ./ or ../
QString CreateValidWebFileName(QString str); //выбирает из строки только разрешённые символы
bool toLog(QString logFN, QString logMessage); //добавляет в журнал строку в формате "Дата Время Сообщение"
bool createEmptyHtml(QString fileName, QString title); 
QString uniqueFileName(QString fileName); //return a unique file name based on "fileName"

#endif // __PCOMMON_H__
