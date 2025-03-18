#include "applyfriend.h"
#include "qscrollbar.h"
#include "ui_applyfriend.h"
#include "usermgr.h"

ApplyFriend::ApplyFriend(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ApplyFriend)
{
    ui->setupUi(this);
    // 隐藏对话框标题栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setObjectName("ApplyFriend");
    this->setModal(true);
    ui->nameEdit->setPlaceholderText(tr("恋恋风辰"));
    ui->labEdit->setPlaceholderText("搜索、添加标签");
    ui->backEdit->setPlaceholderText("燃烧的胸毛");


    ui->labEdit->move(2, 2);
    ui->labEdit->setFixedHeight(20);
    ui->labEdit->setMaxLength(10);
    ui->inputWid->hide();

    _tip_cur_point = QPoint(5, 5);

    _tip_data = { "同学","家人","菜鸟教程","C++ Primer","Rust 程序设计",
                 "父与子学Python","nodejs开发指南","go 语言开发指南",
                 "游戏伙伴","金融投资","微信读书","拼多多拼友" };

    connect(ui->moreLab, &ClickedOnceLabel::clicked, this, &ApplyFriend::ShowMoreLabel);
    InitTipLabs();
    //链接输入标签回车事件
    connect(ui->labEdit, &CustomizeEdit::returnPressed, this, &ApplyFriend::SlotLabelEnter);
    connect(ui->labEdit, &CustomizeEdit::textChanged, this, &ApplyFriend::SlotLabelTextChange);
    connect(ui->labEdit, &CustomizeEdit::editingFinished, this, &ApplyFriend::SlotLabelEditFinished);
    connect(ui->tipLab, &ClickedOnceLabel::clicked, this, &ApplyFriend::SlotAddFirendLabelByClickTip);

    ui->scrollArea->horizontalScrollBar()->setHidden(true);
    ui->scrollArea->verticalScrollBar()->setHidden(true);
    ui->scrollArea->installEventFilter(this);
    ui->sureBtn->setState("normal","hover","press");
    ui->cancelBtn->setState("normal","hover","press");
    //连接确认和取消按钮的槽函数
    connect(ui->cancelBtn, &QPushButton::clicked, this, &ApplyFriend::SlotApplyCancel);
    connect(ui->sureBtn, &QPushButton::clicked, this, &ApplyFriend::SlotApplySure);
}

ApplyFriend::~ApplyFriend()
{
    delete ui;
}

void ApplyFriend::InitTipLabs()
{
    int lines = 1;
    for(int i = 0; i < _tip_data.size(); i++){

        auto* lb = new ClickedLabel(ui->labListWid);
        lb->SetState("normal", "hover", "pressed", "selected_normal",
                     "selected_hover", "selected_pressed");
        lb->setObjectName("tipslb");
        lb->setText(_tip_data[i]);
        //connect(lb, &ClickedLabel::clicked, this, &ApplyFriend::SlotChangeFriendLabelByTip);

        QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
        int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
        int textHeight = fontMetrics.height(); // 获取文本的高度
        // todo

        if (_tip_cur_point.x() + textWidth + tip_offset > ui->labListWid->width()) {
            lines++;
            if (lines > 2) {
                delete lb;
                return;
            }

            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

        }

        auto next_point = _tip_cur_point;

        AddTipLabs(lb, _tip_cur_point,next_point, textWidth, textHeight);

        _tip_cur_point = next_point;
    }
}

void ApplyFriend::AddTipLabs(ClickedLabel *lb, QPoint cur_point, QPoint& next_point, int text_width, int text_height)
{
    lb->move(cur_point);
    lb->show();
    _add_labels.insert(lb->text(), lb);
    _add_label_keys.push_back(lb->text());
    next_point.setX(lb->pos().x() + text_width + 15);
    next_point.setY(lb->pos().y());
}

bool ApplyFriend::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->scrollArea && event->type() == QEvent::Enter)
    {
        ui->scrollArea->verticalScrollBar()->setHidden(false);
    }
    else if (watched == ui->scrollArea && event->type() == QEvent::Leave)
    {
        ui->scrollArea->verticalScrollBar()->setHidden(true);
    }
    return QObject::eventFilter(watched, event);
}

void ApplyFriend::SetSearchInfo(std::shared_ptr<SearchInfo> si)
{
    _si = si;
    auto applyname = UserMgr::GetInstance()->getName();
    auto bakname = si->_name;

    ui->nameEdit->setText(applyname);
    ui->backEdit->setText(bakname);
}

void ApplyFriend::ResetLabs()
{
    auto max_width = ui->gridWid->width();
    auto label_height = 0;
    for(auto iter = _friend_labels.begin(); iter != _friend_labels.end(); iter++){
        //todo... 添加宽度统计
        if( _label_point.x() + iter.value()->width() > max_width) {
            _label_point.setY(_label_point.y()+iter.value()->height()+6);
            _label_point.setX(2);
        }

        iter.value()->move(_label_point);
        iter.value()->show();

        _label_point.setX(_label_point.x()+iter.value()->width()+2);
        _label_point.setY(_label_point.y());
        label_height = iter.value()->height();
    }

    if(_friend_labels.isEmpty()){
        ui->labEdit->move(_label_point);
        return;
    }

    if(_label_point.x() + MIN_APPLY_LABEL_ED_LEN > ui->gridWid->width()){
        ui->labEdit->move(2,_label_point.y()+label_height+6);
    }else{
        ui->labEdit->move(_label_point);
    }
}

