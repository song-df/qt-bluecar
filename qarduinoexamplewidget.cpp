#include "qarduinoexamplewidget.h"
#include "ui_qarduinoexamplewidget.h"


QArduinoExampleWidget::QArduinoExampleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QArduinoExampleWidget)
{
    ui->setupUi(this);
    arduino = new QArduino("WXHL");
    connect(arduino,SIGNAL(connectionInfo(QString)),ui->labelStatus,SLOT(setText(QString)));
    connect(arduino,SIGNAL(connectionInfo(QString)),this,SLOT(slotUpdateMsg(QString)));
}

QArduinoExampleWidget::~QArduinoExampleWidget()
{
    delete ui;
}
void QArduinoExampleWidget::slotUpdateMsg(QString msg)
{
    ui->listWidget->addItem(msg);
    ui->listWidget->scrollToBottom();
}

void QArduinoExampleWidget::on_pushButton_clicked()
{
    arduino->sendMsg("1");
}

void QArduinoExampleWidget::on_pushButton_2_clicked()
{

    arduino->sendMsg("2");
}

void QArduinoExampleWidget::on_pushButton_3_clicked()
{

    arduino->sendMsg("3");
}

void QArduinoExampleWidget::on_pushButton_4_clicked()
{

    arduino->sendMsg("4");
}

void QArduinoExampleWidget::on_pushButton_pressed()
{

    arduino->sendMsg("——1");
}
