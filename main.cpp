/******************************************************************************
 *
 * @file       main.cpp
 * @brief      XXXX Function
 *
 * @author     飞鱼（flyingfry)
 * @date       2024/08/08
 * @history
 *****************************************************************************/
#include "mainwindow.h"
#include <QFile>
#include <QApplication>
#include "global.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug("open sucess");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        qDebug("open failed");
    }
    QString fileName = "config.ini";
    QString app_path = QCoreApplication::applicationDirPath();
    qDebug()<<app_path;
    QString configpath = QDir::toNativeSeparators(app_path += QDir::separator()+fileName);
    QSettings settings(configpath,QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    qDebug()<<gate_host<<gate_port;
    gate_url_prefix = "http://"+gate_host+":"+gate_port;
    MainWindow w;
    w.show();
    return a.exec();
}
