#ifndef APPLYFRIEND_H
#define APPLYFRIEND_H

#include <QDialog>
#include "clickedlabel.h"
#include "friendlab.h"
#include "userdata.h"

namespace Ui {
class ApplyFriend;
}

class ApplyFriend : public QDialog
{
    Q_OBJECT

public:
    explicit ApplyFriend(QWidget *parent = nullptr);
    ~ApplyFriend();
    void InitTipLabs();
    void AddTipLabs(ClickedLabel*,QPoint cur_point,QPoint& next_point,int text_width,int text_height);
    bool eventFilter(QObject* watched,QEvent* event) override;
    void SetSearchInfo(std::shared_ptr<SearchInfo> si);

private:
    Ui::ApplyFriend *ui;
    void ResetLabs();
    QPoint _label_point;
    QMap<QString,ClickedLabel*> _add_labels;
    std::vector<QString> _add_label_keys;
    QMap<QString,FriendLab*> _friend_labels;
    std::vector<QString> _friend_label_keys;
    void AddLabel(QString name);
    QPoint _tip_cur_point;
    std::shared_ptr<SearchInfo> _si;
    std::vector<QString> _tip_data;
public slots:
    //显示更多label标签
    void ShowMoreLabel();
    //输入label按下回车触发将标签加入展示栏
    void SlotLabelEnter();
    //点击关闭，移除展示栏好友便签
    void SlotRemoveFriendLabel(QString);
    //通过点击tip实现增加和减少好友便签
    void SlotChangeFriendLabelByTip(QString, ClickLbState);
    //输入框文本变化显示不同提示
    void SlotLabelTextChange(const QString& text);
    //输入框输入完成
    void SlotLabelEditFinished();
    //输入标签显示提示框，点击提示框内容后添加好友便签
    void SlotAddFirendLabelByClickTip(QString text);
    //处理确认回调
    void SlotApplySure();
    //处理取消回调
    void SlotApplyCancel();
};

#endif // APPLYFRIEND_H
