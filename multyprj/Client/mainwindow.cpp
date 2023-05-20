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

// TODO: Использовать единый стиль кода
// Пробелы:
// IP = ui->lineEdit_IP->text();
// qDebug() << IP;
// if (in.status() == QDataStream::Ok)
// socket->bytesAvailable()

void MainWindow::on_pushButton_clicked()
{
    IP = ui->lineEdit_IP->text();
    qDebug() << IP;

    QString PortS = ui->lineEdit_Port->text();
    Port = PortS.toInt();
    qDebug() << Port;

    socket->connectToHost(IP, Port);
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_9);
    if (in.status() == QDataStream::Ok)
    {
        //QString str;
        //in >> str;
        //ui->textBrowser->append(str);
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

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    QFileInfo fileInfo(str); // Достаём из пути название фала
    QString fileName = fileInfo.fileName();

    QImageReader reader(str);
    QImage image = reader.read();

    out << quint16(0) << QTime::currentTime() << fileName << image;
    out.device()->seek(0);
    out << quint16(Data.size()-sizeof (quint16));
    socket->write(Data);
    ui->lineEdit_FilePath->clear();
}

void MainWindow::SendToServer(QString ip, int port, QString str)
{
    // TODO: По-хорошему нужно добавить проверку на валидность входных параметров
    // TODO: Объект socket должен создаваться внутри этой функции
    socket->connectToHost(ip, port);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

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
    ui->lineEdit_FilePath->clear();

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


// TODO: Убрать неиспользуемые функции
void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{

}


void MainWindow::on_pushButton_3_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this);
}

