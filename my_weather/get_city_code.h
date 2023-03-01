#ifndef GET_CITY_CODE_H
#define GET_CITY_CODE_H

#include<QMap>
class get_city_code
{
public:
    get_city_code();


    static QString get_code(QString city_name);

private:
    static QMap<QString, QString> city_map;
    static void init_city_map();
};

#endif // GET_CITY_CODE_H
