#include "mainwindow.hpp"

void MainWindow::trade_buy()
{
    // send buy request
    send_buy_order();

    //update account balances
    sleep(5);
    get_account();

    //update display
    _logs->setTextColor(Qt::black);
    QString log = "bought " + QString::number(_last_buy_volume) + " ETH at " + QString::number(_last_buy_price) + "$";
    _logs->append(QDateTime::currentDateTime().toString("d/M 'at' h:m:s : ") + log);

    // update variables
    _current_move = LONG;
    _trade_count++;

    //send notification
    _pushbullet.push_note("Katie", log.toUtf8());

}

void MainWindow::trade_sell()
{
    // send sell request
    send_sell_order();

    //update account balances
    sleep(5);
    get_account();

    // update display
    QString log = "sold " + QString::number(_last_sell_volume) + " ETH at " + QString::number(_last_sell_price)
        + "$, balance now at " + QString::number(_balance_fiat) + "$,";
    double change = _last_sell_price/_last_buy_price;
    if (change > 0) {
        _logs->setTextColor(QColor(50,200,50));
        log = log + " gain of " + QString::number(change) + " %";
    }
    else  {
        _logs->setTextColor(Qt::red);
        log = log + " loss of " + QString::number(change) + " %";
    }
    _logs->append(QDateTime::currentDateTime().toString("d/M 'at' h:m : ") + log);

    double overall_gain  = ((_balance_fiat/_initial_balance)-1)*100;
    _show_overall_gain->setText("Gain : " + QString::number(overall_gain) + " %");
    if (overall_gain > 0){
        _show_overall_gain->setStyleSheet("border: 1px solid; border-radius:5px; background-color: lime; ");
    }
    else  _show_overall_gain->setStyleSheet("border: 1px solid; border-radius:5px; background-color: red; ");

    // update variables
    _current_move = SHORT;
    _trade_count++;

    //send notification
    _pushbullet.push_note("Katie", log.toUtf8());

}

void MainWindow::trade_hull(double &curr_price,
                            double &hma1,
                            double &hma2 )
{
    if (_current_move == SHORT){
        if ((hma2 > hma1) && (curr_price > hma1)){
            get_market();
            trade_buy();
            _current_move = LONG;
        }
    }
    else if (_current_move == LONG){
        double current_profit = curr_price/_last_buy_price;
        if (((hma1 / hma2) > 1.002) || ((hma1 > hma2) && ((hma1 / curr_price) > 1.01))){
            get_market();
            trade_sell();
        }
        else if (current_profit > 1.013){
            get_market();
            _logs->setTextColor(Qt::blue);
            _logs->append("sell cause more than 1.3% of profit");
            trade_sell();
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
