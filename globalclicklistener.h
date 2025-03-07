// GlobalClickListener.h
#ifndef GLOBALCLICKLISTENER_H
#define GLOBALCLICKLISTENER_H

#include <QWidget>
#include <QMouseEvent>
#include <QScreen>

class GlobalClickListener : public QWidget {
    Q_OBJECT

public:
    GlobalClickListener(QWidget *parent = nullptr);
    ~GlobalClickListener();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void globalMouseClicked(const QPoint &globalPos);
};

#endif // GLOBALCLICKLISTENER_H
