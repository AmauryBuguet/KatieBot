#include "mainwindow.hpp"

void MainWindow::trade_buy(double &price)
{
    _logs->setTextColor(Qt::black);
    QString log = "bought ETH at " + QString::number(price);
    _logs->append(QDateTime::currentDateTime().toString("d/M 'at' h:m : ") + log);
    _current_move = LONG;
    // €de btc = (_balance_fiat/1.001)
    _last_buy_volume = _balance_fiat/(1.001*price);
    _last_buy_price = price;
    _balance_crypto += _last_buy_volume;
    _balance_fiat = 0;
    _trade_count++;

    //send notification
    _pushbullet.push_note("Katie", log.toUtf8());
}

void MainWindow::trade_sell(double &price)
{
    _balance_fiat += (_balance_crypto * price)*0.999;
    double change = ((((_balance_crypto * price)*0.999)/(_last_buy_volume * 1.001 * _last_buy_price))-1)*100;
    _balance_crypto = 0;
    _trade_count++;
    QString log = "sold ETH at " + QString::number(price) + ", balance now at " + QString::number(_balance_fiat) + "$,";
    if (change > 0) {
        _logs->setTextColor(QColor(50,200,50));
        log = log + " gain of " + QString::number(change) + " %";
    }
    else  {
        _logs->setTextColor(Qt::red);
        log = log + " loss of " + QString::number(change) + " %";
    }
    _logs->append(QDateTime::currentDateTime().toString("d/M 'at' h:m : ") + log);
    _current_move = SHORT;
    double overall_gain  = ((_balance_fiat/175)-1)*100;
    _show_overall_gain->setText("Gain : " + QString::number(overall_gain) + " %");
    if (overall_gain > 0){
        _show_overall_gain->setStyleSheet("border: 1px solid; border-radius:5px; background-color: lime; ");
    }
    else  _show_overall_gain->setStyleSheet("border: 1px solid; border-radius:5px; background-color: red; ");

    //send notification
    _pushbullet.push_note("Katie", log.toUtf8());
    //TODO check account balance and send it too
}

void MainWindow::trade_hull(double &curr_price,
                            double &hma1,
                            double &hma2 )
{
    if (_current_move == SHORT){
        //buy if
        if ((hma2 > hma1) && (curr_price > hma1)){
            get_market();
            trade_buy(_ask_price);
            _current_move = LONG;
        }
    }
    else if (_current_move == LONG){
        double current_profit = curr_price/_last_buy_price;
        if (((hma1 / hma2) > 1.002) || ((hma1 > hma2) && ((hma1 / curr_price) > 1.01))){
            get_market();
            trade_sell(_bid_price);
        }
        else if (current_profit > 1.013){
            get_market();
            _logs->setTextColor(Qt::blue);
            _logs->append("sell cause more than 1.3% of profit");
            trade_sell(_bid_price);
            _current_move = STOPPED_LONGING;
        }
    }
    else if (_current_move == STOPPED_LONGING){
        if (hma1 > hma2){
            _current_move = SHORT;
        }
    }
    update_infos();
}
