#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:


    void on_pushButton_2_clicked();

    void on_lineEdit_2_returnPressed();

    void on_textEdit_textChanged();

    void on_pushButton_clicked();

    void on_comboBox_activated(int index);

    void on_lineEdit_4_returnPressed();

    void on_lineEdit_6_returnPressed();

    void on_comboBox_2_activated(int index);

    void on_comboBox_3_activated(int index);

    void on_comboBox_4_activated(int index);

    void on_lineEdit_10_returnPressed();

    void on_lineEdit_11_returnPressed();

    void on_lineEdit_12_returnPressed();

    void on_lineEdit_13_returnPressed();

    void on_lineEdit_14_returnPressed();

    void on_lineEdit_15_returnPressed();

    void on_lineEdit_16_returnPressed();

    void on_pushButton_3_clicked();

    void on_lineEdit_3_returnPressed();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();



    void on_Open_ReadFiles_clicked();

    void on_Apply_clicked();

    void on_ReadFIle_Path_cursorPositionChanged(int arg1, int arg2);

    // void on_Can_Message_Table_cellActivated(int row, int column);

    // void on_CAN_Signal_Table_cellActivated(int row, int column);

    void on_DBC_Read_Temp_textChanged();

    void on_Apply_3_clicked();

    void on_Apply_2_clicked();

    void on_Can_Message_Table_cellChanged(int row, int column);

    void on_CAN_Signal_Table_cellChanged(int row, int column);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_Apply_4_clicked();

    void on_DBC_Read_Temp_2_textChanged();

    void on_pushButton_8_clicked();

private:
    Ui::Dialog *ui;

};
#endif // DIALOG_H
