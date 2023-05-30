#include "fft.h"
#include <iostream>
#include <math.h>
#include <complex.h>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <set>



fft::fft()
{
    m_dft_vector_ = nullptr;
    m_has_dft_vector_ = false;
    m_dft_vector_size_ = 0;
}

fft::~fft()
{
    if(m_has_dft_vector_ && (nullptr!=m_dft_vector_) && (m_dft_vector_size_)>0)
    {
        delete [] m_dft_vector_;

    }
}

bool fft::has_dft_vector()
{
    return m_has_dft_vector_;
}

void fft::clear_dft_vector()
{
    if (m_has_dft_vector_ && (nullptr != m_dft_vector_) && (m_dft_vector_size_ > 0)) {
        delete[] m_dft_vector_;
        m_has_dft_vector_ = false;
        m_dft_vector_size_ = 0;
    }
}

void fft::print()
{
    char msg[256] = "11111 ";

    if ((!m_has_dft_vector_) || (nullptr == m_dft_vector_) || (m_dft_vector_size_ <= 0))
        return;

    for (int i = 0; i < m_dft_vector_size_; i++) {
        sprintf(msg + 6, "%d: %lf + %lfi", i + 1, m_dft_vector_[i].m_rl, m_dft_vector_[i].m_im);

        std::cout << msg << std::endl;
        //OutputDebugStringA(msg);
    }

}

bool fft::dft(double vec[],int len)
{
    if ((len <= 0) || (nullptr == vec))
        return false;

    clear_dft_vector();

    m_dft_vector_ = new complex_number[len];
    for (int u = 0; u < len; u++) {
        m_dft_vector_[u].SetValue(0, 0);
    }
    complex_number   cplTemp(0, 0);
    double fixed_factor = (-2 * PI) / len;
    // 这是最关键的两个嵌套for循环，是离散傅里叶变换公式的代码实现，其中的运算操作和公式定义是完全一致的
    for (int u = 0; u < len; u++) {
        for (int x = 0; x < len; x++) {
            double power = u * x * fixed_factor;
            cplTemp.m_rl = vec[x] * cos(power);
            cplTemp.m_im = vec[x] * sin(power);
            m_dft_vector_[u] = m_dft_vector_[u] + cplTemp;


        }

    }

    m_has_dft_vector_ = true;
    m_dft_vector_size_ = len;


    return true;
}

//离散傅里叶变换 得到复数 根据复数计算幅值
QVector<double> fft::dft(QVector<double> vec,int len)
{
    amplitude_.clear();

    if ((len <= 0) || (vec.empty()))
        qDebug("vec empty ! \n");

    clear_dft_vector();

    m_dft_vector_ = new complex_number[len];
    for (int u = 0; u < len; u++) {
        m_dft_vector_[u].SetValue(0, 0);
    }

    complex_number   cplTemp(0, 0);
    double fixed_factor = (-2 * PI) / len;
    // 这是最关键的两个嵌套for循环，是离散傅里叶变换公式的代码实现，其中的运算操作和公式定义是完全一致的
    for (int u = 0; u < len; u++) {
        for (int x = 0; x < len; x++) {
            double power = u * x * fixed_factor;
            cplTemp.m_rl = vec[x] * cos(power);
            cplTemp.m_im = vec[x] * sin(power);
            m_dft_vector_[u] = m_dft_vector_[u] + cplTemp;
        }
    }

    m_has_dft_vector_ = true;
    m_dft_vector_size_ = len;


    phase_angle_vec_.reserve(len);
    amplitude_.reserve(len);
    for (int i = 1; i < m_dft_vector_size_/*/2*/; i++) {

        std::complex<double> c3(m_dft_vector_[i].m_rl, m_dft_vector_[i].m_im);

        auto phase_angle=/*(-1)**/((atan2( m_dft_vector_[i].m_im, m_dft_vector_[i].m_rl))*180)/PI;


        float num=(float)(fabs(c3))/(len/2);

        QString num_string=QString::number(num,'%f',4);
        num=num_string.toFloat();
        amplitude_.push_back(num);
        phase_angle_vec_.push_back(phase_angle);

//                std::ofstream ofs;
//                ofs.open("C:\\Users\\gaohuan\\Desktop\\luokuo_fuzhi.txt", std::ios::out|std::ios::app);	//打开文件

//                if (!ofs.is_open())						//判断文件是否打开
//                {
//                    qDebug() << "打开文件失败！！！";
//                }
//                QString jiaodu=QString::number(num);
//                std::string num_str=std::to_string(i)+"   "+jiaodu.toStdString()+"\n";
//                ofs.write(num_str.c_str(),num_str.size());

    }

//    calculateMax1Max2Max3(amplitude_);
    return amplitude_;
}

