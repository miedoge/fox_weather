#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMenu>

#include<QNetworkAccessManager>
#include<QNetworkReply> //添加俩网络类用于http请求

#include<QList>
#include<QLabel>// 用于读取到数据->更新ui

#include<QMap>
#include "weather.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void init_ui_list();
    void init_windwos();
    void contextMenuEvent(QContextMenuEvent *event);// 重写右键菜单功能
    void mousePressEvent(QMouseEvent *event);// 重写鼠标左键按住功能，搭配移动形成拖拽
    void mouseMoveEvent(QMouseEvent *event);// 搭配按住功能形成拖拽。
    void getWeather(QString city_code);
    void parse_json_data(QByteArray &byte_array);
    void update_ui();
    int checked_aqi(int aqi); // 用于检测空气质量指数
    bool eventFilter(QObject *watched, QEvent *event);// 重写父类even filter方法
    void draw_high_curve();
    void draw_low_curve();
private:
    Ui::MainWindow *ui;
    QMenu *exit_menu; // 菜单->退出
    QAction *exit_act;// 退出行为
    QPoint mouse_offset;// 用于计算拖拽的移动距离量 窗口移动时，鼠标与窗口左上角的偏移
    QNetworkAccessManager *my_NetAccessManger; // 负责http请求
    Today my_today;
    Day m_day[6];//一周七天
    // 星期和日期的UI
    QList<QLabel *> my_week_list;
    QList<QLabel *> my_datelist;

    //天气污染指数
    QList<QLabel *> my_aqilist;

    //天气与天气图标
    QList<QLabel *> my_weather_list;
    QList<QLabel *> my_weather_icon_list;

    // 风力和风向
    QList<QLabel *> my_fllist;
    QList<QLabel *> my_fxlist;

    // 存放图标和图标路径的map，用于更新图标，用图标的字符作为key，路径作为value。
    QMap<QString, QString> my_icon_map;
private slots:
    void quit_app();
    void onReply(QNetworkReply *reply);
    void on_btn_search_clicked();
};
#endif // MAINWINDOW_H
