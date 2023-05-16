#ifndef GET_POLAR_DATA_H
#define GET_POLAR_DATA_H

#include <qwt_polar_plot.h>

#include <qwt_polar_grid.h>

#include <qwt_polar_curve.h>
#include <qwt_series_data.h>


class GetPolarData: public QwtSeriesData< QwtPointPolar >
{

    virtual size_t size() const
    {
        return get_polar_data.size();
    }

    virtual QwtPointPolar sample(size_t i) const
    {

        auto value_float=float(get_polar_data.size());
        double theta = i /value_float * 360.0 ;

        double r=get_polar_data[i];


        return QwtPointPolar(theta, r);

    }

    virtual QRectF boundingRect() const
    {

        //        return QRectF(-2.0, -2.0, 4, 4);
        if ( d_boundingRect.width() < 0.0 )
            d_boundingRect = qwtBoundingRect( *this );

        return d_boundingRect;
    }

public:
    void get_data(QVector<double> num)
    {
        get_polar_data=num;
    }


    QVector<double> get_polar_data;
};

#endif // GET_POLAR_DATA_H
