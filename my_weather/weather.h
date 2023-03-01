#ifndef WEATHER_H
#define WEATHER_H
#include<QString>

class Today
{
public: Today()
    {
        date = "2022-10-20";
        city = "广州";
        ganmao = "感冒指数";
        wendu = 0;
        shidu = "0%";
        pm25 = 0;
        quality = "无数据";
        type = "多云";
        fl = "2级";
        fx = "南风";
        high = 30;
        low = 18;
        notice = "123";
    }

public:
    QString date;
    QString city;
    QString ganmao;
    QString notice;
    QString type;
    QString fx;
    QString fl;
    QString week;
    QString shidu;
    int pm25;
    QString quality;

    double aqi;

    int wendu;
    int high;
    int low;

};

class Day: public Today
{
public: Day() {}
};





#endif // WEATHER_H
