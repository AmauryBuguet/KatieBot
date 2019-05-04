#include "fullchart.hpp"
#include "mainwindow.hpp"

using namespace std;

FullChart::FullChart(QWidget *parent) :
    QChartView(parent)
{
    setMouseTracking(true);

    _serie = new QLineSeries();
    _serie2 = new QLineSeries();
    _serie3 = new QLineSeries();
    _serie4 = new QLineSeries();
    _serie5 = new QLineSeries();

    _pen = new QPen(Qt::blue);
    _pen->setWidth(2);
    _serie->setPen(*_pen);
    _pen->setColor(Qt::red);
    _serie2->setPen(*_pen);
    _serie3->setPen(*_pen);
    _pen->setColor(Qt::green);
    _serie4->setPen(*_pen);
    _serie5->setPen(*_pen);

    _chart = new QChart();
    _chart->legend()->hide();
    _chart->addSeries(_serie);
    _chart->addSeries(_serie2);
    _chart->addSeries(_serie3);
    _chart->addSeries(_serie4);
    _chart->addSeries(_serie5);
    _chart->layout()->setContentsMargins(0, 0, 0, 0);

    _x_axis = new QDateTimeAxis();
    _x_axis->setFormat("d/M h:m");
    _x_axis->setTitleText("Time");
    _x_axis->setTickCount(7);

    _y_vaxis = new QValueAxis();
    _y_vaxis->setLabelFormat("%.1f");
    _y_vaxis->setTitleText("Price (€)");
    _y_vaxis->setTickCount(6);

    _chart->addAxis(_x_axis, Qt::AlignBottom);
    _chart->addAxis(_y_vaxis, Qt::AlignLeft);

    _serie->attachAxis(_x_axis);
    _serie2->attachAxis(_x_axis);
    _serie3->attachAxis(_x_axis);
    _serie4->attachAxis(_x_axis);
    _serie5->attachAxis(_x_axis);

    _serie->attachAxis(_y_vaxis);
    _serie2->attachAxis(_y_vaxis);
    _serie3->attachAxis(_y_vaxis);
    _serie4->attachAxis(_y_vaxis);
    _serie5->attachAxis(_y_vaxis);


    _chart->setTitle("BTC-EUR Price Chart");


    setRenderHint(QPainter::Antialiasing);
    setMinimumSize(700,320);
    setChart(_chart);

}

void FullChart::setSerie(long time, double price)
{
    _serie->append(time, price);
}

void FullChart::setSerie2(long time, double price)
{
    _serie2->append(time, price);
}

void FullChart::setSerie3(long time, double price)
{
    _serie3->append(time, price);
}

void FullChart::setSerie4(long time, double price)
{
    _serie4->append(time, price);
}

void FullChart::setSerie5(long time, double price)
{
    _serie5->append(time, price);
}

void FullChart::setXrange(QDateTime min, QDateTime max)
{
    _x_axis->setRange(min, max);
}

void FullChart::setYrange(double min, double max)
{
    _y_vaxis->setRange(min, max);
}

QChart *FullChart::chart()
{
    return _chart;
}

QDateTimeAxis *FullChart::timeAxis()
{
    return _x_axis;
}

QValueAxis *FullChart::valueAxis()
{
    return _y_vaxis;
}
