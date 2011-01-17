/* Copyright (C) 2007 Jan Kundrát <jkt@gentoo.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "qtest_kde.h"

#include "test_Imap_LowLevelParser.h"
#include "test_Imap_LowLevelParser.moc"

#include "Imap/Exceptions.h"

typedef QPair<QByteArray,Imap::LowLevelParser::ParsedAs> StringWithKind;

Q_DECLARE_METATYPE(QDateTime) // needed for Qt-4.1

void ImapLowLevelParserTest::testParseList()
{
    using namespace Imap::LowLevelParser;

    QByteArray line = "()";
    int start = 0;
    QVariant res;
    QVariantList list;

    res = parseList( '(', ')', line, start );
    QCOMPARE( res, QVariant( QVariantList() ) );
    QCOMPARE( start, line.size() );

    line = "() 123";
    start = 0;
    res = parseList( '(', ')', line, start );
    QCOMPARE( res, QVariant( QVariantList() ) );
    QCOMPARE( line.mid(start), QByteArray(" 123") );

    line = "(smrt)";
    start = 0;
    res = parseList( '(', ')', line, start );
    Q_ASSERT( res.canConvert( QVariant::List ) );
    list = res.toList();
    QCOMPARE( list.size(), 1 );
    QCOMPARE( list, QVariantList() <<  "smrt" );
    QCOMPARE( line.size(), start );

    line = "[\\smrt] ahoj";
    start = 0;
    res = parseList( '[', ']', line, start );
    Q_ASSERT( res.canConvert( QVariant::List ) );
    list = res.toList();
    QCOMPARE( list.size(), 1 );
    QCOMPARE( list, QVariantList() <<  "\\smrt" );
    QCOMPARE( line.at(start), ' ' );
    ++start;
    QCOMPARE( line.at(start), 'a' );

    line = "(smrt [666] (999 1337) 3)";
    start = 0;
    res = parseList( '(', ')', line, start );
    Q_ASSERT( res.canConvert( QVariant::List ) );
    list = res.toList();
    QCOMPARE( list.size(), 4 );
    QCOMPARE( list, QVariantList() <<  "smrt" <<
            QVariant( QVariantList() << 666) << 
            QVariant( QVariantList() << 999 << 1337 ) << 3 );
    QCOMPARE( line.size(), start );

    try {
        line = "ahoj";
        start = 0;
        res = parseList( '(', ')', line, start );
        QFAIL( "parseList() against a string parameter should have thrown an exception" );
    } catch ( Imap::UnexpectedHere& ) {
        QVERIFY( true );
    }

    line = "(ahoj cau {6}\r\nnazdar 1337 \\* 666 \"aho\\\"oooj\") (bleee \\*)";
    start = 0;
    res = parseList( '(', ')', line, start );
    Q_ASSERT( res.canConvert( QVariant::List ) );
    list = res.toList();
    QCOMPARE( list.size(), 7 );
    QCOMPARE( list, QVariantList() <<  "ahoj" << "cau" << "nazdar" << 1337 << "\\*" << 666 << "aho\"oooj" );
    QVERIFY( start < line.size() );
    QCOMPARE( line.at( start ), ' ' );
    ++start;
    QVERIFY( start < line.size() );
    res = parseList( '(', ')', line, start );
    Q_ASSERT( res.canConvert( QVariant::List ) );
    list = res.toList();
    QCOMPARE( list.size(), 2 );
    QCOMPARE( list, QVariantList() << "bleee" << "\\*" );
    QCOMPARE( start, line.size() );
}

void ImapLowLevelParserTest::testGetString()
{
    using namespace Imap::LowLevelParser;

    QByteArray line = "ahOj";
    int pos = 0;
    StringWithKind res;

    try {
        line = ""; pos = 0;
        res = getString( line, pos );
        QFAIL( "getString() should scream on empty line" );
    } catch ( Imap::NoData& ) {
        QCOMPARE( pos, 0 );
    }

    try {
        line = "ah0j 333"; pos = 0;
        res = getString( line, pos );
        QFAIL( "getString() should ignore atoms" );
    } catch ( Imap::UnexpectedHere& ) {
        QCOMPARE( pos, 0 );
    }

    line = "{0}\r\na"; pos = 0;
    res = getString( line, pos );
    QCOMPARE( res.first.size(), 0 );
    QCOMPARE( res.second, LITERAL );
    QCOMPARE( pos, line.size() - 1 );

    line = "{3}\r\n666"; pos = 0;
    res = getString( line, pos );
    QCOMPARE( res.first, QByteArray("666") );
    QCOMPARE( res.second, LITERAL );
    QCOMPARE( pos, line.size() );

    QByteArray myData;
    myData.append('\x00');
    myData.append("\x01\x02\x03 abcde");
    line = QByteArray("{") + QByteArray::number( myData.size() ) + QByteArray("}\r\n");
    line.append(myData);
    pos = 0;
    res = getString( line, pos );
    QCOMPARE( res.first, myData );
    QCOMPARE( res.second, LITERAL );
    QCOMPARE( pos, line.size() );

    line = "\"333\\\\ \\\" 666\"a"; pos = 0;
    res = getString( line, pos );
    QCOMPARE( res.first, QByteArray("333\\ \" 666") );
    QCOMPARE( res.second, QUOTED );
    QCOMPARE( line.at(pos), 'a' );

    line = "\"\"x"; pos = 0;
    res = getString( line, pos );
    QCOMPARE( res.first.size(), 0 );
    QCOMPARE( res.second, QUOTED );
    QCOMPARE( pos, line.size() - 1 );

}

void ImapLowLevelParserTest::testGetUInt()
{
    using namespace Imap::LowLevelParser;

    QByteArray line = "123456 789 a666b333";
    uint res;
    int pos = 0;

    res = getUInt( line, pos );
    QCOMPARE( res, 123456u );
    ++pos;

    res = getUInt( line, pos );
    QCOMPARE( res, 789u );
    ++pos;

    try {
        res = getUInt( line, pos );
        QFAIL("exception not raised");
    } catch (Imap::ParseError& e) {
        QCOMPARE( pos, 11 );
        ++pos;
    }

    res = getUInt( line, pos );
    QCOMPARE( res, 666u );

    QCOMPARE( pos, 15 );
    ++pos;

    res = getUInt( line, pos );
    QCOMPARE( res, 333u );

    Q_ASSERT( pos == line.size() );
}

void ImapLowLevelParserTest::testGetAtom()
{
    using namespace Imap::LowLevelParser;

    QByteArray line = "blesmrt troj1ta s matovou\nomackou";
    int pos = 0;

    QCOMPARE( getAtom( line, pos ), QByteArray("blesmrt") );
    ++pos;
    QCOMPARE( getAtom( line, pos ), QByteArray("troj1ta") );
    ++pos;
    QCOMPARE( getAtom( line, pos ), QByteArray("s") );
    ++pos;
    QCOMPARE( getAtom( line, pos ), QByteArray("matovou") );
    ++pos;
    QCOMPARE( getAtom( line, pos ), QByteArray("omackou") );
    Q_ASSERT( pos == line.size() );
}

void ImapLowLevelParserTest::testGetAString()
{
    using namespace Imap::LowLevelParser;

    QByteArray line = "ahOj";
    int pos = 0;

    StringWithKind res = getAString( line, pos );
    QCOMPARE( res.first, QByteArray("ahOj") );
    QCOMPARE( res.second, ATOM );
    QCOMPARE( pos, line.size() );

    line = "ah0j 333"; pos = 0;
    res = getAString( line, pos );
    QCOMPARE( res.first, QByteArray("ah0j") );
    QCOMPARE( res.second, ATOM );
    ++pos;
    res = getAString( line, pos );
    QCOMPARE( res.first, QByteArray("333") );
    QCOMPARE( pos, line.size() );

    line = "\"ah0j 333\""; pos = 0;
    res = getAString( line, pos );
    QCOMPARE( res.first, QByteArray("ah0j 333") );
    QCOMPARE( res.second, QUOTED );
    QCOMPARE( pos, line.size() );

    line = "{8}\r\nah0j 333"; pos = 0;
    res = getAString( line, pos );
    QCOMPARE( res.first, QByteArray("ah0j 333") );
    QCOMPARE( res.second, LITERAL );
    QCOMPARE( pos, line.size() );
}

void ImapLowLevelParserTest::testGetAnything()
{
    using namespace Imap::LowLevelParser;
    QByteArray line = "ah0j";
    int pos = 0;
    QVariant res;

    res = getAnything( line, pos );
    QCOMPARE( line, res.toByteArray() );
    QCOMPARE( pos, line.size() );

    line = "ahoj cau";
    pos = 0;
    res = getAnything( line, pos );
    QCOMPARE( QByteArray("ahoj"), res.toByteArray() );
    QCOMPARE( line.at( pos ), ' ' );
    ++pos;
    res = getAnything( line, pos );
    QCOMPARE( QByteArray("cau"), res.toByteArray() );
    QCOMPARE( pos, line.size() );

    line = "1337"; pos = 0;
    res = getAnything( line, pos );
    Q_ASSERT( res.canConvert( QVariant::Int ) );
    QCOMPARE( 1337, res.toInt() );
    QCOMPARE( pos, line.size() );

    line = "0 666";
    pos = 0;
    res = getAnything( line, pos );
    Q_ASSERT( res.canConvert( QVariant::Int ) );
    QCOMPARE( 0, res.toInt() );
    QCOMPARE( line.at( pos ), ' ' );
    ++pos;
    res = getAnything( line, pos );
    Q_ASSERT( res.canConvert( QVariant::Int ) );
    QCOMPARE( 666, res.toInt() );
    QCOMPARE( pos, line.size() );

    line = "Blesmrt[trojita (666 333 1337)]"; pos = 0;
    res = getAnything( line, pos );
    QCOMPARE( line, res.toByteArray() );
    QCOMPARE( pos, line.size() );

    line = "Blesmrt[trojita (666 333 1337)] ahoj"; pos = 0;
    res = getAnything( line, pos );
    QCOMPARE( QByteArray("Blesmrt[trojita (666 333 1337)]"), res.toByteArray() );
    QCOMPARE( line.mid(pos), QByteArray(" ahoj") );

    line = "Blesmrt[trojita (666 333 1337)]<1337> z666"; pos = 0;
    res = getAnything( line, pos );
    QCOMPARE( QByteArray("Blesmrt[trojita (666 333 1337)]<1337>"), res.toByteArray() );
    QCOMPARE( line.mid(pos), QByteArray(" z666") );
}

void ImapLowLevelParserTest::testGetRFC2822DateTime()
{
    QFETCH( QString, line );
    QFETCH( QDateTime, date );
    QDateTime d;
    try {
        d = Imap::LowLevelParser::parseRFC2822DateTime( line );
    } catch ( Imap::ParseError& e ) {
        qDebug() << e.what();
    }
    QCOMPARE( d, date );
}

void ImapLowLevelParserTest::testGetRFC2822DateTime_data()
{
    QTest::addColumn<QString>("line");
    QTest::addColumn<QDateTime>("date");

    /*
      courier-imap on woodpecker.gentoo.org won't convert the following:
    QTest::newRow("date-no-leading-zero")
        << QString("Fri Apr  3 00:21:52 UTC 2009")
        << QDateTime( QDate( 2009, 4, 3 ), QTime( 0, 21, 52), Qt::UTC );*/

    QTest::newRow("date-manual")
        << QString("Wed, 09 Apr 2008 20:16:12 +0200")
        << QDateTime( QDate( 2008, 4, 9 ), QTime( 18, 16, 12 ), Qt::UTC );

    QTest::newRow("date-wo-weekday")
        << QString("09 Apr 2008 20:16:12 +0200")
        << QDateTime( QDate( 2008, 4, 9 ), QTime( 18, 16, 12 ), Qt::UTC );

    QTest::newRow("date-no-weekday-no-zone")
        << QString("08 Apr 2009 03:00:19")
        << QDateTime( QDate( 2009, 4, 8 ), QTime( 3, 0, 19 ), Qt::UTC );

    QTest::newRow("date-no-zone")
        << QString("Wed, 08 Apr 2009 03:00:19")
        << QDateTime( QDate( 2009, 4, 8 ), QTime( 3, 0, 19 ), Qt::UTC );

    QTest::newRow("date-ut")
        << QString("Wed, 09 Apr 2008 20:16:12 UT")
        << QDateTime( QDate( 2008, 4, 9 ), QTime( 20, 16, 12 ), Qt::UTC );

    QTest::newRow("date-gmt")
        << QString("Wed, 09 Apr 2008 20:16:12 gMt")
        << QDateTime( QDate( 2008, 4, 9 ), QTime( 20, 16, 12 ), Qt::UTC );

    QTest::newRow("date-est")
        << QString("Wed, 09 Apr 2008 13:16:12 est")
        << QDateTime( QDate( 2008, 4, 9 ), QTime( 18, 16, 12 ), Qt::UTC );

    QTest::newRow("date-edt")
        << QString("Wed, 09 Apr 2008 13:16:12 edt")
        << QDateTime( QDate( 2008, 4, 9 ), QTime( 17, 16, 12 ), Qt::UTC );

    QTest::newRow("date-cst")
        << QString("Wed, 09 Apr 2008 10:17:12 CST")
        << QDateTime( QDate( 2008, 4, 9 ), QTime( 16, 17, 12 ), Qt::UTC );

    QTest::newRow("date-cdt")
        << QString("Wed, 09 Apr 2008 20:16:12 CDT")
        << QDateTime( QDate( 2008, 4, 10 ), QTime( 1, 16, 12 ), Qt::UTC );

    QTest::newRow("date-mst")
        << QString("wEd, 09 APr 2001 20:16:12 mst")
        << QDateTime( QDate( 2001, 4, 10 ), QTime( 3, 16, 12 ), Qt::UTC );

    QTest::newRow("date-mdt")
        << QString("Wed, 09 Apr 2008 20:16:12 mdT")
        << QDateTime( QDate( 2008, 4, 10 ), QTime( 2, 16, 12 ), Qt::UTC );

    QTest::newRow("date-pst")
        << QString("Wed, 09 Apr 2008 20:16:12 pst")
        << QDateTime( QDate( 2008, 4, 10 ), QTime( 4, 16, 12 ), Qt::UTC );

    QTest::newRow("date-pdt")
        << QString("Wed, 09 Apr 2008 20:16:12 pdt")
        << QDateTime( QDate( 2008, 4, 10 ), QTime( 3, 16, 12 ), Qt::UTC );

    QTest::newRow("date-a")
        << QString("Wed, 09 Apr 2008 20:16:12 a")
        << QDateTime( QDate( 2008, 4, 9 ), QTime( 20, 16, 12 ), Qt::UTC );

    int month = 1;
    int day = 1;
    QTime time( 0, 0, 0 );
    QStringList months = QStringList() << "Jan" << "FEb" << "mar" << "Apr" <<
        "May" << "Jun" << "Jul" << "AUG" << "Sep" << "Oct" << "nOV" << "Dec";
    QStringList wDays = QStringList() << "Mon" << "tue" << "WED" << "ThU" <<
        "Fri" << "Sat" << "Sun";
    int tz = 11*60;
    bool plusOrSpace = false;
    for ( int year = 1970; year < 2035; year += 1, plusOrSpace = !plusOrSpace ) {
        QDateTime date( QDate( year, month, day), time, Qt::UTC );
        QString str = date.toString( "%1, dd %2 yyyy hh:mm:ss %3%4%5" ).arg(
                        wDays[ date.date().dayOfWeek() - 1 ]
                    ).arg(
                        months[ month - 1 ]
                    ).arg(
                        ( tz >= 12* 60 ) ? ( plusOrSpace ? "" : "+" ) : "-"
                    ).arg(
                        qAbs(int( ( tz - 12*60 ) / 60 )), 2, 10, QChar('0')
                    ).arg (
                        qAbs( ( tz - 12 * 60 ) % 60 ), 2, 10, QChar('0') );
        date = date.addSecs( - 60 * tz + 12*3600 );
        QTest::newRow("date-generated") << str << date;

        month = qMax( ( month + 1 ) % 12, 1 );
        day = qMax( ( day + 3 ) % 31, 1 );
        time = time.addSecs( 3600 * 13 + 60 * 27 + 17 );
        tz = ( tz + 30 ) % ( 24 * 60 );
    }
    
}

QTEST_KDEMAIN_CORE( ImapLowLevelParserTest )

namespace QTest {

template<> char * toString( const QPair<QStringList,QByteArray>& res )
{
    QByteArray buf;
    QTextStream stream( &buf );
    stream << "first: " << res.first.join(", ") << ", second: " << res.second;
    stream.flush();
    return qstrdup( buf.data() );
}

}
