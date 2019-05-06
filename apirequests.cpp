#include "mainwindow.hpp"
#include "binacpp.h"
#include "binacpp_websocket.h"
#include <json/json.h>

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
    Json::Value balances;
    long recvWindow = 10000;

    BinaCPP::get_account(recvWindow, result);
    balances = result["balances"];

    for ( int i = 0 ; i < balances.size() ; i++ ) {
        if (QString::fromStdString(balances[i]["asset"].asString().c_str()) == "ETH"){
            _balance_crypto = QString::fromStdString(balances[i]["free"].asString().c_str()).toDouble();
            qDebug() << "balance eth :" << _balance_crypto;
        }
        if (QString::fromStdString(balances[i]["asset"].asString().c_str()) == "USDT"){
            _balance_fiat = QString::fromStdString(balances[i]["free"].asString().c_str()).toDouble();
            qDebug() << "balance usdt :" << _balance_fiat;
        }
    }
}

void MainWindow::send_buy_order()
{

    BinaCPP::init(_api_key , _secret_key);

    Json::Value result;
    long recvWindow = 10000;

    //determine buy volume
    double buy_volume = (_balance_fiat * 0.99) / (1.001*_ask_price);

    BinaCPP::send_order("ETHUSDT", "BUY", "MARKET", "GTC", buy_volume , 0, "", 0, 0, recvWindow, result );
    //                   pair ,buy/sell, market/limit, gtc, qty, price, ....)
    cout << result << endl;

    //check if order is filled
    if ((QString::fromStdString(result["status"].asString().c_str())) == "FILLED"){
        _last_buy_price = QString::fromStdString(result["fills"][0]["price"].asString().c_str()).toDouble();
        qDebug() << "buy order successful";
    }
    //else check_order(QString::fromStdString(result["orderId"].asString().c_str()).toLong());

    _last_buy_volume = buy_volume;
}

void MainWindow::send_sell_order()
{

    BinaCPP::init(_api_key , _secret_key);

    Json::Value result;
    long recvWindow = 10000;

    double sell_volume = _balance_crypto;

    BinaCPP::send_order("ETHUSDT", "SELL", "MARKET", "GTC", sell_volume , 0, "", 0, 0, recvWindow, result );
    cout << result << endl;

    //check if order is filled
    if ((QString::fromStdString(result["status"].asString().c_str())) == "FILLED"){
        _last_sell_price = QString::fromStdString(result["fills"][0]["price"].asString().c_str()).toDouble();
        qDebug() << "sell order successful";
        qDebug() << "sell price" << _last_sell_price;
    }
    //else check_order(QString::fromStdString(result["orderId"].asString().c_str()).toLong());

    _last_sell_volume = sell_volume;
}

void MainWindow::check_order(long id)
{

    Json::Value result;
    long recvWindow = 10000;

    BinaCPP::get_order( "ETHUSDT", id, "", recvWindow, result );
    cout << result << endl;

    //check if order is filled
    if (QString::fromStdString(result["status"].asString().c_str()) == "FILLED"){
        if (QString::fromStdString(result["side"].asString().c_str()) == "BUY"){
            qDebug() << "buy order" << id << "successful";
            // price is always 0 here
            _last_buy_price = _ask_price;
            //_last_buy_price = QString::fromStdString(result["price"].asString().c_str()).toDouble();
        }
        else {
            qDebug() << "sell order" << id << "successful";
            _last_sell_price = _bid_price;
            // price is always 0 here
            //_last_sell_price = QString::fromStdString(result["price"].asString().c_str()).toDouble();
        }
    }
    else {
        qDebug() << "order still not filled";
        sleep(3);
        check_order(id);
    }
}
