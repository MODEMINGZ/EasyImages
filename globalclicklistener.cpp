// GlobalClickListener.cpp
#include "globalclicklistener.h"
#include <QApplication>
#include <QGuiApplication>

GlobalClickListener::GlobalClickListener(QWidget *parent)
    : QWidget(parent, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool) {
    // 设置窗口属性为无边框和工具窗口
    // 确保窗口是透明的
    setAttribute(Qt::WA_TranslucentBackground);
    // 安装事件过滤器以捕获所有事件
    qApp->installEventFilter(this);
    // 显示全屏
    showFullScreen();
}

GlobalClickListener::~GlobalClickListener() {
    // 移除事件过滤器
    qApp->removeEventFilter(this);
}

bool GlobalClickListener::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress && qobject_cast<QWidget*>(obj) == nullptr) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        emit globalMouseClicked(mouseEvent->pos());
        return true; // 已经处理了这个事件，不再传递
    }
    return QObject::eventFilter(obj, event);
}

void GlobalClickListener::mousePressEvent(QMouseEvent *event) {
    // 如果需要处理窗口内部的点击事件，可以在这里添加代码
    QWidget::mousePressEvent(event);
}
