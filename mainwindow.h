#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTcpSocket"
#include "QTcpServer"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QTcpServer *server;
    QTcpSocket *socket;
    quint16     m_nNextBlockSize;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void newtcpConnection();
    void readyRead();
    void disconnected();
    void sendToClient(QTcpSocket*, const QString&);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
