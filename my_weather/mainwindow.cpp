#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "get_city_code.h"
#include<QContextMenuEvent>
#include<QMessageBox>
#include<QByteArray>

#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include<QJsonArray>

#include<QPainter>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    exit_menu = new QMenu(this);
    exit_act = new QAction();
    my_NetAccessManger = new QNetworkAccessManager(this);
    connect(my_NetAccessManger, &QNetworkAccessManager::finished, this, &MainWindow::onReply);
    init_windwos();
    init_ui_list();
//    getWeather("101280901");
    getWeather("揭阳");
    // 给两个标签绑定事件过滤器
    ui->lblHighCurve->installEventFilter(this);
    ui->lblLowCurve->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete exit_menu;
    delete exit_act;
    delete my_NetAccessManger;
    exit_menu = nullptr;
    exit_act = nullptr;
    my_NetAccessManger = nullptr;
}

void MainWindow::init_ui_list()
{
    //将各个ui控件添加进控件list中
    // 星期与日期
    my_week_list << ui->lblWeek0 << ui->lblWeek1 << ui->lblWeek2 << ui->lblWeek3 << ui->lblWeek4 << ui->lblWeek5;
    my_datelist << ui->lblDate0 << ui->lblDate1 << ui->lblDate2 << ui->lblDate3 << ui->lblDate4 << ui->lblDate5;
    // 天气和天气图标
    my_weather_list << ui->lblType0 << ui->lblType1 << ui->lblType2 << ui->lblType3 << ui->lblType4 << ui->lblType5;
    my_weather_icon_list << ui->lblTypeIcon0 << ui->lblTypeIcon1 << ui->lblTypeIcon2 << ui->lblTypeIcon3 << ui->lblTypeIcon4 << ui->lblTypeIcon5;
    // 天气指数
    my_aqilist << ui->lblQuality0 << ui->lblQuality1 << ui->lblQuality2 << ui->lblQuality3 << ui->lblQuality4 << ui->lblQuality5;
    // 风向和风力
    my_fxlist << ui->lblFx0 << ui->lblFx1 << ui->lblFx2 << ui->lblFx3 << ui->lblFx4 << ui->lblFx5;
    my_fllist << ui->lblFl0 << ui->lblFl1 << ui->lblFl2 << ui->lblFl3 << ui->lblFl4 << ui->lblFl5;
    //
    my_icon_map.insert("暴雪", ":/icon/res/type/BaoXue.png");
    my_icon_map.insert("暴雨", ":/icon/res/type/BaoYu.png");
    my_icon_map.insert("暴雨到大暴雨", ":/icon/res/type/BaoYuDaoDaBaoYu.png");
    my_icon_map.insert("大暴雨", ":/icon/res/type/DaBaoYu.png");
    my_icon_map.insert("大暴雨到特大暴雨", ":/icon/res/type/DaBaoYuDaoTeDaBaoYu.png");
    my_icon_map.insert("大到暴雪", ":/icon/res/type/DaDaoBaoXue.png");
    my_icon_map.insert("大到暴雨", ":/icon/res/type/DaDaoBaoYu.png");
    my_icon_map.insert("大雪", ":/icon/res/type/DaXue.png");
    my_icon_map.insert("大雨", ":/icon/res/type/DaYu.png");
    my_icon_map.insert("冻雨", ":/icon/res/type/DongYu.png");
    my_icon_map.insert("多云", ":/icon/res/type/DuoYun.png");
    my_icon_map.insert("浮沉", ":/icon/res/type/FuChen.png");
    my_icon_map.insert("雷阵雨", ":/icon/res/type/LeiZhenYu.png");
    my_icon_map.insert("雷阵雨伴有冰雹", ":/icon/res/type/LeiZhenYuBanYouBingBao.png");
    my_icon_map.insert("霾", ":/icon/res/type/Mai.png");
    my_icon_map.insert("强沙尘暴", ":/icon/res/type/QiangShaChenBao.png");
    my_icon_map.insert("晴", ":/icon/res/type/Qing.png");
    my_icon_map.insert("沙尘暴", ":/icon/res/type/ShaChenBao.png");
    my_icon_map.insert("特大暴雨", ":/icon/res/type/TeDaBaoYu.png");
    my_icon_map.insert("undefined", ":/icon/res/type/undefined.png");
    my_icon_map.insert("雾", ":/icon/res/type/Wu.png");
    my_icon_map.insert("小到中雪", ":/icon/res/type/XiaoDaoZhongXue.png");
    my_icon_map.insert("小到中雨", ":/icon/res/type/XiaoDaoZhongYu.png");
    my_icon_map.insert("小雪", ":/icon/res/type/XiaoXue.png");
    my_icon_map.insert("小雨", ":/icon/res/type/XiaoYu.png");
    my_icon_map.insert("雪", ":/icon/res/type/Xue.png");
    my_icon_map.insert("扬沙", ":/icon/res/type/YangSha.png");
    my_icon_map.insert("阴", ":/icon/res/type/Yin.png");
    my_icon_map.insert("雨", ":/icon/res/type/Yu.png");
    my_icon_map.insert("雨夹雪", ":/icon/res/type/YuJiaXue.png");
    my_icon_map.insert("阵雪", ":/icon/res/type/ZhenXue.png");
    my_icon_map.insert("阵雨", ":/icon/res/type/ZhenYu.png");
    my_icon_map.insert("中到大雪", ":/icon/res/type/ZhongDaoDaXue.png");
    my_icon_map.insert("中到大雨", ":/icon/res/type/ZhongDaoDaYu.png");
    my_icon_map.insert("中雪", ":/icon/res/type/ZhongXue.png");
    my_icon_map.insert("中雨", ":/icon/res/type/ZhongYu.png");
}

