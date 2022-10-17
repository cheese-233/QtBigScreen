#include "icon.h"
#include "Toast.h"
#include <Windows.h>
#include "QtNetwork/qnetworkinterface.h"
#include <QClipboard>
#include <QSettings>
#include <QTextCodec>
#include <QDir>

QSystemTrayIcon *tray;
icon::icon(QWidget *parent)
    : QMainWindow(parent)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
}
void RegAppAutoStart()
{
    const QString REG_RUN = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    QSettings regSettings(REG_RUN, QSettings::NativeFormat);
    QString appName = QApplication::applicationName();
    if (regSettings.contains(appName))
    {
        regSettings.remove(appName);
        Toast::showTip("已取消开机启动");
    }
    else
    {
        QString appPath = QApplication::applicationFilePath();
        regSettings.setValue(appName, appPath.replace("/", "\\"));
        Toast::showTip("已设置开机启动");
    }
}
bool XCopyFile(QString source, QString destination)
{
    QDir source_dir(source);
    source_dir.mkpath(source);
    QString str="xcopy \""+source+"\" \""+destination+"\" /y /E /i /q &";
    qDebug() << str;
    system(str.toStdString().c_str());
    return false;
}
std::map<QString,QString> getNetWorkInfo()
{
    std::map<QString, QString> mapNetWorkInfo;
    mapNetWorkInfo["ip"] = "127.0.0.1";
    mapNetWorkInfo["hardwareAddress"] = "null";

    auto interFaces = QNetworkInterface::allInterfaces();
    qDebug() << QString("QNetworkInterface size : %1").arg(interFaces.size());

    for (QNetworkInterface netInterface : interFaces)
    {
        auto nflags = netInterface.flags();
        if (nflags & QNetworkInterface::IsLoopBack
            || !(nflags & QNetworkInterface::IsRunning))
        {
            continue;
        }

        auto entryList = netInterface.allAddresses();
        for(auto entry: entryList)
        {
            auto ipItem = entry;
            if (ipItem.protocol() == QAbstractSocket::IPv4Protocol && ipItem != QHostAddress::Null && !(ipItem.QHostAddress::isLoopback()) && !(ipItem.isLinkLocal()) && !(ipItem.toString().split(".")[3] == "1"))
            {
                qDebug() << "Find ip :"+ipItem.toString();
                mapNetWorkInfo["ip"] = ipItem.toString();
            }
        }

        qDebug() << QString("QNetworkInterface return : ") + mapNetWorkInfo["name"];
        break;
    }
    return mapNetWorkInfo;
}
void icon::showicon(){
    qDebug("Icon: created");
    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/ie.png"));
    tray->setToolTip("服务器运行中");
    connect(tray , SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onActivated(QSystemTrayIcon::ActivationReason)));
    QMenu *menuTray = new QMenu;
    auto netWorkInfo = getNetWorkInfo();
    QAction *actIp = new QAction("当前地址:"+netWorkInfo["ip"]+":10086", menuTray);
    actIp->setToolTip("点击复制");
    connect(actIp, SIGNAL(triggered()), this, SLOT(paste()));
    menuTray->addAction(actIp);
    QAction *actAutoRun = new QAction("开机启动", menuTray);
    connect(actAutoRun, SIGNAL(triggered()), this, SLOT(AutoRun()));
    menuTray->addAction(actAutoRun);
    QAction *actQuit = new QAction("退出", menuTray);
    connect(actQuit, SIGNAL(triggered()), this, SLOT(stop()));
    menuTray->addAction(actQuit);
    tray->setContextMenu(menuTray);
    tray->show();
}
void icon::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    {
        break;
    }
    default:
        break;
    }
}
void icon::AutoRun(){
    RegAppAutoStart();
}
void icon::stop(){
    int a = GetCurrentProcessId();
    system("TASKKILL /F /PID "+QByteArray::number(a)+" /T");
}
void icon::paste(){
    auto netWorkInfo = getNetWorkInfo();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText("http://"+netWorkInfo["ip"]+":10086/");
}
