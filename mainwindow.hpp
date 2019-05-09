#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "fullchart.hpp"
#include "pushbullet.hpp"
#include "unistd.h"

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
    void check_order(long id);
    void init_chart();
    void update_chart();
    void check_time();
    double compute_wma(long id, int n);
    void change_view();
    void update_Yranges();
    void update_Xranges();
    void update_infos();
    bool send_buy_order();
    bool send_sell_order();
    void trade_buy();
    void trade_sell();
    void trade_hull(double &curr_price, double &hma, double &tema);

private:
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

    double _last_buy_price = 10000.0;
    double _last_buy_volume = 0.0;
    double _last_sell_price = 0.0;
    double _last_sell_volume = 0.0;
    double _trade_count = 0.0;

    double _ask_price;
    double _bid_price;

    long _oldest = 0;
    long _lfp_time = 0.0;
    bool _viewchanged = false;

    int _hull_period = 100;
    int _tema_period = 20;
    double _last_ema1 = 0.0;
    double _last_ema2 = 0.0;
    double _last_ema3 = 0.0;

    const char *_interval = "15m";
    const char *_pair = "BTCUSDT";

    Position _current_move = STOPPED_LONGING;
    double _initial_balance;
    double _balance_fiat = 175.0;
    double _balance_crypto = 0.0;

    string _api_key = "";
    string _secret_key = "";

    Pushbullet _pushbullet = Pushbullet("");
};

#endif // MAINWINDOW_H
