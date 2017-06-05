#ifndef DOWNLOADERRORUI_H
#define DOWNLOADERRORUI_H

#include <QDialog>
#include <QStringList>
#include <QStringListIterator>

namespace Ui {
    class DownloadErrorUi;
}

class DownloadErrorUi : public QDialog
{
        Q_OBJECT

    public:
        DownloadErrorUi(QWidget *parent, QString modsList);
        ~DownloadErrorUi();

    private:
        Ui::DownloadErrorUi *ui;
};

#endif // DOWNLOADERRORUI_H
