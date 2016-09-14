#ifndef QARDUINOEXAMPLEWIDGET_H
#define QARDUINOEXAMPLEWIDGET_H

#include <QWidget>
#include "QArduino.h"

namespace Ui {
class QArduinoExampleWidget;
}

class QArduinoExampleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QArduinoExampleWidget(QWidget *parent = 0);
    ~QArduinoExampleWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void slotUpdateMsg(QString);

    void on_pushButton_pressed();

private:
    Ui::QArduinoExampleWidget *ui;
    QArduino *arduino;
};

#endif // QARDUINOEXAMPLEWIDGET_H
