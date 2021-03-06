#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include "widget.hxx"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    Widget w;
    w.show();

    return a.exec();
}
