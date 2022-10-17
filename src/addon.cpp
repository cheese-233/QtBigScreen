#include "addon.h"
#include <QSettings>
#include <QtXml>
#include "Toast.h"

addon::addon(QObject *parent)
    : QObject{parent}
{
}
#include <QDir>
QString MainBinDir = "";
void addon::BinDir(QString bindir)
{
    MainBinDir = bindir;
    qDebug() << MainBinDir;
}

QString searchAddonFile()
{
    QString binDir = MainBinDir;
    QStringList searchList;
    searchList.append(binDir + "/real_docroot/addon");
    searchList.append(binDir + "/real_docroot/addon");
    searchList.append(binDir + "/../etc/real_docroot/addon");
    searchList.append(binDir + "/../QtBigScreen/etc/real_docroot/addon");    // for development with shadow build (Linux)
    searchList.append(binDir + "/../../QtBigScreen/etc/real_docroot/addon"); // for development with shadow build (Windows)
    searchList.append(QDir::rootPath() + "etc/opt/real_docroot/addon");
    searchList.append(QDir::rootPath() + "etc/real_docroot/addon");
    QString dir;
    foreach (dir, searchList)
    {
        QDir file(dir);
        if (file.exists())
        {
            QString fileName = file.path();
            return fileName;
        }
    }

    // not found
    QString dir1;
    foreach (dir1, searchList)
    {
        qWarning("%s not found", qPrintable(dir1));
    }
    qFatal("Cannot find addon %s", qPrintable(dir1));
    return nullptr;
}
QString addonfolder;
QStringList FindFile(const QString &_filePath)
{
    QDir dir(_filePath);
    QStringList folderList = {};
    if (!dir.exists())
    {
        return {};
    }

    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    dir.setSorting(QDir::DirsFirst);

    QFileInfoList list = dir.entryInfoList();
    int i = 0;

    do
    {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isDir())
        {
            folderList.append(fileInfo.filePath());
        }
        i++;
    } while (i < list.size());

    return folderList;
}
int TableTr = 0;
int NavTr = 0;
int SettingTr = 0;
int LastTr = 0;
bool copyFile(QString sourceDir, QString toDir)
{
    toDir.replace("/", "\\");
    if (sourceDir == toDir)
    {
        return true;
    }
    if (!QFile::exists(sourceDir))
    {
        return false;
    }
    QDir *createfile = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist)
    {
        createfile->remove(toDir);
    }

    if (!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}