void MainWindow::init_windwos()
{
    setWindowFlag(Qt::FramelessWindowHint);//设置窗口无边框
    setFixedSize(900, 500); // 设置固定窗口大小
    exit_act->setText("退出");
    exit_act->setIcon(QIcon(":/icon/res/close.png"));
    exit_menu->addAction(exit_act);
    connect(exit_act, SIGNAL(triggered()), this, SLOT(quit_app()));
}


// 重写父类的虚函数，默认右键没有菜单，重写之后就有了。
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    // 右键菜单->退出
    exit_menu->exec(QCursor::pos());
    //调用accept，表示这个事件已经处理，不再向上传递。
    event->accept();
}

// 重写鼠标按住功能。
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mouse_offset = event->globalPos() - this->pos();
}

// 重写移动功能，搭配按住形成拖拽功能
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - mouse_offset);
}

void MainWindow::getWeather(QString city_name)
{
    QString city_code = get_city_code::get_code(city_name);
    if (city_code.isEmpty()) {
//        QMessageBox error_message;
//        error_message.setText("请输入正确的城市名");
//        error_message.exec();
        QMessageBox::warning(this, "城市", "请输入正确的城市名", QMessageBox::Ok);
        return;
    }
    QUrl url = ("http://t.weather.itboy.net/api/weather/city/" + city_code);
    my_NetAccessManger->get(QNetworkRequest(url));
}



void MainWindow::quit_app()
{
    qApp->exit(0);
}

void MainWindow::onReply(QNetworkReply *reply)
{
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status_code != 200 || reply->error() != QNetworkReply::NoError) { //请求异常
        QMessageBox::warning(this, "天气", reply->errorString(), QMessageBox::Ok);
    } else {
        QByteArray reply_data = reply->readAll();
        parse_json_data(reply_data); //传参时 不要解引用
    }
    reply->deleteLater();//释放内存
}

