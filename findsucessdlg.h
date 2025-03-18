#ifndef FINDSUCESSDLG_H
#define FINDSUCESSDLG_H

#include <QDialog>
#include "userdata.h"
namespace Ui {
class FindSucessDlg;
}

class FindSucessDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FindSucessDlg(QWidget *parent = nullptr);
    ~FindSucessDlg();
    void setSearchInfo(std::shared_ptr<SearchInfo> si);
private:
    Ui::FindSucessDlg *ui;
    std::shared_ptr<SearchInfo> _si;
    QWidget* _parent;

private slots:
    void slot_addFrinedBtn_click();
};

#endif // FINDSUCESSDLG_H