void fft::polarCoordinatesToCartesianCoordinates(QVector<double> displacement_of_each_point,double x,double y)
{

    cv::Point3f point_;
    QVector<cv::Point3f> polygon (7200);
    cv::Point2f point;
    QVector<cv::Point2f> points;
//    Point point_;
//    point_info_.reserve(1024);
    point_info_.clear();
//    a.clear();
    m_dft_vector_size_=displacement_of_each_point.size();

    for(int i=0;i<(m_dft_vector_size_/*/2*/)/*-1*/;i++)
    {
        auto num=float((float(i) /(float)m_dft_vector_size_) * 360.0);
        radius_x_y_.x=displacement_of_each_point[i]*cos(num);
        radius_x_y_.y=displacement_of_each_point[i]*sin(num);
        point_.x=x+displacement_of_each_point[i]*cos(/*phase_angle_vec_[i]*/num);

        point_.y=y+displacement_of_each_point[i]*sin(/*phase_angle_vec_[i]*/num);
        point.x=x+displacement_of_each_point[i]*cos(/*phase_angle_vec_[i]*/num);
        point.y=y+displacement_of_each_point[i]*sin(/*phase_angle_vec_[i]*/num);
        points.push_back(point);
        point_info_.push_back(radius_x_y_);
        polygon.push_back(point_);


//        a.push_back(point_);
    }

    leastSquaresMeanCircle(point_info_);

//     QVector<double> w(points.size(), 1);

//     cv::Point2f center_opencv;
//          double radius_opencv = 0;

//    CirleFit(points, w, center_opencv, radius_opencv);

    radius_x_y center;
    double radius;
    minCoverCircle(point_info_,point_info_.size(),center,radius);

//    maximalIncutCircle(a);
//    opencvmaxcovercirle(polygon);



}

void fft::CirleFit(const QVector<cv::Point2f>& points, const QVector<double>& weights, cv::Point2f& circleCenter, double& radius)
{
    //检查输入参数 | Check input parameters
    assert(!points.empty() && points.size() == weights.size());

    //构造矩阵 | Construct mat

    double XiSum = 0;
    double Xi2Sum = 0;
    double Xi3Sum = 0;
    double YiSum = 0;
    double Yi2Sum = 0;
    double Yi3Sum = 0;
    double XiYiSum = 0;
    double Xi2YiSum = 0;
    double XiYi2Sum = 0;
    double WiSum = 0;

    for (size_t i = 0; i < points.size(); i++)
    {
        XiSum += points.at(i).x * weights.at(i);
        Xi2Sum += points.at(i).x * points.at(i).x * weights.at(i);
        Xi3Sum += points.at(i).x * points.at(i).x * points.at(i).x * weights.at(i);
        YiSum += points.at(i).y * weights.at(i);
        Yi2Sum += points.at(i).y * points.at(i).y * weights.at(i);
        Yi3Sum += points.at(i).y * points.at(i).y * points.at(i).y * weights.at(i);
        XiYiSum += points.at(i).x * points.at(i).y * weights.at(i);
        Xi2YiSum += points.at(i).x * points.at(i).x * points.at(i).y * weights.at(i);
        XiYi2Sum += points.at(i).x * points.at(i).y * points.at(i).y * weights.at(i);
        WiSum += weights.at(i);
    }
    const int N = 3;
    cv::Mat A = cv::Mat::zeros(N, N, CV_64FC1);
    cv::Mat B = cv::Mat::zeros(N, 1, CV_64FC1);

    A.at<double>(0, 0) = Xi2Sum;
    A.at<double>(0, 1) = XiYiSum;
    A.at<double>(0, 2) = XiSum;

    A.at<double>(1, 0) = XiYiSum;
    A.at<double>(1, 1) = Yi2Sum;
    A.at<double>(1, 2) = YiSum;

    A.at<double>(2, 0) = XiSum;
    A.at<double>(2, 1) = YiSum;
    A.at<double>(2, 2) = WiSum;

    B.at<double>(0, 0) = -(Xi3Sum + XiYi2Sum);
    B.at<double>(1, 0) = -(Xi2YiSum + Yi3Sum);
    B.at<double>(2, 0) = -(Xi2Sum + Yi2Sum);

    //解矩阵 | Solve
    //求解A*X = B | Solve the A*X = B
    cv::Mat X;
    cv::solve(A, B, X, cv::DECOMP_LU);
    double a = X.at<double>(0, 0);
    double b = X.at<double>(1, 0);
    double c = X.at<double>(2, 0);

    //计算圆心和半径 | Calculate center and radius.
    circleCenter.x = -0.5 * a;
    circleCenter.y = -0.5 * b;
    radius = 0.5 * std::sqrt(a * a + b * b - 4 * c);
    qDebug()<<"最小二乘圆opencv x: "<<circleCenter.x<<"最小二乘圆y: "<< circleCenter.y<<"最小二乘圆 r: "<<radius<<"\n";


}