void MainWindow::parse_json_data(QByteArray &byte_array)
{
    QJsonParseError json_error;
    QJsonDocument json_doc = QJsonDocument::fromJson(byte_array, &json_error);
    if (json_error.error != QJsonParseError::NoError) {// 异常状态不解析
        return;
    }
    QJsonObject tree_obj = json_doc.object();
    //----
    // 获取昨天的数据
    QJsonObject yesterday_data = tree_obj.value("data").toObject().value("yesterday").toObject();
    QString s_high = yesterday_data.value("high").toString().split(" ").at(1);
    QString s_low = yesterday_data.value("low").toString().split(" ").at(1);
    m_day[0].week = yesterday_data.value("week").toString();
    m_day[0].date = yesterday_data.value("ymd").toString();
    m_day[0].high  = s_high.left(s_high.length() - 1).toInt();
    m_day[0].low = s_low.left(s_low.length() - 1).toInt();
    m_day[0].type = yesterday_data.value("type").toString();
    m_day[0].fx = yesterday_data.value("fx").toString();
    m_day[0].fl = yesterday_data.value("fl").toString();
    m_day[0].notice = yesterday_data.value("notice").toString();
    m_day[0].aqi = yesterday_data.value("aqi").toDouble();
    // 解析forcast中五天的数据.
    QJsonArray forecast_data =  tree_obj.value("data").toObject().value("forecast").toArray();
    for (int i = 0; i < 5; i++) {
        QJsonObject forecast_obj = forecast_data[i].toObject();
        // 因为0存放的是昨天的数据，所以数组下标+1
        //高温低温
        QString f_s_high = forecast_obj.value("high").toString().split(" ").at(1);
        QString f_s_low = forecast_obj.value("low").toString().split(" ").at(1);
        //
        m_day[i + 1].week = forecast_obj.value("week").toString();
        m_day[i + 1].date = forecast_obj.value("ymd").toString();
        m_day[i + 1].high  = f_s_high.left(f_s_high.length() - 1).toInt();
        m_day[i + 1].low = f_s_low.left(f_s_low.length() - 1).toInt();
        m_day[i + 1].type = forecast_obj.value("type").toString();
        m_day[i + 1].fx = forecast_obj.value("fx").toString();
        m_day[i + 1].fl = forecast_obj.value("fl").toString();
        m_day[i + 1].notice = forecast_obj.value("notice").toString();
        m_day[i + 1].aqi = forecast_obj.value("aqi").toDouble();
    }
    //
    // 获取今天的各项数据
    my_today.date = tree_obj.value("date").toString();
    my_today.city = tree_obj.value("cityInfo").toObject().value("city").toString();
    my_today.shidu = tree_obj.value("data").toObject().value("shidu").toString();
    my_today.pm25 = tree_obj.value("data").toObject().value("pm25").toInt();
    my_today.quality = tree_obj.value("data").toObject().value("quality").toString();
    my_today.wendu = tree_obj.value("data").toObject().value("wendu").toString().toInt();
    my_today.ganmao = tree_obj.value("data").toObject().value("ganmao").toString();
    // 因为m_day[1]里面存放的数据实际上就是今天的数据，因此将m_day[1]的数据赋给my_today。
    my_today.type = m_day[1].type;
    my_today.fx = m_day[1].fx;
    my_today.fl = m_day[1].fl;
    my_today.high = m_day[1].high;
    my_today.low = m_day[1].low;
    my_today.aqi = m_day[1].aqi;
    my_today.notice = m_day[1].notice;
    my_today.week = m_day[1].week;
    // 解析完毕所有数据，然后更新UI
    update_ui();
    // 对绘制的图形进行一次更新
    ui->lblHighCurve->update();
    ui->lblLowCurve->update();
}

