#ifndef SEARCHLIST_H
#define SEARCHLIST_H

#include <QListWidget>
#include <QWheelEvent>
#include "userdata.h"

class LoadingDlg;
class SearchList:public QListWidget
{
    Q_OBJECT
public:
    SearchList(QWidget* parent=nullptr);
    void closeFindDlg();
    void SetSearchEdit(QWidget* edit);
protected:
    bool eventFilter(QObject* o,QEvent* e) override;
private:
    void waitPending(bool pending);
    bool _send_pending;
    void addTipItem();
    std::shared_ptr<QDialog> _find_dlg;
    QWidget* _search_edit;
    LoadingDlg* _loadingDlg;
private:
    void slot_item_clicked(QListWidgetItem* item);
    void slot_user_search(std::shared_ptr<SearchInfo> si);
};

#endif // SEARCHLIST_H
