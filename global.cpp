#include "global.h"

std::function<void(QWidget*)> repolish = [](QWidget* w){
    w->style()->unpolish(w);
    w->style()->polish(w);
};
std::function<QString(QString)> xorstring = [](QString input){
    QString result = input;
    int length = input.length();
    length %= 255;
    for(int i =0;i<length;i++){
        result[i]=QChar(ushort(result[i].unicode()^(ushort)length));
    }
    return result;
};
QString gate_url_prefix = "";