void MainWindow::update_ui()
{
    // 更新各个UI
    //利用Qdatetime这个类里面的fromstring对yyyymmdd格式的日期进行转换
    // 更新日期和城市
    ui->lb_date->setText(QDateTime::fromString(my_today.date, "yyyyMMdd").toString("yyyy/MM/dd") + " " + my_today.week);
    ui->lb_city->setText(my_today.city);
    //更新今天的数据
//    QString t_wendu =  QString::number(my_today.wendu) + "℃";
    ui->lb_max_temperature->setText(QString::number(my_today.wendu) + "℃");
    ui->lb_weather_icon->setPixmap(my_icon_map[my_today.type]);
    ui->lb_weather->setText(my_today.type);
    ui->lb_max_min->setText(QString::number(my_today.low) + "~" + QString::number(my_today.high) + "℃");
    ui->lbGaoMao->setText("感冒指数:" + my_today.ganmao);
    ui->lanotice->setText("注意:" + my_today.notice);
    ui->lbWind_fx->setText(my_today.fx);
    ui->lbWind_fl->setText(my_today.fl);
    ui->lbAQI_num->setText(QString::number(my_today.aqi));
    ui->lbShidu_num->setText(my_today.shidu);
    ui->lbPM25_num->setText(QString::number(my_today.pm25));
    // 更新昨天、后面五天的数据。
    for (int i = 0; i < 6; i++) {
        my_weather_icon_list[i]->setPixmap(my_icon_map[m_day[i].type]);
        my_week_list[i]->setText("周" + m_day[i].week.right(1));
        my_week_list[0]->setText("昨天");
        my_week_list[1]->setText("今天");
        my_week_list[2]->setText("明天");
        QStringList ymdList = m_day[i].date.split("-");
        my_datelist[i]->setText(ymdList.at(1) + "-" + ymdList.at(2));
        // 更新天气类型
        my_weather_list[i]->setText(m_day[i].type);
        // 更新空气质量
        switch (checked_aqi(m_day[i].aqi)) {
            case 1:
                my_aqilist[i]->setText("优");
                my_aqilist[i]->setStyleSheet("background-color: rgb(121,184,0);");
                break;
            case 2:
                my_aqilist[i]->setText("良");
                my_aqilist[i]->setStyleSheet("background-color: rgb(255,187,23);");
                break;
            case 3:
                my_aqilist[i]->setText("轻度污染");
                my_aqilist[i]->setStyleSheet("background-color: rgb(255,87,97);");
                break;
            case 4:
                my_aqilist[i]->setText("中度污染");
                my_aqilist[i]->setStyleSheet("background-color: rgb(235,17,27);");
                break;
            case 5:
                my_aqilist[i]->setText("重度污染");
                my_aqilist[i]->setStyleSheet("background-color: rgb(170,0,0);");
                break;
            case 6:
                my_aqilist[i]->setText("严重污染");
                my_aqilist[i]->setStyleSheet("background-color: rgb(110,0,0);");
                break;
        }
        // 更新每天的风力风向
        my_fxlist[i]->setText(m_day[i].fx);
        my_fllist[i]->setText(m_day[i].fl);
    }
}

int MainWindow::checked_aqi(int aqi)
{
    // 判断空气质量的，小于等于50为优
    if (aqi >= 0 && aqi <= 50) {
        return 1;
        // <=100 良
    } else if (aqi > 50 && aqi <= 100) {
        return 2;
        // <= 150 轻度污染
    } else if (aqi > 100 && aqi <= 150) {
        return 3;
        // <= 200 中度污染
    } else if (aqi > 150 && aqi <= 200) {
        return 4;
        // <= 250 重度污染
    } else if (aqi > 200 && aqi <= 250) {
        return 5;
        // >250 严重污染
    } else if (aqi > 250) {
        return 6;
    }
    return -1;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->lblHighCurve && event->type() == QEvent::Paint) {
        //是否是lblHighCurve且事件的类型为Paint
        draw_high_curve();
    }
    if (watched == ui->lblLowCurve && event->type() == QEvent::Paint) {
        //是否是lblLowCurve且事件的类型为Paint
        draw_low_curve();
    }
    //执行完我们的操作后,继续父类的操作
    return QWidget::eventFilter(watched, event);
}

