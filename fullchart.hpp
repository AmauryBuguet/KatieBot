#ifndef FULLCHART_HPP
#define FULLCHART_HPP


#include <QtWidgets>
#include <QtCharts>

class FullChart : public QChartView
{
    Q_OBJECT

public:
    explicit FullChart( QWidget *parent = nullptr);
    void setSerie(long time, double price);
    void setSerie2(long time, double price);
    void setSerie3(long time, double price);
    void setSerie4(long time, double price);
    void setSerie5(long time, double price);
    void setXrange(QDateTime min = QDateTime::currentDateTime().addDays(-1),
                   QDateTime max = QDateTime::currentDateTime());
    void setYrange(double min, double max);
    QDateTimeAxis *timeAxis();
    QValueAxis *valueAxis();
    QChart *chart();

signals:
    void topChartUpdated();

private:
    QLineSeries *_serie;
    QLineSeries *_serie2;
    QLineSeries *_serie3;
    QLineSeries *_serie4;
    QLineSeries *_serie5;
    QChart *_chart;
    QDateTimeAxis *_x_axis;
    QValueAxis *_y_vaxis;
    QPen *_pen;
    double _x_pos;

protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE
    {
        if (event->button() == Qt::LeftButton)
        {
            _x_pos = event->x();
        }
    }

    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE
    {
        if (event->buttons() & Qt::LeftButton)
        {
            _chart->scroll(_x_pos - event->x(),0);
            _x_pos = event->x();
            emit topChartUpdated();
        }
    }

    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE
    {
        setXrange(timeAxis()->min().addSecs(5*event->angleDelta().y()),
                                timeAxis()->max().addSecs(-5*(event->angleDelta().y())));
        emit topChartUpdated();
        event->accept();
    }

};


#endif // FULLCHART_HPP
