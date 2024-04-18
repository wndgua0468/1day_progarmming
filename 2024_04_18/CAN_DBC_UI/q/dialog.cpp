#include "dialog.h"
#include "./ui_dialog.h"
#include <QHash>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug> //

// 클래스 내에 해시 맵을 정의합니다.
QHash<QString, QString> comboBoxValueMapping;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // 해시 맵에 매핑 값을 추가합니다.
    comboBoxValueMapping.insert("Standard", "419361278");
    comboBoxValueMapping.insert("Extended", "2566844926");
    comboBoxValueMapping.insert("Extended (J1939PG)", "2566844926");

    // 시그널과 슬롯을 연결합니다.
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Dialog::on_textEdit_textChanged);

}

Dialog::~Dialog()
{
    delete ui;

}

// 종료 버튼
void Dialog::on_pushButton_2_clicked()
{
    this->close();
}

// 출력 버튼
void Dialog::on_pushButton_clicked()
{
    // BO_ 출력
    QString comboBoxValue = ui->comboBox->currentText();
    QString lineEdit2Text = ui->lineEdit_2->text();
    QString lineEdit4Text = ui->lineEdit_4->text();
    QString outputString = "BO_ " + comboBoxValue + " " + lineEdit2Text + " : " + lineEdit4Text + " Vector__XXX";
    ui->textEdit->append(outputString);

    // SG_ 출력
    QString signalName = ui->lineEdit_6->text();
    QString signalStart = ui->lineEdit_10->text();
    QString signalLength = ui->lineEdit_11->text();
    QString signalOrder = ui->comboBox_3->currentIndex() == 0 ? "1" : "0";
    QString signalType = ui->comboBox_4->currentIndex() == 0 ? "+" : "-";
    QString signalFactor = ui->lineEdit_12->text();
    QString signalOffset = ui->lineEdit_13->text();
    QString signalMin = ui->lineEdit_14->text();
    QString signalMax = ui->lineEdit_15->text();
    QString signalUnit = ui->lineEdit_16->text();
    QString outputSGString = "SG_ " + signalName + " : " + signalStart + "|" + signalLength + "@" + signalOrder + signalType + " (" + signalFactor + "," + signalOffset + ") [" + signalMin + "|" + signalMax + "] \"" + signalUnit + "\" Vector__XXX";
    ui->textEdit->append(outputSGString);
}

// messages name
void Dialog::on_lineEdit_2_returnPressed()
{
    QString text = ui->lineEdit_2->text();
    ui->textEdit->append(text);
    ui->lineEdit_2->clear();
}

// messages type
void Dialog::on_comboBox_activated(int index)
{

}

// DLC
void Dialog::on_lineEdit_4_returnPressed()
{
    QString text = ui->lineEdit_4->text();
    ui->textEdit->append(text);
    ui->lineEdit_4->clear();
}

// signal Name
void Dialog::on_lineEdit_6_returnPressed()
{
    QString textFromLineEdit = ui->lineEdit_6->text();
    ui->textEdit->append(textFromLineEdit);
}

// signal type
void Dialog::on_comboBox_2_activated(int index)
{

}

// signal order
void Dialog::on_comboBox_3_activated(int index)
{

}

// signal mode
void Dialog::on_comboBox_4_activated(int index)
{

}

// signal start
void Dialog::on_lineEdit_10_returnPressed()
{

}

// signal Length
void Dialog::on_lineEdit_11_returnPressed()
{

}

// signal factor
void Dialog::on_lineEdit_12_returnPressed()
{

}

// signal offset
void Dialog::on_lineEdit_13_returnPressed()
{

}

// signal min
void Dialog::on_lineEdit_14_returnPressed()
{

}

// signal max
void Dialog::on_lineEdit_15_returnPressed()
{

}

// signal unit
void Dialog::on_lineEdit_16_returnPressed()
{

}

// textEdit textChanged
void Dialog::on_textEdit_textChanged()
{

}

