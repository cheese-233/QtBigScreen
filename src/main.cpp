#include <QApplication>
#include <QDir>
#include "global.h"
#include "httplistener.h"
#include "requestmapper.h"
#include "icon.h"
#include "Toast.h"
#include "addon.h"
#include <QProcess>
#include <Windows.h>
#include <QFile>
using namespace stefanfrings;
QProcess myPro;
/** Search the configuration file */
QString dir1;
QString searchConfigFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString fileName("Settings.ini");
    QStringList searchList;
    searchList.append(binDir);
    searchList.append(binDir+"/etc");
    searchList.append(binDir+"/../etc");
    searchList.append(binDir+"/../QtBigScreen/etc"); // for development with shadow build (Linux)
    searchList.append(binDir+"/../../QtBigScreen/etc"); // for development with shadow build (Windows)
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (dir1, searchList)
    {
        QFile file(dir1+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(fileName));
    return nullptr;
}
bool xCopyFile(QString source, QString destination)
{
    QDir source_dir(source);
    source_dir.mkpath(source);
    QString str="xcopy \""+source+"\" \""+destination+"\" /y /E /i /q &";
    qDebug() << str;
    system(str.toStdString().c_str());
    return false;
}
bool initRealDocroot(){
    QDir Real_docroot = dir1+"\\real_docroot\\index.html";
    if(!Real_docroot.exists()){
        return xCopyFile(dir1+"\\source",dir1+"\\real_docroot");
    }
    return 0;
}
/**
  Entry point of the program.
*/
int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    QApplication::setQuitOnLastWindowClosed(false);
    HANDLE hMutex = CreateMutex(NULL, TRUE, LPCWSTR(app.applicationName().toStdString().c_str()));
    if(hMutex != NULL)
    {
        if(GetLastError() == ERROR_ALREADY_EXISTS)
        {
            Toast::showTip("程序已经在运行!");
            int a = GetCurrentProcessId();
            system("TASKKILL /F /PID "+QByteArray::number(a)+" /T");
            return 1;
        }
    }
    app.setApplicationName("Server");
    // Find the configuration file
    QString configFileName=searchConfigFile();

    // Configure logging into a file
    QSettings* logSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    logSettings->beginGroup("logging");
    FileLogger* logger=new FileLogger(logSettings,10000,&app);
    logger->installMsgHandler();

    // Configure template loader and cache
    QSettings* templateSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    templateSettings->beginGroup("templates");
    templateCache=new TemplateCache(templateSettings,&app);

    // Configure session store
    QSettings* sessionSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    sessionSettings->beginGroup("sessions");
    sessionStore=new HttpSessionStore(sessionSettings,&app);

    icon i;
    i.showicon();
    initRealDocroot();
    addon ad;
    ad.BinDir(dir1);
    ad.init();

    // Configure static file controller
    QSettings* fileSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    fileSettings->beginGroup("docroot");
    staticFileController=new StaticFileController(fileSettings,&app);

    // Configure and start the TCP listener
    QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings,new RequestMapper(&app),&app);
    qWarning("Application has started");
    app.exec();
    qWarning("Application has stopped");
}

