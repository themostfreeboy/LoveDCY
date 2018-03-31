#include "notemodifydlg.h"
#include "ui_notemodifydlg.h"
#include "common.h"
//#include <QMessageBox>
//#include <QDebug>

#define NOTE_CONTENT_MAXLENGTH 10224//数据库VARCHAR(20480)，aes加密，16Byte一组，16Byte加密后变为16Byte，转为字符串32Byte(纯英文)，PKCS5补位，防止补16Byte，所以，20480/2=10240;10240/16=640;639*16=10224;

NoteModifyDlg::NoteModifyDlg(QWidget *parent) :
    CSelfTitleDlg(parent),
    ui(new Ui::NoteModifyDlg)
{
    ui->setupUi(this);
    init(TITLE_MIN_FLAG);
}

NoteModifyDlg::~NoteModifyDlg()
{
    delete ui;
}

void NoteModifyDlg::init_data()
{
    ui->plainNoteContent->setPlainText(content);
    return;
}

void NoteModifyDlg::on_btnSave_clicked()
{
    content = cutQString(ui->plainNoteContent->toPlainText().trimmed(),NOTE_CONTENT_MAXLENGTH);
    this->accept();
}

void NoteModifyDlg::on_btnCancel_clicked()
{
    this->reject();
}
