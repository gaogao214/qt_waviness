#include "histogram.h"
#include "ui_histogram.h"

#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_barchart.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>

class DistroChartItem: public QwtPlotBarChart
{
public:
    void addDistro( const QString &distro, const QColor &color )
    {
        d_colors += color;
        d_distros += distro;
        itemChanged();
    }

    virtual QwtColumnSymbol *specialSymbol(
            int index, const QPointF& ) const
    {
        // we want to have individual colors for each bar

        QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
        symbol->setLineWidth( 2 );
        symbol->setFrameStyle( QwtColumnSymbol::Raised );

        QColor c( Qt::white );
        if ( index >= 0 && index < d_colors.size() )
            c = d_colors[ index ];

        symbol->setPalette( c );

        return symbol;
    }

private:
    QList<QColor> d_colors;
    QList<QString> d_distros;
};



histogram::histogram(QWidget *parent) :
    ui(new Ui::histogram)
{
    ui->setupUi(this);

    setAxisScale(QwtPlot::xBottom, 0, 2048);
    setAxisMaxMajor(QwtPlot::xBottom, 30);
    setAxisMaxMinor(QwtPlot::xBottom, 0);
    setAxisScale(QwtPlot::yLeft, 0, 1);
    //    d_chart->setAxisMaxMajor(QwtPlot::yLeft, 6);
    //    d_chart->setAxisMaxMinor(QwtPlot::yLeft, 2);

    QFont fontX;
    fontX.setFamily(QStringLiteral("微软雅黑"));
    fontX.setPointSize(5);
    setAxisFont(QwtPlot::xBottom, fontX);
    setTitle("谐波");

//    QwtPlotGrid *grid = new QwtPlotGrid;
//    grid->setMajorPen(QColor(193, 193, 193), 1, Qt::DashLine);
//    grid->attach(histogram_chart_.get());

}

histogram::~histogram()
{
    delete ui;
}

void histogram::draw_histogram(QVector<double> data)
{

    DistroChartItem* d_barChartItem = new DistroChartItem();

    QVector<double> vec_sample_;

    for ( int i = 1; i < data.size(); i++ )
    {
        vec_sample_.push_back(data[i]);

        d_barChartItem->addDistro(
                    QString::number(i), QColor( "DodgerBlue" ) );
    }

    d_barChartItem->setSamples( vec_sample_ );

    d_barChartItem->attach(this);
}

void histogram::waviness(QVector<double> data,int rotate_speed)
{

    DistroChartItem* d_barChartItem = new DistroChartItem();

    QVector<double> vec_sample_;

    for ( int i = 2; i < data.size(); i++ )
    {
        vec_sample_.append(1.4142 * 3.1416 * (rotate_speed/60) * data[i]*i);  //根号2 * pi * 转速/60 *谐波*波次
        d_barChartItem->addDistro(
                    QString::number(i), QColor( "DodgerBlue" ) );
    }

    d_barChartItem->setSamples( vec_sample_ );

    d_barChartItem->attach(this);
}
