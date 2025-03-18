#include "textbubble.h"

#include <QEvent>
#include <QTextLayout>
#include <QTextDocument>
#include <QTextBlock>
#include <QFontMetricsF>
TextBubble::TextBubble(ChatRole role, const QString &text, QWidget *parent):BubbleFrame(role,parent)
{
    _textEdit = new QTextEdit();
    _textEdit->setReadOnly(true);
    _textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _textEdit->installEventFilter(this);
    QFont font("Microsoft YaHei");
    font.setPointSize(12);
    _textEdit->setFont(font);
    setPlainText(text);
    setWidget(_textEdit);
    initStyleSheet();
}

bool TextBubble::eventFilter(QObject *o, QEvent *e)
{
    if(_textEdit==o&&e->type()==QEvent::Paint){
        adjustTextHeight();
    }
    return BubbleFrame::eventFilter(o,e);
}

void TextBubble::adjustTextHeight()
{
    qreal doc_margin = _textEdit->document()->documentMargin();
    QTextDocument *doc= _textEdit->document();
    qreal text_height = 0;
    for (auto it = doc->begin(); it != doc->end(); it = it.next())
    {
        QTextLayout *pLayout = it.layout();
        QRectF text_rect = pLayout->boundingRect();                             //这段的rect
        text_height += text_rect.height();
    }
    int vMargin = this->layout()->contentsMargins().top();
    //设置这个气泡需要的高度 文本高+文本边距+TextEdit边框到气泡边框的距离
    setFixedHeight(text_height + doc_margin *2 + vMargin*2 );
}

void TextBubble::setPlainText(const QString &text)
{
    _textEdit->setPlainText(text);
    //m_pTextEdit->setHtml(text);
    //找到段落中最大宽度
    qreal doc_margin = _textEdit->document()->documentMargin();
    int margin_left = this->layout()->contentsMargins().left();
    int margin_right = this->layout()->contentsMargins().right();
    QFontMetricsF fm(_textEdit->font());

    QTextDocument *doc = _textEdit->document();
    int max_width = 0;
    //遍历每一段找到 最宽的那一段
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())    //字体总长
    {

        int txtW = int(fm.horizontalAdvance(it.text()));
        max_width = max_width < txtW ? txtW : max_width;                 //找到最长的那段
    }
    //设置这个气泡的最大宽度 只需要设置一次
    setMaximumWidth(max_width + doc_margin * 2 + (margin_left + margin_right));
}

void TextBubble::initStyleSheet()
{
    _textEdit->setStyleSheet("QTextEdit{background:transparent;border:none}");
}