// 저장 버튼
void Dialog::on_pushButton_3_clicked()
{
    // 파일 저장 대화상자 열기
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt)"));

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << ui->textEdit->toPlainText();
            file.close();
        }
    }
}

//dbc file
void Dialog::on_lineEdit_3_returnPressed()
{

}

//select
void Dialog::on_pushButton_4_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open CAN DBC File"), "", tr("CAN DBC Files (*.dbc);;All Files (*)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString fileContent = in.readAll();
            ui->textEdit->setText(fileContent);
            file.close();
        } else {
            qDebug() << "Failed to open file: " << fileName;
        }
    }

}

//save
void Dialog::on_pushButton_5_clicked()
{


}

//tap2 파일선택
void Dialog::on_Open_ReadFiles_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("DBC Files (*.dbc);;All Files (*)"));
    if (!filePath.isEmpty()) {
        // 파일 경로를 텍스트 상자에 표시합니다.
        ui->ReadFIle_Path->setText(filePath);
    }
}

//tap2 적용
void Dialog::on_Apply_clicked()
{
    // 선택된 파일 경로를 가져옵니다.
    QString filePath = ui->ReadFIle_Path->text();

    // 파일을 열어서 내용을 읽어옵니다.
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for reading";
        return;
    }

    QTextStream in(&file);
    QString fileContent = in.readAll();

    // 읽은 파일 내용을 텍스트 상자에 표시합니다.
    ui->DBC_Read_Temp->setText(fileContent);

    // 파일 포인터를 다시 파일의 처음으로 이동합니다.
    file.seek(0);

    // Can_Message_Table 및 CAN_Signal_Table에 데이터 표시
    int messageRow = 0;
    int signalRow = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("BO_")) {
            QStringList parts = line.split(" ");
            if (parts.size() >= 4) {
                QString messageId = parts[1];
                QString messageName = parts[2];
                QString messageLength = parts[3];
                QString messageComment = parts[4];  // 코멘트는 빈 문자열로 가정합니다.
                ui->Can_Message_Table->insertRow(messageRow);
                ui->Can_Message_Table->setItem(messageRow, 0, new QTableWidgetItem(messageName));
                ui->Can_Message_Table->setItem(messageRow, 1, new QTableWidgetItem(messageId));
                ui->Can_Message_Table->setItem(messageRow, 2, new QTableWidgetItem(messageLength));
                ui->Can_Message_Table->setItem(messageRow, 3, new QTableWidgetItem(messageComment)); // 코멘트는 빈 문자열
                ++messageRow;
            }
        } else if (line.startsWith(" SG_")) {
            QStringList parts = line.split(" ");
            if (parts.size() >= 10) {
                QString signalName = parts[2];
                QString startBit = parts[4].split("|").first();
                QString length = parts[4].split("|").last().replace("@1+", "");
                QString factor = parts[5].split(",").first().replace("(", "");
                QString offset = parts[5].split(",").last().chopped(1);
                QString min = parts[6].split("|").first().replace("[","");
                QString max = parts[6].split("|").last().replace("]","");
                QString unit = parts[7].remove('"');
                QString comment = parts[9].remove('"');
                ui->CAN_Signal_Table->insertRow(signalRow);
                ui->CAN_Signal_Table->setItem(signalRow, 0, new QTableWidgetItem(signalName));
                ui->CAN_Signal_Table->setItem(signalRow, 1, new QTableWidgetItem(startBit));
                ui->CAN_Signal_Table->setItem(signalRow, 2, new QTableWidgetItem(length));
                ui->CAN_Signal_Table->setItem(signalRow, 3, new QTableWidgetItem(factor));
                ui->CAN_Signal_Table->setItem(signalRow, 4, new QTableWidgetItem(offset));
                ui->CAN_Signal_Table->setItem(signalRow, 5, new QTableWidgetItem(min));
                ui->CAN_Signal_Table->setItem(signalRow, 6, new QTableWidgetItem(max));
                ui->CAN_Signal_Table->setItem(signalRow, 7, new QTableWidgetItem(unit));
                ui->CAN_Signal_Table->setItem(signalRow, 8, new QTableWidgetItem(comment));
                ++signalRow;
            }
        }
    }

    file.close();
}

