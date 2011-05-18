#include "widget.hxx"

#include <QtWebKit/QtWebKit>
#include <QtCore/QStringList>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    connect(webView,SIGNAL(loadFinished(bool)),this,SLOT(displaySource()));
    webView->setUrl(QUrl("http://2008.v-lo.tcs.uj.edu.pl/"));
    textBrowser->setPlainText("Czekaj...");
}

void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void Widget::displaySource()
{
    textBrowser->setPlainText("Zaloguj się na swoje konto i przejdź do \"Results\"");
    QString str = webView->page()->mainFrame()->toHtml();

    QRegExp rx("/\\d+\"");
    int pos = 0;

    while ((pos = rx.indexIn(str, pos)) != -1) {
        list<<rx.cap(0).remove('"');
        pos += rx.matchedLength();
    }

    if(list.size()>0)
    {
        webView->disconnect(this);
        QDir directory(qApp->applicationDirPath());
        directory.mkdir("athina");
        directory.cd("athina");
        if(!QDir::setCurrent(directory.absolutePath()))
        {
            textBrowser->setPlainText("Nie udało się przygotować katalogu na dane z Athiny. Nie mogę kontynuować.");
            return;
        }

        textBrowser->setPlainText("Rozpoczynam ściąganie twoich kodów do katalogu "+directory.absolutePath());

        QFile output("index.html");
        output.open(QIODevice::WriteOnly);
        output.write(webView->page()->mainFrame()->toHtml().toLocal8Bit());
        output.close();
        connect(this,SIGNAL(readyForNextFetch()),this,SLOT(beginFetching()));
        connect(webView,SIGNAL(loadFinished(bool)),this,SLOT(endFetching()));

        i=list.begin();
        emit readyForNextFetch();
    }
}

void Widget::beginFetching()
{
    textBrowser->append(QString("Ściągam submit ")+i->right(i->size()-1));
    webView->setUrl(QString("http://2008.v-lo.tcs.uj.edu.pl")+*i);
}

void Widget::endFetching()
{
    QFile output(i->right(i->size()-1)+".html");
    output.open(QIODevice::WriteOnly);
    output.write(webView->page()->mainFrame()->toHtml().toLocal8Bit());
    output.close();
    if(++i==list.end())
    {
        webView->disconnect(this);
        disconnect(this);
        textBrowser->append("Ściąganie zakończone.");
    }
    else
        emit readyForNextFetch();
}
