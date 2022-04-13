#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QtDebug>
#include <ostream>
#include <QProcess>
#include <QCryptographicHash>
#include <QFile>
#include <QString>
#include <QInputDialog>
#include <QClipboard>
#include <QFileInfo>
#include <string>
#include <stdio.h>


bool fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_process(new QProcess(this))
{
    ui->setupUi(this);
    this->setWindowTitle("Chatbaren"); //set title
    ui->messageInput->setFocus(); //focus on lineedit

    //start listen server
    connect(m_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
    connect(m_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
    connect(m_process, SIGNAL(readChannelFinished()), this, SLOT(readChannelFinished()));
    connect(m_process, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    connect(m_process, SIGNAL(started()), this, SLOT(started()));
    connect(m_process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(stateChanged(QProcess::ProcessState)));
    connect(m_process, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    // |      -- Server
    // V
    QString commandLine = "bash -c";
    QStringList list = commandLine.split(" ");

    if(list.isEmpty())
        return;

    QString command = list.first();
    list.removeFirst();

    QStringList arguments;
    foreach(const QString &tmp, list)
        arguments << tmp;

    arguments << "chatbaren -d serve";
    m_process->start(command, arguments);

    //get prefix
    QProcess getPrefix;
    QString command01 = "chatbaren";
    QString param = "-d prefix";
    QString exec = QString(command01)+" "+QString(param);
    getPrefix.start("bash", QStringList() << "-c" << exec);
    getPrefix.waitForFinished(-1);
    QString prefix = getPrefix.readAllStandardOutput();

    //clear tor log
    QFile file(prefix+"/tor/notices.log");
    file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&file);
    stream << "" << endl;
    file.close();

    //restart tor
    QProcess reStartTor;
    QString command999 = "nohup chatbaren";
    QString param999 = "-d restarttor > /dev/null";
    QString exec999 = QString(command999)+" "+QString(param999);
    reStartTor.start("bash", QStringList() << "-c" << exec999);
    reStartTor.waitForFinished(-1);

    //get contacts
    QProcess getContacts;
    QString command02 = "ls";
    QString param02 = ""+prefix+"/keys/*.onion | awk -F'/' '{print $NF}'";
    QString exec02 = QString(command02)+" "+QString(param02);
    getContacts.start("bash", QStringList() << "-c" << exec02);
    getContacts.waitForFinished(-1);
    QString contacts = getContacts.readAllStandardOutput();
    //
    int lineNum = 0;
    QTextStream in(&contacts);
    QString pepoles[1000];
    while ( !in.atEnd() && lineNum < 1000)
    {
       QString line = in.readLine();

       ui->contactsList->addItem(line);
       pepoles[lineNum++] = line;
     }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    //ui->plainTextEdit->append(trUtf8("exit Code: %1, exit status: %2").arg(exitCode).arg(exitStatus));
}

void MainWindow::error(QProcess::ProcessError error)
{
    //ui->textEdit->append(trUtf8("error: %1").arg(error));
}

void MainWindow::readChannelFinished()
{
    //ui->textEdit->append(trUtf8("readChannelFinished()..."));
}

void MainWindow::readyRead()
{
    //ui->textEdit->append(trUtf8("readRead()..."));
}

void MainWindow::readyReadStandardError()
{
    //ui->textEdit->append(trUtf8("readyReadStandardError()..."));
    QByteArray buffer = m_process->readAllStandardError();
    ui->plainTextEdit->appendHtml(buffer);
}

void MainWindow::readyReadStandardOutput()
{
    //ui->textEdit->append(trUtf8("readyReadStandardOutput()..."));
    QByteArray buffer = m_process->readAllStandardOutput();
    ui->plainTextEdit->appendHtml(buffer);
}

void MainWindow::started()
{
    //ui->textEdit->append(trUtf8("started()..."));
}

void MainWindow::stateChanged(QProcess::ProcessState state)
{
    //ui->textEdit->append(trUtf8("state: %1").arg(state));
}

void MainWindow::bytesWritten(qint64 size)
{
    //ui->textEdit->append(trUtf8("bytes written: %1").arg(size));
}



void MainWindow::on_pushButton_clicked()
{
    QString username;
    if (ui->actionHide_username->isChecked() != true){
        QProcess getUsername;
        QString command = "chatbaren";
        QString param = "-d username";
        QString exec = QString(command)+" "+QString(param);
        getUsername.start("bash", QStringList() << "-c" << exec);
        getUsername.waitForFinished(-1);
        QString stdout = getUsername.readAllStandardOutput();
        username = "["+stdout+"]";
    }

    QProcess sendMessage;
    if (ui->messageInput->text() != "" && ui->messageInput->text() != " "){
    QString message = ui->messageInput->text();
    QString messageBase64 = message.toUtf8().toBase64();
    QString recipient = ui->contactsList->currentText();
    QString param = "-m";
    QString command = "chatbaren";
    QString exec;
    if (ui->actionHide_username->isChecked()){
        exec = QString(command)+" "+QString(param)+" "+QString(recipient)+" "+QString(messageBase64)+" _-_--.HIDE_USERNAME.--_-_";
    } else {
        exec = QString(command)+" "+QString(param)+" "+QString(recipient)+" "+QString(messageBase64);
    }

    sendMessage.start("bash", QStringList() << "-c" << exec);
    ui->messageInput->clear();
    sendMessage.waitForFinished(-1);
    ui->plainTextEdit->appendHtml("<div style='color:blue;'>"+username+"</div> "+message);
    QString stdout = sendMessage.readAllStandardOutput();
    }
    //QMessageBox::StandardButton reply = QMessageBox::information(this,"Out",exec);
    /*if (reply == QMessageBox::Yes) {
        //QProcess::execute("ls");
        QString ccc = "echo hihi | sha256sum";
        system(ccc.toStdString().c_str());

        QMessageBox::information(this,"Verified!","Verified!");
        QMessageBox::warning(this, tr("My Application"),
                                       tr("The document has been modified.\n"
                                          "Do you want to save your changes?"),
                                       QMessageBox::Save | QMessageBox::Discard
                                       | QMessageBox::Cancel,
                                       QMessageBox::Save);
    }*/
}

void MainWindow::on_messageInput_returnPressed()
{
    if (ui->messageInput->text() != "" && ui->messageInput->text() != " "){
    ui->pushButton->click();
    }
}

void MainWindow::on_actionAddress_triggered()
{
    QProcess getAddress;
    QString command = "chatbaren";
    QString param = "-d address";
    QString exec = QString(command)+" "+QString(param);
    getAddress.start("bash", QStringList() << "-c" << exec);
    getAddress.waitForFinished(-1);
    QString stdout = getAddress.readAllStandardOutput();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(stdout);
    QMessageBox::information(this,"Your address.","Copied.\n\n"+stdout);
}

void MainWindow::on_actionQuit_triggered()
{
    MainWindow::close();
}

void MainWindow::on_actionChange_username_triggered()
{
    QString username = QInputDialog::getText(this, "Change username",
                                             "New username:", QLineEdit::Normal);
    QProcess getPrefix;
    QString command = "chatbaren";
    QString param = "-d prefix";
    QString exec = QString(command)+" "+QString(param);
    getPrefix.start("bash", QStringList() << "-c" << exec);
    getPrefix.waitForFinished(-1);
    QString stdout = getPrefix.readAllStandardOutput();
    QString filename=stdout+"/username.txt";
    QFile file(filename);
    file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&file);
    stream << username << endl;
    file.close();
}

void MainWindow::on_actionShow_triggered()
{
    QProcess getPubKey;
    QString command = "chatbaren";
    QString param = "-d pubkey";
    QString exec = QString(command)+" "+QString(param);
    getPubKey.start("bash", QStringList() << "-c" << exec);
    getPubKey.waitForFinished(-1);
    QString stdout = getPubKey.readAllStandardOutput();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(stdout);
    QMessageBox::information(this,"Your public key","Key copied to clipboard\n\n"+stdout);
}

void MainWindow::on_actionClear_log_triggered()
{
    ui->plainTextEdit->clear();
}

void MainWindow::on_actionLog_triggered()
{
    torlog = new TorLog(this);
    torlog->show();
}

void MainWindow::on_actionStart_triggered()
{
    QProcess startTor;
    QString command = "nohup chatbaren";
    QString param = "-d starttor > /dev/null";
    QString exec = QString(command)+" "+QString(param);
    startTor.start("bash", QStringList() << "-c" << exec);
    startTor.waitForFinished(-1);
    QMessageBox::information(this,"Tor started.","Tor started.");
}

void MainWindow::on_actionRestart_triggered()
{
    QProcess reStartTor;
    QString command = "nohup chatbaren";
    QString param = "-d restarttor > /dev/null";
    QString exec = QString(command)+" "+QString(param);
    reStartTor.start("bash", QStringList() << "-c" << exec);
    reStartTor.waitForFinished(-1);
    QMessageBox::information(this,"Tor restarted.","Tor restarted.");

}

void MainWindow::on_actionStop_triggered()
{
    QProcess stopTor;
    QString command = "nohup chatbaren";
    QString param = "-d stoptor > /dev/null";
    QString exec = QString(command)+" "+QString(param);
    stopTor.start("bash", QStringList() << "-c" << exec);
    stopTor.waitForFinished(-1);
    QMessageBox::information(this,"Tor stoped.","Tor stoped.");

}

void MainWindow::on_actionHashsums_triggered()
{
    QProcess getSums;
    QString command = "chatbaren";
    QString param = "-d sums "+ui->contactsList->currentText();
    QString exec = QString(command)+" "+QString(param);
    getSums.start("bash", QStringList() << "-c" << exec);
    getSums.waitForFinished(-1);
    QString sums = getSums.readAllStandardOutput();
    QMessageBox::information(this,"Hashsums","Hashsums:\n\n"+sums);
}


void MainWindow::on_actionAdd_triggered()
{
    QString addAddress = QInputDialog::getText(this, "Enter onion address.",
                                                 "Enter onion address: ", QLineEdit::Normal);
    if (addAddress != ""){
    QString addKey = QInputDialog::getMultiLineText(this, "Enter public key.",
                                                 "Enter public key: ");
    if (addKey != ""){
        QProcess getPrefix;
        QString command01 = "chatbaren";
        QString param = "-d prefix";
        QString exec = QString(command01)+" "+QString(param);
        getPrefix.start("bash", QStringList() << "-c" << exec);
        getPrefix.waitForFinished(-1);
        QString prefix = getPrefix.readAllStandardOutput();

        ui->contactsList->addItem(addAddress);

        QFile file(prefix+"/contacts.txt");
        file.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        QTextStream stream(&file);
        stream << endl << addAddress;
        file.close();

        QFile keyFile(prefix+"/keys/"+addAddress);
        keyFile.open(QIODevice::ReadWrite | QIODevice::NewOnly | QIODevice::Text);
        QTextStream stream01(&keyFile);
        stream01 << addKey;
        keyFile.close();
        }
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,"Help","Chatbaren-Qt - Secure, open-source, peer-to-peer, end-to-end encrypted Tor messaging system with Qt for Linux.\n\nDonate:\nBTC 15EmrTsRjFuuiRgohSPKqDjjAXdisWULbs\n");
}
