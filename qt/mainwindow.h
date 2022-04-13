#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QProcess>
#include <QMainWindow>
#include "torlog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_messageInput_returnPressed();

    void on_actionAddress_triggered();

    void on_actionQuit_triggered();

    void on_actionChange_username_triggered();

    void on_actionShow_triggered();

    void on_actionClear_log_triggered();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_pushButton_clicked();
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void error(QProcess::ProcessError error);
    void readChannelFinished();
    void readyRead();
    void readyReadStandardError();
    void readyReadStandardOutput();
    void started();
    void stateChanged(QProcess::ProcessState state);
    void bytesWritten(qint64 size);

    void on_actionLog_triggered();

    void on_actionStart_triggered();

    void on_actionRestart_triggered();

    void on_actionStop_triggered();

    void on_actionHashsums_triggered();

    void on_actionAdd_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QProcess *m_process;
    TorLog *torlog;
};

#endif // MAINWINDOW_H
