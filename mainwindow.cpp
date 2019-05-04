#include "mainwindow.hpp"
#include "math.h"
#include "binacpp.h"


using namespace std;

MainWindow::MainWindow()
{

    showMaximized();
    _btn_resetview = new QPushButton("reset view",this);
    _btn_resetview->setFixedSize(100,35);

    QPixmap back_pix("/home/amo/Images/btctradingbig.jpg");
    QPalette palette;
    palette.setBrush(QPalette::Background, back_pix);
    this->setPalette(palette);

    _price_chart = new FullChart(this);
    _price_chart->setMaximumHeight(450);

    _logs = new QTextEdit(this);
    _logs->setReadOnly(true);
    _logs->setFixedWidth(400);
    _logs->setMaximumHeight(400);

    _show_balance = new QLabel(this);
    _show_balance->setFixedSize(150,50);
    _show_balance->setStyleSheet("border: 1px solid; border-radius:5px; background-color: lime; ");

    _show_trade_count = new QLabel(this);
    _show_trade_count->setFixedSize(150,50);
    _show_trade_count->setStyleSheet("border: 1px solid; border-radius:5px; background-color: white; ");

    _show_overall_gain = new QLabel(this);
    _show_overall_gain->setFixedSize(150,50);
    _show_overall_gain->setStyleSheet("border: 1px solid; border-radius:5px; background-color: white; ");

    _glayout = new QGridLayout();
    _glayout->addWidget(_btn_resetview, 0, 0, Qt::AlignCenter);
    _glayout->addWidget(_show_balance, 0, 1);
    _glayout->addWidget(_show_overall_gain, 1, 1);
    _glayout->addWidget(_show_trade_count, 1, 0);

    _main_layout = new QGridLayout();
    _main_layout->addWidget(_price_chart,0,0,2,1);
    _main_layout->addLayout(_glayout,0,1);
    _main_layout->addWidget(_logs,1,1);

    QWidget *window = new QWidget();
    window->setLayout(_main_layout);
    setCentralWidget(window);

    BinaCPP::init(_api_key , _secret_key);

    get_klines();
    get_market();


    _pushbullet.push_note("Katie", "test curl");
    get_account();
    //init_chart();

    _timer_wait = new QTimer(this);
    _timer_wait->setInterval(3600000);

    _timer_check = new QTimer(this);
    _timer_check->setInterval(1000);
    _timer_check->start();

    connect(_price_chart, &FullChart::topChartUpdated, this, &MainWindow::change_view);
    connect(_timer_wait,  &QTimer::timeout, this, &MainWindow::update_chart);
    connect(_timer_check, &QTimer::timeout, this, &MainWindow::check_time);
    connect(_btn_resetview, &QPushButton::clicked, this, &MainWindow::update_Xranges);

}

void MainWindow::init_chart()
{
    // get prices from Binance
    get_klines();

    // display prices and indicators
    for ( vector<Kline>::iterator it = _klines.begin(); it != _klines.end() - 1; ++it) {

        double current_period = it-_klines.begin();
        long currtime = it->time;


        if (current_period > (_hull_period + sqrt(_hull_period))) {

            //HMA1
            double hma1 = 0.0;
            double total = 0.0;
            for (int i = 1; i < (sqrt(_hull_period)+1); ++i){
                total += i;
                hma1 += i * ((compute_wma(current_period - sqrt(_hull_period) + i, (_hull_period / 2))) * 2 -
                            compute_wma(current_period - sqrt(_hull_period) + i, _hull_period));
            }
            hma1 = hma1/total;

            //HMA2
            double hma2 = 0.0;
            total = 0.0;
            for (int i = 1; i < (sqrt(_hull_period2)+1); ++i){
                total += i;
                hma2 += i * ((compute_wma(current_period - sqrt(_hull_period2) + i, (_hull_period2 / 2))) * 2 -
                            compute_wma(current_period - sqrt(_hull_period2) + i, _hull_period2));
            }
            hma2 = hma2/total;

            //chart
            _price_chart->setSerie(currtime, it->closeprice );
            _price_chart->setSerie2(currtime, hma1);
            _price_chart->setSerie4(currtime, hma2);

            //setup trading
            if (hma1 < hma2){
                _current_move = STOPPED_LONGING;
            }
            else _current_move = SHORT;
        }
    }
    _oldest = (_klines.back().time) / 1000;
    _latest_finished_period = (_klines[_klines.size() - 2].time);
    update_Xranges();
}

