#ifndef TORLOG_H
#define TORLOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class TorLog;
}

class TorLog : public QDialog
{
    Q_OBJECT

public:
    explicit TorLog(QWidget *parent = nullptr);
    ~TorLog();

private slots:
    void on_updateButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::TorLog *ui;
    TorLog *torlog;
};

#endif // TORLOG_H