void fft::leastSquaresMeanCircle(QVector<radius_x_y> &points)
{
    double X1=0,Y1=0,X2=0,Y2=0,X3=0,Y3=0;
    double X1Y1=0,X1Y2=0,X2Y1=0;
    int N=points.size();

    for(int i=0;i<points.size();i++)
    {
        double x=points.at(i).x;
        double y=points.at(i).y;
        double x2=x*x,y2=y*y;
        X1+=x;
        Y1+=y;
        X2+=x2;
        Y2+=y2;
        X3+=x*x2;
        Y3+=y*y2;
        X1Y1+=x*y;
        X1Y2+=x*y2;
        X2Y1+=x2*y;
    }

    double a=0,b=0,c=0;
    double C1=0,D1=0,E1=0,G1=0,H1=0;

    C1=N*X2-X1*X1;
    D1=N*X1Y1-X1*Y1;
    E1=N*X3+N*X1Y2-(X2+Y2)*X1;
    G1=N*Y2-Y1*Y1;
    H1=N*X2Y1+N*Y3-(X2+Y2)*Y1;

    a=(H1*D1-E1*G1)/(C1*G1-D1*D1);
    b=(H1*C1-E1*D1)/(D1*D1-G1*C1);
    c=-(a*X1+b*Y1+X2+Y2)/N;

    tempPoint_.x=a/(-2);
    tempPoint_.y=b/(-2);
    tempPoint_.radius=(sqrt(a*a+b*b-4*c))/2;

    qDebug()<<"最小二乘圆 x: "<<tempPoint_.x<<"最小二乘圆y: "<<tempPoint_.y<<"最小二乘圆 r: "<<tempPoint_.radius<<"\n";
    return /*true*/;
}

/*************************************************
Function: get_sign
Description: 求实数x的符号
Input: 实数x
Output: 无
Return: 实数x的符号(0, -1, 1)
*************************************************/
int fft::getSingle(double x)
{
    if (fabs(x) < 1.0e-8)
    {
        return 0;
    }
    else
    {
        return (x < 0) ? -1 : 1;
    }
}

