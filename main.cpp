#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    /* È«¾Ö×Ö·û±àÂë UTF-8 */
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