double MainWindow::compute_wma(long id, int n)
{
    double wma = 0.0;
    int i = 1;
    int total = 0;
    for ( vector<Kline>::iterator it = _klines.begin() + id - n + 1; it != _klines.begin() + id + 1; ++it, ++i) {
        wma += it->closeprice * i;
        total += i;
    }
    return (wma/total);
}

void MainWindow::change_view()
{
    _viewchanged = true;
    update_Yranges();
}

void MainWindow::update_Yranges()
{
    long timemax = _price_chart->timeAxis()->max().toMSecsSinceEpoch();
    long timemin = _price_chart->timeAxis()->min().toMSecsSinceEpoch();
    double max = 0;
    double min = 1000000;
    for ( vector<Kline>::iterator it = _klines.begin(); it != _klines.end(); ++it) {
        if (it->time > timemin && it->time < timemax){
            if (it->closeprice > max) max=it->closeprice;
            if (it->closeprice < min) min=it->closeprice;
        }
    }
    _price_chart->setYrange(min, max);
}

void MainWindow::update_Xranges()
{
    QDateTime min = QDateTime::fromMSecsSinceEpoch(_klines.back().time).addDays(-1);
    QDateTime max = QDateTime::fromMSecsSinceEpoch(_klines.back().time);
    _price_chart->setXrange(min, max);
    _viewchanged = false;
    update_Yranges();
}

void MainWindow::update_chart()
{
    qDebug() << QDateTime::currentDateTime() << "Starting update";
    // get prices from Binance
    get_klines();
    _latest_finished_period = _klines[_klines.size() - 2].time;
    unsigned long last_finished_period = _klines.size() - 2;
    long lfp_time = _klines[last_finished_period].time;

    //HMA1
    double hma1 = 0.0;
    double total = 0.0;
    for (int i = 1; i < (sqrt(_hull_period)+1); ++i){
        total += i;
        hma1 += i * ((compute_wma(last_finished_period - sqrt(_hull_period) + i, (_hull_period / 2))) * 2 -
                    compute_wma(last_finished_period - sqrt(_hull_period) + i, _hull_period));
    }
    hma1 = hma1/total;

    //HMA2
    double hma2 = 0.0;
    total = 0.0;
    for (int i = 1; i < (sqrt(_hull_period2)+1); ++i){
        total += i;
        hma2 += i * ((compute_wma(last_finished_period - sqrt(_hull_period2) + i, (_hull_period2 / 2))) * 2 -
                    compute_wma(last_finished_period - sqrt(_hull_period2) + i, _hull_period2));
    }
    hma2 = hma2/total;

    //trading
    trade_hull(_klines[last_finished_period].closeprice, hma1, hma2);

    //chart
    _price_chart->setSerie(lfp_time, _klines[_klines.size() - 2].closeprice );
    _price_chart->setSerie2(lfp_time, hma1);
    _price_chart->setSerie4(lfp_time, hma2);
    update_Xranges();
    qDebug() << QDateTime::currentDateTime() << "update and trade finished";
}

void MainWindow::update_infos()
{
    _show_balance->setText(QString::number(_balance_fiat) + " €\n" + QString::number(_balance_crypto) + " BTC");
    double days_since_first_trade = (QDateTime::currentSecsSinceEpoch() - _oldest)/86400.0;
    _show_trade_count->setText("Trades Count : " + QString::number(_trade_count) + "\nAvg : "
                               + QString::number(_trade_count/days_since_first_trade) + " / day");
    _show_trade_count->setAlignment(Qt::AlignCenter);
    _show_overall_gain->setAlignment(Qt::AlignCenter);
    _show_balance->setAlignment(Qt::AlignCenter);
}

void MainWindow::check_time()
{
    long curr_time = QDateTime::currentDateTime().toSecsSinceEpoch();
    if (((curr_time - 2) % 3600) == 0 || ((curr_time - 3) % 3600) == 0){
        _timer_check->stop();
        _timer_wait->start();
        qDebug() << "1H timer started";
        update_chart();
    }
}

MainWindow::~MainWindow()
{

}
