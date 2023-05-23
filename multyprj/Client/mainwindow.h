#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include <QFileInfo>
#include <QImage>
#include <QImageReader>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_returnPressed();

    void on_pushButto_Connect_clicked();

    void on_pushButton_OpenFile_clicked();

    void on_pushButton_Send_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    quint16 nextBlockSize;
    QString IP;
    int Port;
    void SendToServer(QString ip, int port, QString str);


    ////
public slots:
    void slotReadyRead();
};
#endif // MAINWINDOW_H
