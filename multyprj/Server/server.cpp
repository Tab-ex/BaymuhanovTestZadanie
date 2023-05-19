#include "server.h"
#include "QDataStream"

Server::Server()
{
    if(this->listen(QHostAddress::Any, 2323)) // Идёт прослушивание со всех одресов и порта 2323
    {
        qDebug() <<"Start"; // Сообщение о запуске сервера
    }
    else
    {
        qDebug()<<"error"; // сообщение о ошибке запуска сервера
    }

    nextBlockSize =0;

    QDir().mkdir(QDate::currentDate().toString("yyyy-MM-dd"));
    QDir::setCurrent("/"+QDate::currentDate().toString("yyyy-MM-dd"));
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket -> setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug()<<"client connected" << socketDescriptor;
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        qDebug()<<"read...";
        /*QString str; // Отправка строки
        in>> str;
        qDebug()<<str;
        SendToClient(str);*/
        for (; ; )
        {
            if(nextBlockSize ==0)
            {
                qDebug()<<"next blockSize =0";
                if(socket->bytesAvailable()<2)
                {
                    qDebug()<<"Data <2,break";
                    break;
                }
                in >>nextBlockSize;
            }
            if(socket->bytesAvailable()<nextBlockSize)
            {
                break;
            }

            QString fileName;
            QTime time;
            QImage image;
            in>>time>>fileName>> image;

            image.save(QDate::currentDate().toString("yyyy-MM-dd")+"-"+time.toString("hh-mm-ss")+ fileName);

            nextBlockSize = 0;
            qDebug()<<fileName;
            break;
        }
    }
    else
    {
        qDebug()<<"DataStream error";
    }
}

void Server::SendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out<< quint16(0) <<QTime::currentTime()<<str;
    out.device()->seek(0);
    out<<quint16(Data.size()-sizeof (quint16));
    //socket->write(Data);
    for(int i = 0; i <Sockets.size(); i++)
    {
        Sockets[i]->write(Data);
    }
}