//tap2 파일 출력
void Dialog::on_ReadFIle_Path_cursorPositionChanged(int arg1, int arg2)
{
    // 현재 텍스트 상자의 내용을 가져와서 파일 경로로 출력합니다.
    QString filePath = ui->ReadFIle_Path->text();
    qDebug() << "File path:" << filePath;
}

// void Dialog::on_Can_Message_Table_cellActivated(int row, int column)
// {

// }

// void Dialog::on_CAN_Signal_Table_cellActivated(int row, int column)
// {

// }

void Dialog::on_DBC_Read_Temp_textChanged()
{
}


void Dialog::on_Apply_3_clicked()
{
    QString updatedContent = ui->DBC_Read_Temp->toPlainText();

    // Can_Message_Table의 변경된 값 적용
    for (int row = 0; row < ui->Can_Message_Table->rowCount(); ++row) {
        for (int col = 0; col < ui->Can_Message_Table->columnCount(); ++col) {
            QTableWidgetItem *item = ui->Can_Message_Table->item(row, col);
            if (item && item->data(Qt::UserRole).toBool()) { // 변경된 값인지 확인
                QString modifiedValue = item->text();
                // Can_Message_Table의 해당 위치에 있는 값을 찾아서 변경
                int startPos = updatedContent.indexOf(modifiedValue);
                if (startPos != -1) {
                    updatedContent.replace(startPos, modifiedValue.length(), modifiedValue);
                }
                // 변경된 값 표시 플래그를 해제
                item->setData(Qt::UserRole, false);
            }
        }
    }

    // CAN_Signal_Table의 변경된 값 적용
    for (int row = 0; row < ui->CAN_Signal_Table->rowCount(); ++row) {
        for (int col = 0; col < ui->CAN_Signal_Table->columnCount(); ++col) {
            QTableWidgetItem *item = ui->CAN_Signal_Table->item(row, col);
            if (item && item->data(Qt::UserRole).toBool()) { // 변경된 값인지 확인
                QString modifiedValue = item->text();
                // CAN_Signal_Table의 해당 위치에 있는 값을 찾아서 변경
                int startPos = updatedContent.indexOf(modifiedValue);
                if (startPos != -1) {
                    updatedContent.replace(startPos, modifiedValue.length(), modifiedValue);
                }
                // 변경된 값 표시 플래그를 해제
                item->setData(Qt::UserRole, false);
            }
        }
    }

    // 변경된 내용을 DBC_Read_Temp에 설정
    ui->DBC_Read_Temp->setPlainText(updatedContent);



}


void Dialog::on_Apply_2_clicked()
{
    // 파일 저장 대화상자 열기
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("DBC Files (*.dbc);;All Files (*)"));

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << ui->DBC_Read_Temp->toPlainText();
            file.close();
        }
    }
}


void Dialog::on_Can_Message_Table_cellChanged(int row, int column)
{
    QTableWidgetItem *item = ui->Can_Message_Table->item(row, column);
    if (item) {
        QString modifiedValue = item->text();
        // 변경된 값을 DBC_Read_Temp에 출력하거나 적용하는 등의 작업을 수행합니다.
    }

}


void Dialog::on_CAN_Signal_Table_cellChanged(int row, int column)
{
    QTableWidgetItem *item = ui->CAN_Signal_Table->item(row, column);
    if (item) {
        QString modifiedValue = item->text();
        // 변경된 값을 DBC_Read_Temp에 출력하거나 적용하는 등의 작업을 수행합니다.
    }

}
////////////////////////////////////////////////////////////////////////////////////
//연결
void Dialog::on_pushButton_6_clicked()
{

}

//출력
void Dialog::on_pushButton_7_clicked()
{

}

//데이터 저장
void Dialog::on_Apply_4_clicked()
{

}

//
void Dialog::on_DBC_Read_Temp_2_textChanged()
{

}

//정지
void Dialog::on_pushButton_8_clicked()
{

}

