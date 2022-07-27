#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydata.h"
#include <QDebug>
#include <QTime>
#include <QDate>
#include "GeneralFunc.h"

#define MAX_LENGTH 40000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("dataFormatCheck");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkPushButton_clicked()
{
    // "AAABAC010000000101100000002C01071B5A0114021B0000000000544331544453000000FFFFFFFFFFFFFFFFFFFFFF0061BABBBC"
    QString info = ui->textEdit->toPlainText();

    int length = info.size();
    qDebug() << info << "\nlength: " << length;
    if(length < 100)
    {
        ui->textEdit_2->setText("too littel Byte!");
        return;
    }
    else if(length > MAX_LENGTH)
    {
        ui->textEdit_2->setText("too much Byte!");
        return;
    }

    // 数据报文按字节显示
    int row = 0;
    QString line = QString("%1-%2  ").arg(row * 10, 4, 10, QChar('0')).arg((row + 1) * 10 - 1, 4, 10, QChar('0'));
    QString display = QString("           00 01 02 03 04 05 06 07 08 09\n");
    display += line;
    for(int i = 0; i < info.size(); ++i)
    {
        display += info.at(i);
        if((i + 1) % 2 == 0)
            display += ' ';
        if((i + 1) % 20 == 0)
        {
            display += '\n';
            row++;
            line = QString("%1-%2  ").arg(row * 10, 4, 10, QChar('0')).arg((row + 1) * 10 - 1, 4, 10, QChar('0'));
            display += line;
        }
    }
    qDebug() << display;
    ui->textEdit_2->setText(display);

    // 数据报文解析，转换为数组
    QString data_array[MAX_LENGTH];
    for(int i = 0; i < length; ++i)
    {
        data_array[i] = info.at(i);
//        qDebug() <<  data_array[i];
    }

    char num_array[MAX_LENGTH];
    bool ok;
    for(int i = 0; i < length; ++i)
    {
        num_array[i] = data_array[i].toInt(&ok, 16);
//        qDebug() << num_array[i];
    }

    unsigned char char_array[MAX_LENGTH];
    for(int i = 0; (2 * i + 1) < length; ++i)
    {
        char_array[i] = (num_array[2*i] * 16 + num_array[2*i+1]);
//        qDebug() << QString().sprintf("%x", char_array[i]).toUpper();
    }

    // 帧头校验
    if(char_array[0] == 0xAA && char_array[1] == 0xAB && char_array[2] == 0xAC)
        ui->lineEdit->setText(QString("ok"));
    else
        ui->lineEdit->setText(QString("false"));

    // 帧尾校验
    if(char_array[length/2 - 3] == 0xBA && char_array[length/2 - 2] == 0xBB && char_array[length/2 - 1] == 0xBC)
        ui->lineEdit_2->setText(QString("ok"));
    else
        ui->lineEdit_2->setText(QString("false"));

    // CRC校验
    unsigned short CRC = Frame_FCS_CRC(char_array + 3, length/2 - 8);
    if((unsigned char)(CRC>>8) == char_array[length/2 - 5] && (unsigned char)CRC == char_array[length/2 - 4])
        ui->lineEdit_3->setText(QString("ok"));
    else
        ui->lineEdit_3->setText(QString("false"));

    // 时间解析
    QDateTime set_time(QDate(char_array[19] + 2000, char_array[20], char_array[21]),QTime(char_array[22], char_array[23], char_array[24], (char_array[25] << 8) + char_array[26]));
    QString strDateTime = set_time.toString("yyyy-MM-dd hh:mm:ss:zzz");
    qDebug() << strDateTime;
    QString strDateTimeDisplay = set_time.toString("yyyy-MM-dd hh:mm:ss");
    ui->lineEdit_4->setText(strDateTimeDisplay);

    // 设备ID解析
    QByteArray baDeviceID((char*)char_array + 27, 9);
    QString deviceID = QString(baDeviceID);
    ui->lineEdit_5->setText(deviceID);

    // 数据包类型解析
    QString dataType;
    switch(char_array[3])
    {
    case 1:
        dataType = "status";
        break;
    case 2:
        dataType = "fault";
        break;
    case 3:
        dataType = "heart";
        break;
    default:
        dataType = "null";
        break;
    }
    ui->lineEdit_6->setText(dataType);

    // 数据包长度解析
    int dataLength = (char_array[10] << 24) + (char_array[11] << 16) + (char_array[12] << 8 )+ char_array[13];
    qDebug() << dataLength;
    ui->lineEdit_7->setText(QString::number(dataLength + 8));
    if(length/2 != dataLength + 8)
        qDebug() << "length error!";

    // 车号解析
    QString carNumber = QString::number((char_array[16] << 8) + char_array[17]);
    qDebug() << carNumber;
    ui->lineEdit_8->setText(carNumber);
}
