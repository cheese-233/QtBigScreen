#ifndef ICON_H
#define ICON_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>
#include <QDebug>
#include <iostream>
#pragma execution_character_set("utf-8")
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class icon : public QMainWindow
{
    Q_OBJECT
public:
    explicit icon(QWidget *parent = nullptr);
public slots:
    void showicon();
    void stop();
    void paste();
    void AutoRun();
private:
    Ui::MainWindow *ui;
private slots:
    void onActivated(QSystemTrayIcon::ActivationReason reason);
signals:

};

#endif // ICON_H
