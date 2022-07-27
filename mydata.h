#ifndef MYDATA_H
#define MYDATA_H

#include <QString>
#include <QVector>

class MyData
{
public:
    MyData();
    void setMyStr(QString str);
    void setMyVec();

private:
    QString myStr;
    QVector<char> myVec;

};

#endif // MYDATA_H
