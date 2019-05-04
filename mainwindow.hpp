#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "fullchart.hpp"
#include "pushbullet.hpp"

using namespace std;

struct Kline {
    long time;
    double openprice;
    double highprice;
    double lowprice;
    double closeprice;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum Position
    {
        SHORT = 0,
        LONG = 1,
        STOPPED_SHORTING = 2,
        STOPPED_LONGING = 3
    };

public:
    MainWindow();
    ~MainWindow();
    void get_klines();
    void get_market();
    void get_account();
    void init_chart();
    void update_chart();
    void check_time();
    double compute_wma(long id, int n);
    void change_view();
    void update_Yranges();
    void update_Xranges();
    void update_infos();
    void send_buy_order();
    void send_sell_order();
    void trade_buy(double &price);
    void trade_sell(double &price);
    void trade_hull(double &curr_price, double &hma1, double &hma2);

private:
    string _api_key = "key";
    string _secret_key = "secret";
    vector<Kline> _klines;
    QPushButton *_btn_resetview;
    FullChart *_price_chart;
    QTextEdit *_logs;
    QLabel *_show_balance;
    QLabel *_show_overall_gain;
    QLabel *_show_trade_count;
    QGridLayout *_glayout;
    QGridLayout *_main_layout;

    QTimer *_timer_wait;
    QTimer *_timer_check;

    double _last_buy_price = 0.0;
    double _last_buy_volume = 0.0;
    double _last_short_price = 0.0;
    double _last_short_volume = 0.0;
    double _trade_count = 0.0;

    double _ask_price;
    double _bid_price;

    long _oldest = 0;
    long _latest_finished_period = 0.0;
    bool _viewchanged = false;

    int _hull_period = 36;
    int _hull_period2 = 9;

    const char *_interval = "1h";
    const char *_pair = "ETHUSDT";

    Position _current_move = STOPPED_LONGING;
    double _balance_fiat = 175.0;
    double _balance_crypto = 0.0;

    Pushbullet _pushbullet = Pushbullet("api_key");
};

#endif // MAINWINDOW_H
