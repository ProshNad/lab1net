#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTcpSocket"
#include "QTcpServer"
#include "QMessageBox"
#include "QTime"

/*
Создать клиент-серверное приложение на сокетах, выполняющее следующую задачу:
клиент высылает 2 неотрицательных
числа, сервер возвращает число сочетаний из большего числа
по меньшему числу.
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), SLOT(newtcpConnection()));
    server->listen(QHostAddress::Any, 1024);
    qDebug()<<"server open";
    m_nNextBlockSize=0;

}

void MainWindow::newtcpConnection()
{
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));

}

void MainWindow::readyRead()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_5);

    for(;;){
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        int nn;
        int kk;
        in >> time >>nn>>kk;
        qDebug()<<"got:"<<nn<<kk;
        m_nNextBlockSize = 0;
        /*

          n!/(k!*(n-k)!)
*/
auto res = [nn,kk](){
    unsigned n=nn; unsigned k=kk;
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result; };

    sendToClient(pClientSocket,QString::number(res()));


    }



}

void MainWindow::disconnected()
{
socket->deleteLater();
}

void MainWindow::sendToClient(QTcpSocket* pClientSocket, const QString& str)
{

    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    pClientSocket->write(arrBlock);

}

