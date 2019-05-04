#include "mainwindow.hpp"
#include "binacpp.h"
#include "binacpp_websocket.h"
#include <json/json.h>
#include <unistd.h>

using namespace std;

void MainWindow::get_klines()
{

    BinaCPP::init( _api_key , _secret_key );

    Json::Value result;
    vector<Kline> buffer;

    BinaCPP::get_klines( _pair, _interval, 787 , 0, 0, result );
    for ( int i = 0 ; i < result.size() ; i++ ) {
        Kline kline;
        kline.time = result[i][0].asInt64();
        kline.openprice = QString::fromStdString(result[i][1].asString().c_str()).toDouble();
        kline.highprice = QString::fromStdString(result[i][2].asString().c_str()).toDouble();
        kline.lowprice = QString::fromStdString(result[i][3].asString().c_str()).toDouble();
        kline.closeprice = QString::fromStdString(result[i][4].asString().c_str()).toDouble();
        buffer.push_back(kline);
    }
    if (_latest_finished_period == buffer[buffer.size() - 2].time){
        qDebug() << QDateTime::currentDateTime() << "no new data, retrying in 1s";
        sleep(1);
        get_klines();
    }
    else _klines.swap(buffer);
}

void MainWindow::get_market()
{

    BinaCPP::init( _api_key , _secret_key );

    Json::Value result;

    BinaCPP::get_bookTicker(_pair, result );
    _ask_price = QString::fromStdString(result["askPrice"].asString().c_str()).toDouble();
    _bid_price = QString::fromStdString(result["bidPrice"].asString().c_str()).toDouble();
    cout << result;
}

void MainWindow::get_account()
{

    BinaCPP::init( _api_key , _secret_key );

    Json::Value result;
    long recvWindow = 10000;

    BinaCPP::get_account(recvWindow, result);
    //_balance_fiat = QString::fromStdString(result["balances"]["USDT"].asString().c_str()).toDouble();
    //_balance_crypto = QString::fromStdString(result["balances"]["ETH"].asString().c_str()).toDouble();
    cout << result;
}

void MainWindow::send_buy_order()
{

    //BinaCPP::init(_api_key , _secret_key);

    Json::Value result;
    long recvWindow = 10000;

    BinaCPP::send_order("ETHUSDT", "BUY", "MARKET", "GTC", 20 , 0, "", 0, 0, recvWindow, result );
    cout << result << endl;
}
