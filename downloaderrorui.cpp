#include "downloaderrorui.h"
#include "ui_downloaderrorui.h"

DownloadErrorUi::DownloadErrorUi(QWidget *parent, QString modsList) : QDialog(parent), ui(new Ui::DownloadErrorUi)
{
    ui->setupUi(this);

    ui->modsListUi->setPlainText(modsList);

    ui->modsListUi->setReadOnly(true);
}

DownloadErrorUi::~DownloadErrorUi()
{
    delete ui;
}