void ApplyFriend::AddLabel(QString name)
{
    if(_friend_labels.find("name")!=_friend_labels.end()){
        return;
    }
    auto temp_lab = new FriendLab(ui->gridWid);
    temp_lab->SetText(name);
    temp_lab->setObjectName("FrientLabel");

    auto maxWidth = ui->gridWid->width();
    if(_label_point.x()+temp_lab->Width()>maxWidth){
        _label_point.setY(_label_point.y()+temp_lab->Height()+6);
        _label_point.setX(2);
    }
    temp_lab->move(_label_point);
    temp_lab->show();
    _friend_labels[name] = temp_lab;
    _friend_label_keys.push_back(name);
    connect(temp_lab,&FriendLab::sig_close,this,&ApplyFriend::SlotRemoveFriendLabel);


    _label_point.setX(_label_point.x()+temp_lab->width()+6);

    if(_label_point.x()+MIN_APPLY_LABEL_ED_LEN+2>maxWidth){
        ui->labEdit->move(2,_label_point.x()+temp_lab->Height()+2);
    }else{
        ui->labEdit->move(_label_point);
    }
    ui->labEdit->clear();
    if (ui->gridWid->height() < _label_point.y() + temp_lab->height() + 2) {
        ui->gridWid->setFixedHeight(_label_point.y() + temp_lab->height() * 2 + 2);
    }
}

void ApplyFriend::ShowMoreLabel()
{
    ui->moreLabWid->hide();

    ui->labListWid->setFixedWidth(325);
    _tip_cur_point = QPoint(5,5);

    auto next_point = _tip_cur_point;
    int textWidth,textHeight;
    for(auto& key:_add_label_keys){
        auto add_lab = _add_labels[key];
        QFontMetrics fontmetrics = QFontMetrics(add_lab->font());
        textWidth = fontmetrics.horizontalAdvance(add_lab->text());
        textHeight = fontmetrics.height();
        if(_tip_cur_point.x()+textWidth+tip_offset>ui->labListWid->width()){
            _tip_cur_point.setY(_tip_cur_point.y()+textHeight+15);
            _tip_cur_point.setX(5);
        }
        add_lab->move(_tip_cur_point);
        next_point.setX(add_lab->pos().x()+textWidth+15);
        next_point.setY(_tip_cur_point.y());

        _tip_cur_point = next_point;
    }
    for(int i =0;i<_tip_data.size();i++){
        auto iter = std::find(_add_label_keys.begin(),_add_label_keys.end(),_tip_data[i]);
        if(iter!=_add_label_keys.end()){
            continue;
        }

        auto lb = new ClickedLabel(ui->labListWid);
        lb->SetState("normal", "hover", "pressed", "selected_normal",
                     "selected_hover", "selected_pressed");
        lb->setObjectName("tipslb");
        lb->setText(_tip_data[i]);
        connect(lb,&ClickedLabel::clicked,this,&ApplyFriend::SlotChangeFriendLabelByTip);
        QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
        int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
        int textHeight = fontMetrics.height(); // 获取文本的高度

        if (_tip_cur_point.x() + textWidth + tip_offset > ui->labListWid->width()) {

            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

        }

        next_point = _tip_cur_point;

        AddTipLabs(lb, _tip_cur_point, next_point, textWidth, textHeight);

        _tip_cur_point = next_point;
    }
    int diff_height = next_point.y() + textHeight + tip_offset - ui->labListWid->height();
    ui->labListWid->setFixedHeight(next_point.y() + textHeight + tip_offset);

    //qDebug()<<"after resize ui->lb_list size is " <<  ui->lb_list->size();
    ui->scrollContents->setFixedHeight(ui->scrollContents->height()+diff_height);

}