bool CopyAddons(QString configDir)
{
    QDir::setCurrent(configDir);
    QString config = configDir + "/config.ini";
    if (!(QFile(config).exists()))
    {
        qWarning() << "Can\'t find " + config;
        return 1;
    }
    QSettings *ini = new QSettings(config, QSettings::IniFormat);
    ini->setIniCodec(QTextCodec::codecForName("UTF-8"));
    // html
    QString html = ini->value("HTML/file", "").toString();
    // change index
    bool hascard = 0;
    QString body_title = "";
    QString body_pic = "";
    QString body_text = "";
    if (!(ini->value("ChangeIndex/body_title").isNull()))
    {
        hascard = 1;
        body_title = ini->value("ChangeIndex/body_title").toByteArray();
        body_pic = ini->value("ChangeIndex/body_pic").toByteArray();
        body_text = ini->value("ChangeIndex/body_text").toByteArray();
    }
    if (!(ini->value("Exec/exec").isNull()))
    {
        QString program = ini->value("Exec/exec").toString();
        qDebug() << "Using program " << program;
        QProcess *mProcess = new QProcess;
        mProcess->setProcessChannelMode(QProcess::MergedChannels);
        int background = 0;
        if (ini->value("Exec/background") == "1" || ini->value("Exec/background") == "true")
        {
            background = 1;
        }
        if (ini->value("Exec/block") == "1" || ini->value("Exec/block") == "true")
        {
            if (background == 1)
            {
                mProcess->start(program);
                mProcess->waitForFinished();
            }
            else
            {
                mProcess->startDetached(program);
                mProcess->waitForFinished();
            }
        }
        else
        {
            if (background == 1)
            {
                mProcess->start(program);
            }
            else
            {
                mProcess->startDetached(program);
            }
        }
    }
    bool isNavbar = 0;
    QString navbar_name;
    if (!(ini->value("navbar/name").isNull()))
    {
        navbar_name = ini->value("navbar/name").toString();
        isNavbar = 1;
    }
    bool has_setting = 0;
    QString SettingName;
    if (!(ini->value("Setting/name").isNull()))
    {
        SettingName = ini->value("Setting/name").toString();
        has_setting = 1;
    }
    // end of read
    if (hascard == 1)
    {
        QFile *js_file = new QFile(addonfolder + "\\..\\loadAddon.js");
        if (!js_file->open(QFile::WriteOnly | QFile::Append))
        {
            qFatal("Can't open js file " + js_file->fileName().toUtf8());
            return 1;
        }
        QTextStream outs(js_file);
        outs.setCodec(QTextCodec::codecForName("utf-8"));
        outs << "//This file is auto generate by QtBigScreen." << Qt::endl;
        outs << "//Your changes will NOT be saved!!!" << Qt::endl;
        QString img = ("/addon/" + QDir(configDir).dirName() + "/" + body_pic);
        if (TableTr % 3 == 0 || TableTr == 0)
        {
            outs << "var tr_" + QString::number(TableTr) + " = document.createElement('tr');" << Qt::endl;
            outs << "var td_" + QString::number(TableTr) + " = document.createElement('td');" << Qt::endl;
            outs << "var div_card_" + QString::number(TableTr) + " = document.createElement('div');" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".className = 'card borderless shadow blur';" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".onmouseover = function () { this.className = 'card border-primary shadow-lg blur' };" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".onmouseout = function () { this.className = 'card borderless shadow blur' };" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".onclick = function () { window.location.href = '" + ("/addon/" + QDir(configDir).dirName() + "/" + html) + "'; };" << Qt::endl;
            outs << "var div_card_img_" + QString::number(TableTr) + " = document.createElement('div');" << Qt::endl;
            outs << "div_card_img_" + QString::number(TableTr) + ".className = 'card-img-top borderless rounded-lg zoomImage';" << Qt::endl;
            outs << "div_card_img_" + QString::number(TableTr) + ".style = `background-image: url('" + (img) + "')`;" << Qt::endl;
            outs << "var div_card_body_" + QString::number(TableTr) + " = document.createElement('div');" << Qt::endl;
            outs << "div_card_body_" + QString::number(TableTr) + ".className = 'card-body';" << Qt::endl;
            outs << "var card_title_" + QString::number(TableTr) + " = document.createElement('h5');" << Qt::endl;
            outs << "card_title_" + QString::number(TableTr) + ".className = 'card-title';" << Qt::endl;
            outs << "card_title_" + QString::number(TableTr) + ".style = 'white-space: nowrap;';" << Qt::endl;
            outs << "card_title_" + QString::number(TableTr) + ".innerText = '" + (body_title) + "';" << Qt::endl;
            outs << "var card_text_" + QString::number(TableTr) + " = document.createElement('p');" << Qt::endl;
            outs << "card_text_" + QString::number(TableTr) + ".className = 'card-text';" << Qt::endl;
            outs << "card_text_" + QString::number(TableTr) + ".style = 'overflow-x:auto;white-space: nowrap;';" << Qt::endl;
            outs << "card_text_" + QString::number(TableTr) + ".appendChild(document.createTextNode('" + (body_text) + "'));" << Qt::endl;
            outs << "tr_" + QString::number(TableTr) + ".appendChild(td_" + QString::number(TableTr) + ");" << Qt::endl;
            outs << "td_" + QString::number(TableTr) + ".appendChild(div_card_" + QString::number(TableTr) + ");" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".appendChild(div_card_img_" + QString::number(TableTr) + ");" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".appendChild(div_card_body_" + QString::number(TableTr) + ");" << Qt::endl;
            outs << "div_card_body_" + QString::number(TableTr) + ".appendChild(card_title_" + QString::number(TableTr) + ");" << Qt::endl;
            outs << "div_card_body_" + QString::number(TableTr) + ".appendChild(card_text_" + QString::number(TableTr) + ");" << Qt::endl;
            outs << "document.getElementsByTagName('table')[0].appendChild(tr_" + QString::number(TableTr) + ");" << Qt::endl;
            LastTr = TableTr;
        }
        else
        {
            outs << "var tr_" + QString::number(TableTr) + " = document.createElement('tr');" << Qt::endl;
            outs << "var td_" + QString::number(TableTr) + " = document.createElement('td');" << Qt::endl;
            outs << "var div_card_" + QString::number(TableTr) + " = document.createElement('div');" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".className = 'card borderless shadow blur';" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".onmouseover = function () { this.className = 'card border-primary shadow-lg blur' };" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".onmouseout = function () { this.className = 'card borderless shadow blur' };" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".onclick = function () { window.location.href = '" + ("/addon/" + (QDir(configDir).dirName() + "/" + html)) + "'; };" << Qt::endl;
            outs << "var div_card_img_" + QString::number(TableTr) + " = document.createElement('div');" << Qt::endl;
            outs << "div_card_img_" + QString::number(TableTr) + ".className = 'card-img-top borderless rounded-lg zoomImage';" << Qt::endl;
            outs << "div_card_img_" + QString::number(TableTr) + ".style = `background-image: url('" + img + "')`;" << Qt::endl;
            outs << "var div_card_body_" + QString::number(TableTr) + " = document.createElement('div');" << Qt::endl;
            outs << "div_card_body_" + QString::number(TableTr) + ".className = 'card-body';" << Qt::endl;
            outs << "var card_title_" + QString::number(TableTr) + " = document.createElement('h5');" << Qt::endl;
            outs << "card_title_" + QString::number(TableTr) + ".className = 'card-title';" << Qt::endl;
            outs << "card_title_" + QString::number(TableTr) + ".style = 'white-space: nowrap;';" << Qt::endl;
            outs << "card_title_" + QString::number(TableTr) + ".innerText = '" + (body_title) + "';" << Qt::endl;
            outs << "var card_text_" + QString::number(TableTr) + " = document.createElement('p');" << Qt::endl;
            outs << "card_text_" + QString::number(TableTr) + ".className = 'card-text';" << Qt::endl;
            outs << "card_text_" + QString::number(TableTr) + ".style = 'overflow-x:auto;white-space: nowrap;';" << Qt::endl;
            outs << "card_text_" + QString::number(TableTr) + ".appendChild(document.createTextNode('" + (body_text) + "'));" << Qt::endl;
            outs << "tr_" + QString::number(LastTr) + ".appendChild(td_" + QString::number(TableTr) + ");" << Qt::endl;
            outs << "td_" + QString::number(TableTr) + ".appendChild(div_card_" + QString::number(TableTr) + ");" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".appendChild(div_card_img_" + QString::number(TableTr) + ");" << Qt::endl;
            outs << "div_card_" + QString::number(TableTr) + ".appendChild(div_card_body_" + QString::number(TableTr) + ");" << Qt::endl;
            outs << "div_card_body_" + QString::number(TableTr) + ".appendChild(card_title_" + QString::number(TableTr) + ");" << Qt::endl;
            outs << "div_card_body_" + QString::number(TableTr) + ".appendChild(card_text_" + QString::number(TableTr) + ");" << Qt::endl;
        }
        TableTr++;
        js_file->close();
    }
    if (isNavbar)
    {
        QFile *js_file = new QFile(addonfolder + "\\..\\LoadNavBar.js");
        if (!js_file->open(QFile::WriteOnly | QFile::Append))
        {
            qFatal("Can't open js file " + js_file->fileName().toLocal8Bit());
            return 1;
        }
        QTextStream outs(js_file);
        outs.setCodec(QTextCodec::codecForName("utf-8"));
        outs << "//This file is auto generate by QtBigScreen." << Qt::endl;
        outs << "//Your changes will NOT be saved!!!" << Qt::endl;
        outs << "var li_" + QString::number(NavTr) + " = document.createElement('li');" << Qt::endl;
        outs << "li_" + QString::number(NavTr) + ".className = 'nav-item'" << Qt::endl;
        outs << "var a_" + QString::number(NavTr) + " = document.createElement('a');" << Qt::endl;
        outs << "a_" + QString::number(NavTr) + ".className = 'nav-link';" << Qt::endl;
        outs << "a_" + QString::number(NavTr) + ".href = '" + ("/addon/" + QDir(configDir).dirName() + "/" + html) + "';" << Qt::endl;
        outs << "a_" + QString::number(NavTr) + ".innerHTML = '" + (navbar_name) + "';" << Qt::endl;
        outs << "li_" + QString::number(NavTr) + ".appendChild(a_" + QString::number(NavTr) + ");" << Qt::endl;
        outs << "document.getElementsByTagName('ul')[0].appendChild(li_" + QString::number(NavTr) + ");" << Qt::endl;
        js_file->close();
        NavTr++;
    }
    if (has_setting)
    {
        QFile *js_file = new QFile(addonfolder + "\\..\\loadSetting.js");
        if (!js_file->open(QFile::WriteOnly | QFile::Append))
        {
            qFatal("Can't open js file " + js_file->fileName().toUtf8());
            return 1;
        }
        QTextStream outs(js_file);
        outs.setCodec(QTextCodec::codecForName("utf-8"));
        outs << "var set_div_" + QString::number(NavTr) + " = document.createElement('div');" << Qt::endl;
        outs << "set_div_" + QString::number(NavTr) + ".className = 'container mt-3 align-content-center col-lg-7 card';" << Qt::endl;
        outs << "set_div_" + QString::number(NavTr) + ".style = 'margin: 0 auto;background-color:rgba(255, 255, 255, 0.6);';" << Qt::endl;
        outs << "var set_body_" + QString::number(NavTr) + " = document.createElement('div');" << Qt::endl;
        outs << "set_body_" + QString::number(NavTr) + ".className = 'card-body';" << Qt::endl;
        outs << "var set_text_" + QString::number(NavTr) + " = document.createElement('p');" << Qt::endl;
        outs << "set_text_" + QString::number(NavTr) + ".className = 'card-text';" << Qt::endl;
        outs << "set_text_" + QString::number(NavTr) + ".innerText = '" + (SettingName) + "';" << Qt::endl;
        outs << "set_div_" + QString::number(NavTr) + ".appendChild(set_body_" + QString::number(NavTr) + ");" << Qt::endl;
        outs << "set_body_" + QString::number(NavTr) + ".appendChild(set_text_" + QString::number(NavTr) + ");" << Qt::endl;
        outs << "document.getElementById('add_card').appendChild(set_div_" + QString::number(NavTr) + ");" << Qt::endl;
        int i = 0;
        ini->beginGroup("Setting");
        QStringList SettingskeyList = ini->allKeys();
        foreach (QString Setting, SettingskeyList)
        {
            if (Setting == "name")
            {
                continue;
            }
            else
            {
                outs << "var set_set_" + QString::number(NavTr) + "_" + QString::number(i) + " = document.createElement('a');" << Qt::endl;
                outs << "set_set_" + QString::number(NavTr) + "_" + QString::number(i) + ".href = '" + (ini->value(Setting).toString()) + "';" << Qt::endl;
                outs << "set_set_"+QString::number(NavTr)+"_"+QString::number(i)+".innerText = '"+(Setting)+"';" << Qt::endl;
                outs << "set_body_" + QString::number(NavTr) + ".appendChild(set_set_" + QString::number(NavTr) + "_" + QString::number(i) + ");" << Qt::endl;
                i++;
            }
        }
        ini->endGroup();
        js_file->close();
    }
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    return 0;
}
bool addon::init()
{
    addonfolder = searchAddonFile();
    QStringList addons = FindFile(addonfolder);
    qDebug() << "Using addon(s):" << addons;
    QString addon;
    int i = 0;
    copyFile(addonfolder + "\\..\\..\\source\\loadAddon.js", addonfolder + "\\..\\loadAddon.js");
    copyFile(addonfolder + "\\..\\..\\source\\LoadNavBar.js", addonfolder + "\\..\\LoadNavBar.js");
    copyFile(addonfolder + "\\..\\..\\source\\loadSetting.js", addonfolder + "\\..\\loadSetting.js");
    foreach (addon, addons)
    {
        i++;
        CopyAddons(addon);
        qDebug() << "Copy addons " + addon;
    }
    Toast::showTip(QString::number(i) + "个插件加载成功!");
    return 0;
}
