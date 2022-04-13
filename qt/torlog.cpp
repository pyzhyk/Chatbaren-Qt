#include "torlog.h"
#include "ui_torlog.h"
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include <QTime>

void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 3000);
}

TorLog::TorLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TorLog)
{
    ui->setupUi(this);

    QProcess getPrefix;
    QString command = "chatbaren";
    QString param = "-d prefix";
    QString exec = QString(command)+" "+QString(param);
    getPrefix.start("bash", QStringList() << "-c" << exec);
    getPrefix.waitForFinished(-1);
    QString prefix = getPrefix.readAllStandardOutput();

    QString filename=prefix+"/tor/notices.log";
    int lineNum = 0;
       QFile inputFile(filename);
       if (inputFile.open(QIODevice::ReadOnly))
       {
          QTextStream in(&inputFile);
          QString pepoles[1000];
          while ( !in.atEnd() && lineNum < 1000)
          {
             QString line = in.readLine();

             ui->plainTextEdit->appendPlainText(line);
             pepoles[lineNum++] = line;
           }
       }
}

TorLog::~TorLog()
{
    delete ui;
}

void TorLog::on_updateButton_clicked()
{
    ui->plainTextEdit->clear();
    QProcess getPrefix;
    QString command = "chatbaren";
    QString param = "-d prefix";
    QString exec = QString(command)+" "+QString(param);
    getPrefix.start("bash", QStringList() << "-c" << exec);
    getPrefix.waitForFinished(-1);
    QString prefix = getPrefix.readAllStandardOutput();

    QString filename=prefix+"/tor/notices.log";
    int lineNum = 0;
       QFile inputFile(filename);
       if (inputFile.open(QIODevice::ReadOnly))
       {
          QTextStream in(&inputFile);
          QString pepoles[1000];
          while ( !in.atEnd() && lineNum < 1000)
          {
             QString line = in.readLine();

             ui->plainTextEdit->appendPlainText(line);
             pepoles[lineNum++] = line;
           }
       }
}

void TorLog::on_closeButton_clicked()
{
    TorLog::close();
}
