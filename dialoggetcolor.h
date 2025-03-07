#ifndef DIALOGGETCOLOR_H
#define DIALOGGETCOLOR_H

#include <QDialog>
#include "globalclicklistener.h"
namespace Ui {
class DialogGetColor;
}

class DialogGetColor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGetColor(QWidget *parent = nullptr);
    ~DialogGetColor();
    QColor getColor();
    void getingColor();
    void setColor();
private:
    Ui::DialogGetColor *ui;
        GlobalClickListener *globalClickListener;
protected:
           // void mousePressEvent(QMouseEvent *event);
           // void mouseReleaseEvent(QMouseEvent *event);
           // void mouseDoubleClickEvent(QMouseEvent *event);
           // void mouseMoveEvent(QMouseEvent *event);
public slots:
    void onGlobalMouseClicked(const QPoint &globalPos);
private slots:
    void on_pushButton_ok_clicked();
};

#endif // DIALOGGETCOLOR_H
