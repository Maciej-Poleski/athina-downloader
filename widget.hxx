#ifndef WIDGET_HXX
#define WIDGET_HXX

#include "ui_widget.h"

class Widget : public QWidget, private Ui::Widget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

signals:
    void readyForNextFetch();

protected:
    void changeEvent(QEvent *e);

private slots:
    void displaySource();
    void beginFetching();
    void endFetching();

private:
    QStringList list;
    QStringList::iterator i;
};

#endif // WIDGET_HXX