void ApplyFriend::SlotLabelEnter()
{
    if(ui->labEdit->text().isEmpty()){
        return;
    }

    auto text = ui->labEdit->text();
    AddLabel(text);
    auto iter = std::find(_tip_data.begin(),_tip_data.end(),text);
    if(iter == _tip_data.end()){
        _tip_data.push_back(text);
    }


    auto add_iter = _add_labels.find(text);
    if(add_iter!=_add_labels.end()){
        _add_labels[text]->SetCurrState(ClickLbState::Selected);
        return;
    }

    auto lb = new ClickedLabel(ui->labListWid);
    lb->SetState("normal", "hover", "pressed", "selected_normal",
                 "selected_hover", "selected_pressed");
    lb->setObjectName("tipslb");
    lb->setText(text);
    connect(lb,&ClickedLabel::clicked,this,&ApplyFriend::SlotChangeFriendLabelByTip);
    QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
    int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
    int textHeight = fontMetrics.height(); // 获取文本的高度
    if(_tip_cur_point.x()+textWidth+tip_offset+3>ui->labListWid->width()){
        _tip_cur_point.setX(5);
        _tip_cur_point.setY(_tip_cur_point.y()+textHeight+15);
    }
    auto next_point = _tip_cur_point;
    AddTipLabs(lb,_tip_cur_point,next_point,textWidth,textHeight);
    _tip_cur_point = next_point;

    int diff_height = next_point.y() + textHeight + tip_offset - ui->labListWid->height();
    ui->labListWid->setFixedHeight(next_point.y() + textHeight + tip_offset);

    lb->SetCurrState(ClickLbState::Selected);

    ui->scrollContents->setFixedHeight(ui->scrollContents->height() + diff_height);

}

void ApplyFriend::SlotRemoveFriendLabel(QString key)
{
    auto find_iter = _friend_labels.find(key);
    if(find_iter == _friend_labels.end()){
        return;
    }
    _label_point.setX(2);
    _label_point.setY(6);

    auto find_key = _friend_label_keys.end();
    for(auto iter=_friend_label_keys.begin();iter!=_friend_label_keys.end();iter++){
        if(*iter==key){
            find_key = iter;
            break;
        }
    }
    if(find_key!=_friend_label_keys.end()){
        _friend_label_keys.erase(find_key);
    }

    delete find_iter.value();
    _friend_labels.erase(find_iter);
    ResetLabs();

    auto find_add = _add_labels.find(key);
    if(find_add==_add_labels.end()){
        return;
    }
    find_add.value()->SetCurrState(ClickLbState::Normal);
}

void ApplyFriend::SlotChangeFriendLabelByTip(QString lbtext, ClickLbState state)
{
    auto find_iter = _add_labels.find(lbtext);
    if(find_iter == _add_labels.end()){
        return;
    }

    if(state == ClickLbState::Selected){
        //编写添加逻辑
        AddLabel(lbtext);
        return;
    }

    if(state == ClickLbState::Normal){
        //编写删除逻辑
        SlotRemoveFriendLabel(lbtext);
        return;
    }
}

void ApplyFriend::SlotLabelTextChange(const QString &text)
{
    if (text.isEmpty()) {
        ui->tipLab->setText("");
        ui->inputWid->hide();
        return;
    }

    auto iter = std::find(_tip_data.begin(), _tip_data.end(), text);
    if (iter == _tip_data.end()) {
        auto new_text = add_prefix + text;
        ui->tipLab->setText(new_text);
        ui->inputWid->show();
        return;
    }
    ui->tipLab->setText(text);
    ui->inputWid->show();
}

void ApplyFriend::SlotLabelEditFinished()
{
    ui->inputWid->hide();
}

void ApplyFriend::SlotAddFirendLabelByClickTip(QString text)
{
    int index = text.indexOf(add_prefix);
    if (index != -1) {
        text = text.mid(index + add_prefix.length());
    }
    AddLabel(text);

    auto find_it = std::find(_tip_data.begin(), _tip_data.end(), text);
    //找到了就只需设置状态为选中即可
    if (find_it == _tip_data.end()) {
        _tip_data.push_back(text);
    }

    //判断标签展示栏是否有该标签
    auto find_add = _add_labels.find(text);
    if (find_add != _add_labels.end()) {
        find_add.value()->SetCurrState(ClickLbState::Selected);
        return;
    }

    //标签展示栏也增加一个标签, 并设置绿色选中
    auto* lb = new ClickedLabel(ui->labListWid);
    lb->SetState("normal", "hover", "pressed", "selected_normal",
                 "selected_hover", "selected_pressed");
    lb->setObjectName("tipslb");
    lb->setText(text);
    connect(lb, &ClickedLabel::clicked, this, &ApplyFriend::SlotChangeFriendLabelByTip);

    QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
    int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
    int textHeight = fontMetrics.height(); // 获取文本的高度
    qDebug() << "textWidth is " << textWidth;

    if (_tip_cur_point.x() + textWidth+ tip_offset+3 > ui->labListWid->width()) {

        _tip_cur_point.setX(5);
        _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

    }

    auto next_point = _tip_cur_point;

    AddTipLabs(lb, _tip_cur_point, next_point, textWidth,textHeight);
    _tip_cur_point = next_point;

    int diff_height = next_point.y() + textHeight + tip_offset - ui->labListWid->height();
    ui->labListWid->setFixedHeight(next_point.y() + textHeight + tip_offset);

    lb->SetCurrState(ClickLbState::Selected);

    ui->scrollContents->setFixedHeight(ui->scrollContents->height()+ diff_height );
}

void ApplyFriend::SlotApplySure()
{
    hide();
    deleteLater();
    //toddo
}

void ApplyFriend::SlotApplyCancel()
{
    hide();
    deleteLater();
    //toddo
}