void MainWindow::draw_high_curve()
{
    //设置painter
    QPainter painter(ui->lblHighCurve);
    //设置抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    //构造画笔
    QPen pen = painter.pen();
    //设置画笔的宽度
    pen.setWidth(1);
    //设置画笔的颜色
    pen.setColor(QColor(255, 165, 0)); // Orange
    //设置画笔
    painter.setPen(pen);
    //设置画刷
    painter.setBrush(QColor(255, 165, 0));
    //保存
    painter.save();
    //计算x y的坐标
    //x的坐标: 上面标签的x+标签的宽度/2
    //y的坐标: 先计算全部数据的平均值,将平均值设置在当前label高度/2,计算公式为(温度 - 平均值) * 要移动的像素点
    QPoint pos[6] = {};
    //高温和
    int tempSum = 0;
    //平均高温
    int tempAverage = 0;
    //中心点
    float center = ui->lblHighCurve->height() / 2;
    for (int i = 0; i < 6; ++i) {
        //计算高温和
        tempSum += m_day[i].high;
    }
    //计算平均值
    tempAverage = tempSum / 6;
    for (int i = 0; i < 6; ++i) {
        //计算往上移动还是往下移动
        int offset = (m_day[i].high - tempAverage) * 2;
        //x
        pos[i].setX(my_week_list[i]->pos().x() + my_week_list[i]->width() / 2);
        //y
        pos[i].setY(center - offset);
        //qDebug() << i << QPoint(pos[i].x() - TEXT_OFFSET_X, pos[i].y() - TEXT_OFFSET_Y);
        //绘制点
        painter.drawEllipse(pos[i], 3, 3);
        //绘制文字 减去的偏移量是12px
        painter.drawText(QPoint(pos[i].x() - 12, pos[i].y() - 12), QString::number(m_day[i].high) + "°");
    }
    //只需要5段
    for (int i = 0; i < 5; ++i) {
        //绘制曲线,昨天为虚线,其余的为实线
        if (i == 0) {
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
        } else {
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        //绘制线
        painter.drawLine(pos[i].x(), pos[i].y(), pos[i + 1].x(), pos[i + 1].y());
    }
    //恢复
    painter.restore();
}

void MainWindow::draw_low_curve()
{
    //设置painter
    QPainter painter(ui->lblLowCurve);
    //设置抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    //构造画笔
    QPen pen = painter.pen();
    //设置画笔的宽度
    pen.setWidth(1);
    //设置画笔的颜色
    pen.setColor(QColor(151, 255, 255)); // DarkSlateGray1
    //设置画笔
    painter.setPen(pen);
    //设置画刷
    painter.setBrush(QColor(151, 255, 255));
    //保存
    painter.save();
    //计算x y的坐标
    //x的坐标: 上面标签的x+标签的宽度/2
    //y的坐标: 先计算全部数据的平均值,将平均值设置在当前label高度/2,计算公式为(温度 - 平均值) * 要移动的像素点
    QPoint pos[6] = {};
    //低温和
    int tempSum = 0;
    //平均温度
    int tempAverage = 0;
    //中心点
    float center = ui->lblLowCurve->height() / 2;
    for (int i = 0; i < 6; ++i) {
        //计算最低温度和
        tempSum += m_day[i].low;
    }
    //计算平均值
    tempAverage = tempSum / 6;
    for (int i = 0; i < 6; ++i) {
        //计算往上移动还是往下移动
        int offset = (m_day[i].low - tempAverage) * 2;
        //x
        pos[i].setX(my_week_list[i]->pos().x() + my_week_list[i]->width() / 2);
        //y
        pos[i].setY(center - offset);
        //qDebug() << i << QPoint(pos[i].x() - TEXT_OFFSET_X, pos[i].y() - TEXT_OFFSET_Y);
        //绘制点
        painter.drawEllipse(pos[i], 3, 3);
        //绘制文字 减去的偏移量是12px
        painter.drawText(QPoint(pos[i].x() - 12, pos[i].y() - 12), QString::number(m_day[i].low) + "°");
    }
    //只需要5段
    for (int i = 0; i < 5; ++i) {
        //绘制曲线,昨天为虚线,其余的为实线
        if (i == 0) {
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
        } else {
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        //绘制线
        painter.drawLine(pos[i].x(), pos[i].y(), pos[i + 1].x(), pos[i + 1].y());
    }
    //恢复
    painter.restore();
}

void MainWindow::on_btn_search_clicked()
{
    QString city_name = ui->le_city->text();
    getWeather(city_name);
}
