#ifndef ADDON_H
#define ADDON_H
#pragma execution_character_set("utf-8")
#include <QObject>
#include <QCoreApplication>

class addon : public QObject
{
    Q_OBJECT
public:
    explicit addon(QObject *parent = nullptr);
public slots:
    bool init();
    void BinDir(QString bindir);
signals:

};

#endif // ADDON_H
