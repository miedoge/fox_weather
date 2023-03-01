#include "get_city_code.h"
#include<QMap>
#include<QFile>
#include<QByteArray>

#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include<QJsonArray>
#include<QJsonParseError>
#include<QDebug>


get_city_code::get_city_code()
{
}

QMap<QString, QString>get_city_code::city_map = {};

void get_city_code::init_city_map()
{
    QString file_path = ":/json/res/citycode.json";
    QFile file(file_path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray json_file = file.readAll();
    file.close();
    // 解析json_file,将其写入city_map,key是城市名，value是code
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json_file, &err);
    if (err.error != QJsonParseError::NoError) {
//        qDebug() << "解析出错";
        return;
    }
    if (!doc.isArray()) {
//        qDebug() << "doc解析的文件非json数组！";
        return;
    }
    QJsonArray city_arr = doc.array();
    for (int i = 0 ; i < city_arr.size(); i++) {
        QString city = city_arr[i].toObject().value("city_name").toString();
        QString code = city_arr[i].toObject().value("city_code").toString();
        if (code.size() > 0) {
            city_map.insert(city, code);
        }
    }
}

QString get_city_code::get_code(QString city_name)
{
    if (city_map.isEmpty()) {
        init_city_map();
    }
    auto it = city_map.find(city_name);
    if (it == city_map.end()) {
        it  = city_map.find(city_name + "市");
    }
    if (it != city_map.end()) {
        return it.value();
    }
    return "";
}