/*************************************************
Function: get_distance_square
Description: 求平面两点之间距离的平方
Input: 平面两点a，b
Output: 无
Return: 平面两点之间距离的平方
*************************************************/
double fft::getDistanceSquare(const radius_x_y a, const radius_x_y b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

/*************************************************
Function: get_circle_center
Description: 求三角形外接圆的圆心
Input: 平面三个点a，b，c
Output: 无
Return: 三角形外接圆的圆心center
*************************************************/
radius_x_y fft::getCircleCenter(const radius_x_y a, const radius_x_y b, const radius_x_y c)
{
    radius_x_y center;
    double a1 = b.x - a.x;
    double b1 = b.y - a.y;
    double c1 = 0.5 * (a1 * a1 + b1 * b1);
    double a2 = c.x - a.x;
    double b2 = c.y - a.y;
    double c2 = 0.5 * (a2 * a2 + b2 * b2);
    double d = a1 * b2 - a2 * b1;

    center.x = a.x + (c1 * b2 - c2 * b1) / d;
    center.y = a.y + (a1 * c2 - a2 * c1) / d;
    return center;
}

/*************************************************
Function: min_cover_circle
Description: 求平面pointCount个点的最小覆盖圆
Input: 平面pointCount个点的数组首元素地址p，点个数pointCount
Output: 平面pointCount个点的最小覆盖圆圆心center，半径radius
Return: 无
*************************************************/
void fft::minCoverCircle(QVector<radius_x_y> p, int pointCount, radius_x_y &center, double &radius)//找最小覆盖圆(这里没有用全局变量p[], 因为是为了封装一个函数便于调用)
{
    double radiusSquare;

    //    random_shuffle(p, p + pointCount);//随机函数,使用了之后使程序更快点,也可以不用

    center = p[0];
    radiusSquare = 0.0;
    for (int i = 1; i < pointCount; i++)
    {
        if (getSingle(getDistanceSquare(p[i], center) - radiusSquare) > 0)//如果p[i]在当前圆的外面, 那么以当前点为圆心开始找
        {
            center = p[i];//圆心为当前点
            radiusSquare = 0.0;//这时候这个圆只包括他自己.所以半径为0
            for (int j = 0; j < i; j++)//找它之前的所有点
            {
                if (getSingle(getDistanceSquare(p[j], center) - radiusSquare) > 0)//如果之前的点有不满足的, 那么就是以这两点为直径的圆
                {
                    center.x = 0.5 * (p[i].x + p[j].x);
                    center.y = 0.5 * (p[i].y + p[j].y);
                    radiusSquare = getDistanceSquare(p[j], center);
                    for (int k = 0; k < j; k++)
                    {
                        if (getSingle(getDistanceSquare(p[k], center) - radiusSquare) > 0)//找新作出来的圆之前的点是否还有不满足的, 如果不满足一定就是三个点都在圆上了
                        {
                            center = getCircleCenter(p[i], p[j], p[k]);
                            radiusSquare = getDistanceSquare(p[i], center);
                        }
                    }
                }
            }
        }
    }
    radius = sqrt(radiusSquare);

    min_coverage_circle_.radius=radius;
    min_coverage_circle_.x=center.x;
    min_coverage_circle_.y=center.y;

    qDebug()<<"最小外切圆 x :"<<center.x<<"最小外切圆  y:"<<center.y<< "最小外切圆  r:"<<radius<<"\n";
}

void fft::maximalIncutCircle(QVector<Point> ak)
{
//    a.clear();

//    Point x_y;
//    x_y.x = 0; x_y.y = 0;
//    a.push_back(x_y);
//    x_y.x = 10000; x_y.y = 0;
//    a.push_back(x_y);
//    x_y.x = 7000; x_y.y = 1000;
//    a.push_back(x_y);

    n = a.size();
    L.resize(n);
    for (int i = 0; i < n; i++) {
        L[i] = Line(a[i], a[(i + 1) % n]);
    }//n - 1条线
    Ef();

}

Point fft::Ist_LP(Line a, double a2, double b2, double c2) {//给两直线求交点
    double a1 = a.e.y - a.s.y, b1 = a.s.x - a.e.x, c1 = a.e.x * a.s.y - a.s.x * a.e.y;
        //	double a2 = b.s.y - b.e.y,b2 = b.e.x - b.s.x,c2 = b.s.x * b.e.y - b.e.x * b.s.y;
        return Point((c1 * b2 - c2 * b1) / (a2 * b1 - a1 * b2), (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1));
}

NODE fft::get_abc(Line X) {
    NODE cur;
    Point S = X.s, E = X.e;
    cur.a = E.y - S.y;//后y - 前y
    cur.b = S.x - E.x;//前x - 后x
    cur.c = S.y * E.x - S.x * E.y;//前y*后x - 前x*后y
    return cur;
}

void fft::cut(double a, double b, double c) {
    int cnt = 0;//这把cut完的凸核还剩几个点
        C.resize(MaxN);
        for (int i = 1; i <= M; i++) {
            if (a * p[i].x + b * p[i].y + c <= 0) {
                C[++cnt] = p[i];//这次cur产生的点数组
            }//<=0:p[i]点在平移后的直线的左边 √
            else {
                //当前点与前后临接点 与平移后的直线相交
                //这里不需要= 都=了还求个锤子交点那不是直接放进去了
                if (a * p[i - 1].x + b * p[i - 1].y + c < 0) {

                    Line a_line = { p[i - 1], p[i] };
                    C[++cnt] = Ist_LP(a_line, a, b, c);
                }
                if (a * p[i + 1].x + b * p[i + 1].y + c < 0) {
                    Line b_line = { p[i], p[i + 1] };
                    C[++cnt] = Ist_LP(b_line, a, b, c);//那得给整个点吧
                }
            }
        }
        for (int i = 1; i <= cnt; i++) p[i] = C[i];
        p[0] = C[cnt];
        p[cnt + 1] = C[1];//p[]存的是上一次cut完剩下的点 环数组
        M = cnt;
}

bool fft::HalfPlane(double D) {//半平面交
    //本题这里已经是排好序的 明确给出逆时针凸多边形
    //L 0~n-1  a[]0~n-1
    p.resize(MaxN);
    for (int i = 0; i < n; i++)
    {
        p[i + 1] = a[i];//方便一会儿cut
    }
    p[n + 1] = a[0];
    p[0] = a[n - 1];
    M = n;
    for (int i = 0; i < n; i++) {
        NODE cur = get_abc(L[i]);
        cur.c += D * sqrt(cur.a * cur.a + cur.b * cur.b);//直线的平移
        //		cout << i+1 <<":"<<cur.a<<" " <<cur.b<<" "<<cur.c<<endl;
        cut(cur.a, cur.b, cur.c/*,p,C*/);
        if (!M) break;
    }
    //	printf("D:%.6f  M:%d\n",D,M);
    //	cout <<"------------------------------\n";
    return M > 0;//组成凸核的点的个数
 }

void fft::Ef() {//二分找最大内接圆半径
    double l = 0.0, r = 4000.0, mid = 0.0;
    while (r - l >= eps) {
        mid = (l + r) / 2.0;
        if (HalfPlane(mid)) l = mid;
        else r = mid;
    }
    qDebug()<<"zuixiaobanjing :::::::::::"<< mid<<"\n";
}

//opencv max内切圆
//int fft::get_line_intersection(cv::Point3f& p0, cv::Point3f& p1, cv::Point3f& p2, cv::Point3f& p3)
//{

//    float  s_numer, t_numer, denom, t;
//    cv::Point3f s10, s32;
//    s10 = p1 - p0;
//    s32 = p3 - p2;


//    denom = s10.x * s32.y - s32.x * s10.y;
//    if (denom == 0)//平行或共线
//        return 0; // Collinear
//    bool denomPositive = denom > 0;

//    cv::Point3f s02;
//    s02 = p0 - p2;

//    s_numer = s10.x * s02.y - s10.y * s02.x;
//    if ((s_numer < 0) == denomPositive)//参数是大于等于0且小于等于1的，分子分母必须同号且分子小于等于分母
//        return 0; // No collision


//    t_numer = s32.x * s02.y - s32.y * s02.x;
//    if ((t_numer < 0) == denomPositive)
//        return 0; // No collision

//    if (fabs(s_numer) > fabs(denom) || fabs(t_numer) > fabs(denom))
//        return 0; // No collision


//    return 1;
//}


//bool fft::IsPointInPolygon(QVector<cv::Point3f>& polyline, cv::Point3f& pt)
//{
//    int count = 0;
//    cv::Point3f mark1(1000, 1000, 0);
//    for (int i = 0; i < polyline.size(); i++)
//    {
//        int id = (i + 1) % polyline.size();
//        if (get_line_intersection(polyline[i], polyline[id], pt, mark1) == 1)
//        {
//            count++;
//        }

//    }
//    if (count % 2 == 1)
//        return true;
//    else
//        return false;

//}

//double fft::DistancePointAPolygon(QVector<cv::Point3f>& polyline, cv::Point3f& pt)
//{
//    ///1 先判断垂足位置
//    cv::Point3f mark;
//    double val, val1, val2, val3;
//    double max_dist = 10000;
//    for (int i = 0; i < polyline.size(); i++)
//    {
//        int id = (i + 1) % polyline.size();

//        cv::Point3f diff = polyline[id] - polyline[i];  ///  -b a

//        mark.x = -diff.y * 10000 + pt.x;
//        mark.y = diff.x * 10000 + pt.y;
//        double dist = 0;

//        if (get_line_intersection(polyline[i], polyline[id], pt, mark) == 1)  // 有交点
//        {
//            cv::Point3f pt_edge = pt - polyline[i];
//            val = pt_edge.x * diff.x + pt_edge.y * diff.y;
//            val1 = pt_edge.x * pt_edge.x + pt_edge.y * pt_edge.y;
//            if (val1 < 0.00001)
//                dist = 0;
//            else
//            {
//                val1 = sqrt(val1);
//                val2 = diff.x * diff.x + diff.y * diff.y;
//                val2 = sqrt(val2);
//                double temp = val / (val1 * val2);
//                if (temp > 1) temp = 1.0;
//                if (temp < -1) temp = -1.0;

//                dist = val1 * sin(acos(temp));
//                // double tmep_ = val / (val2*val1);
//                 // dist = sqrt(1 - temp*temp)*val1;

//            }


//        }
//        else
//        {
//            cv::Point3f dist1 = pt - polyline[i];
//            cv::Point3f dist2 = pt - polyline[id];
//            val1 = dist1.x * dist1.x + dist1.y * dist1.y;
//            val2 = dist2.x * dist2.x + dist2.y * dist2.y;
//            if (val1 > val2)
//            {
//                val2 = sqrt(val2);
//                dist = val2;
//            }
//            else
//            {
//                val1 = sqrt(val1);
//                dist = val1;
//            }

//        }
//        if (dist < max_dist)
//            max_dist = dist;


//    }
//    return max_dist;
//}


//cv::Point3f fft::GeometryFindPIA(QVector<cv::Point3f> polygon, float bounds[])
//{
//    cv::Point3f pia;

//    pia.x = (bounds[0] + bounds[1]) / 2;
//    pia.y = (bounds[2] + bounds[3]) / 2;
//    cv::Point3f tmp;


//    float increment_x = (bounds[1] - bounds[0]) / N_CELLS;
//    float increment_y = (bounds[3] - bounds[2]) / M_CELLS;


//    float max_distance = 0;


//    int i, j;
//    float tmp_distance = FLT_MAX;
//    for (i = 0; i <= N_CELLS; i++)
//    {

//        tmp.x = bounds[0] + i * increment_x;

//        for (j = 0; j <= M_CELLS; j++)
//        {
//            tmp.y = bounds[2] + j * increment_y;


//            // compare with candidate PIA if point is in polygon
//            if (IsPointInPolygon(polygon, tmp))
//            {
//                tmp_distance = DistancePointAPolygon(polygon, tmp);

//                if (tmp_distance > max_distance)
//                {
//                    max_distance = tmp_distance;
//                    pia.x = tmp.x;
//                    pia.y = tmp.y;
//                }
//            }
//        }
//    }



//    return pia;
//}


//int  fft::FindInscribedCircleCenter(QVector<cv::Point3f> polygon, double* output)
//{

//    if (polygon.size() <= 1)
//        return 0;
//    float bounds[4] = { 10000,-10000,10000,-10000 };
//    for (int i = 0; i < polygon.size(); i++)
//    {
//        cv::Point3f pt = polygon[i];
//        if (pt.x < bounds[0]) bounds[0] = pt.x;
//        if (pt.x > bounds[1]) bounds[1] = pt.x;
//        if (pt.y < bounds[2]) bounds[2] = pt.y;
//        if (pt.y > bounds[3]) bounds[3] = pt.y;
//    }

//    cv::Point3f point_pia;

//    float flt_tmp = FLT_MAX;
//    int count = 1;
//    cv::Point3f point_tmp;// = new cv::Point3f;
//    while (count++)
//    {

//        // find new candidate PIA

//        point_tmp = GeometryFindPIA(polygon, bounds);


//        // update current PIA
//        point_pia.x = point_tmp.x;
//        point_pia.y = point_tmp.y;

//        // update the bounds
//        flt_tmp = (bounds[1] - bounds[0]) / (sqrtf(2) * 2);
//        bounds[0] = point_pia.x - flt_tmp;
//        bounds[1] = point_pia.x + flt_tmp;
//        flt_tmp = (bounds[3] - bounds[2]) / (sqrtf(2) * 2);
//        bounds[2] = point_pia.y - flt_tmp;
//        bounds[3] = point_pia.y + flt_tmp;


//        if (bounds[1] - bounds[0] < 0.001 || bounds[3] - bounds[2] < 0.001) break;

//        //	printf("Candidate PIA: (%f,%f)\n", point_pia.x, point_pia.y);
//    }


//    double tmp_distance = DistancePointAPolygon(polygon, point_pia);
//    output[0] = point_pia.x;
//    output[1] = point_pia.y;
//    output[2] = polygon[0].z;
//    output[3] = tmp_distance;
//    return 1;

//}

//void fft::opencvmaxcovercirle(QVector<cv::Point3f> polygon)
//{


//    double output[4] = {1,2,3,4};


//    FindInscribedCircleCenter(polygon, output);
//    incircle_center_x=output[0];
//    incircle_center_y=output[1];
//    incircle_radius=output[3];

//    qDebug()<<"最大内切圆 x:"<<incircle_center_x<<"最大内切圆 y:"<<incircle_center_y<<"最大内切圆 r:"<<incircle_radius<<"\n";

//}

//幅值
void fft::calculateMax1Max2Max3(QVector<double>& nums) {

    std::sort(nums.begin(),nums.end());     //去重前需要排序
    auto it= std::unique(nums.begin(),nums.end());   //去除容器内重复元素
    nums.erase(it,nums.end());
    for(int i=0;i<nums.size();i++)
    {
        if(nums.size()-3==i)
        {
            max3_=nums.at(i);
        }
        if(nums.size()-2==i)
        {
            max2_=nums.at(i);
        }
        if(nums.size()-1==i)
        {
            max1_=nums.at(i);
        }

    }

    return ;
}

double fft::gaussian(double x, double sigma) {
    return exp(-x * x / (2 * sigma * sigma)) / sqrt(2 * M_PI) / sigma;
}

QVector<double> fft::gaussian_filter(QVector<double>& data, double sigma) {
    QVector<double> filter;
    int size = 2 * ceil(3 * sigma) + 1;
    filter.resize(size);
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        filter[i] = gaussian(i - size / 2, sigma);
        sum += filter[i];
    }
    for (int i = 0; i < size; i++) {
        filter[i] /= sum;
    }
    QVector<double> result;
    int n = data.size();
    result.resize(n);
    for (int i = 0; i < n; i++) {
        double value = 0.0;
        for (int j = 0; j < size; j++) {
            int k = i + j - size / 2;
            if (k >= 0 && k < n) {
                value += data[k] * filter[j];
            }
        }
        result[i] = value;
    }
    return result;
}

QVector<double> fft::filterBand(int begin,int end)
{
    QVector<double> vec_data;

    for(int i=begin;i<end;i++)
    {
        vec_data.push_back(filter_band_[i]);
    }

    double sigma=1;
    return gaussian_filter(vec_data,sigma);
}

void fft::gaussianBand(QVector<double>& data)
{
    filter_band_=data;

}
