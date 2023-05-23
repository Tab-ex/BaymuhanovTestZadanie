#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlockSize = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_9);
    if (in.status() == QDataStream::Ok)
    {

        for (; ; )
        {
            if (nextBlockSize == 0)
            {
                if (socket->bytesAvailable() < 2)
                {
                    break;
                }
                in >> nextBlockSize;
            }
            if (socket->bytesAvailable() < nextBlockSize)
            {
                break;
            }

            QString str;
            QTime time;
            in >> time >> str;
            nextBlockSize = 0;
            ui->textBrowser_DebugLog->append(time.toString() + str);
        }
    }
    else
    {
        ui->textBrowser_DebugLog->append("read error");
    }
}

//void MainWindow::SendToServer1(QString ip, int port, QString filePath)
//{
//    //    /*
//    //     * Для работы данной функции должны быть подключены следущие библиотеки:
//    //    #include <QTcpSocket>
//    //    #include <QTime>
//    //    #include <QFileInfo>
//    //    #include<QImage>
//    //    #include <QImageReader>
//    //    #include<QFileDialog>

//    //    Так же в файле .pro должна присутствовать следущяя строчка:
//    //        QT       += core gui network


//    QTcpSocket socket;
//    socket.connectToHost(ip, port);

//    QByteArray data;
//    QDataStream out(&data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_9);

//    QFileInfo fileInfo(filePath);
//    QString fileName = fileInfo.fileName();

//    QImageReader reader(filePath);
//    QImage image = reader.read();

//    out << quint16(0) << QTime::currentTime() << fileName << image;
//    out.device()->seek(0);
//    out << quint16(data.size()-sizeof(quint16));
//    socket.write(data);
//}

void MainWindow::SendToServer(QString ip, int port, QString str)
{
    /*
     * Для работы данной функции должны быть подключены следущие библиотеки:
    #include <QTcpSocket>
    #include <QTime>
    #include <QFileInfo>
    #include<QImage>
    #include <QImageReader>
    #include<QFileDialog>

    Так же в файле .pro должна присутствовать следущяя строчка:
        QT       += core gui network

     */
    // TODO: По-хорошему нужно добавить проверку на валидность входных параметров
    // TODO: Объект socket должен создаваться внутри этой функции
    socket = new QTcpSocket(this);

    socket->connectToHost(ip, port);



    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlockSize = 0;
    // TODO: Добавить проверку статуса сокета (если есть) открылся / не открылся

    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    QFileInfo fileInfo(str); // Достаём из пути название фала
    QString fileName = fileInfo.fileName();

    QImageReader reader(str);
    QImage image = reader.read();

    out << quint16(0) << QTime::currentTime() << fileName << image;
    out.device()->seek(0);
    out << quint16(Data.size()-sizeof(quint16));
    socket->write(Data);

    // TODO: Внутри этой функции не должно быть никаких взаимодействий с GUI
    // TODO: Освобождение ресурсов
}

// TODO: Выполнить рефакторинг названий функция (сигналов/слотов)
void MainWindow::on_pushButton_2_clicked()
{
    // TODO: Параметры должны считываться с полей формы
    SendToServer("127.0.0.1", 2323, ui->lineEdit_FilePath->text());
}

void MainWindow::on_lineEdit_returnPressed()
{
    // TODO: Параметры должны считываться с полей формы
    SendToServer("127.0.0.1", 2323, ui->lineEdit_FilePath->text());
}

void MainWindow::on_pushButto_Connect_clicked()
{
    IP = ui->lineEdit_IP->text();
    qDebug() << IP;

    QString PortS = ui->lineEdit_Port->text();
    Port = PortS.toInt();
    qDebug() << Port;

//    socket->connectToHost(IP, Port);
    SendToServer(ui->lineEdit_IP->text(), Port, ui->lineEdit_FilePath->text());
     ui->lineEdit_FilePath->clear();
}


void MainWindow::on_pushButton_OpenFile_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this);
    ui->lineEdit_FilePath->setText(filename);
}

void MainWindow::on_pushButton_Send_clicked()
{
    int port = (ui->label_Port->text()).toInt();
    SendToServer(ui->lineEdit_IP->text(), port, ui->lineEdit_FilePath->text());
}

