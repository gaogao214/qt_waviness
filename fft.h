#ifndef FFT_H
#define FFT_H

#include "complex_number.h"
#include <complex>
#include <opencv2/opencv.hpp>

#define      MAX_VECTOR_SIZE          2048                       // 原始信号最大允许长度
#define      PI                       3.141592653         // 圆周率π的近似值
#define N_CELLS 20
#define M_CELLS 20
struct radius_x_y
{
    double radius=0;
    double x=0;
    double y=0;
};

#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cmath>
#include <vector>


//using namespace std;
constexpr double eps = 1e-6;
const double pi = acos(-1.0);
constexpr int inf = 0x3f3f3f3f;
constexpr int MaxN = 4096;


struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};
typedef Point Vector;

struct Line {
    Point s, e;
    Line() {}
    Line(Point a, Point b) { s = a, e = b; }
};

struct NODE {
    double a, b, c;
};

class fft
{
public:
    fft();
    ~fft();

public:
    bool dft(double vec[], int len);                // 一维离散傅里叶变换

    QVector<double> dft(QVector<double> vec,int len);
    //bool idft(LPVOID OUT* pVec, int OUT* ilen);                    // 一维离散傅里叶逆变换

    bool has_dft_vector();                                                         // 是否已存有变换结果

    void clear_dft_vector();                                                       // 清除已有的变换结果
    void print();                                                                           // 打印变换结果

    void CirleFit(const QVector<cv::Point2f>& points, const QVector<double>& weights, cv::Point2f& circleCenter, double& radius);

    void leastSquaresMeanCircle(QVector<radius_x_y> &points);           //最小二乘拟合圆

    void polarCoordinatesToCartesianCoordinates(QVector<double> displacement_of_each_point,double x,double y);  //极坐标转笛卡尔坐标

    int getSingle(double x);

    double getDistanceSquare(const radius_x_y a, const radius_x_y b);

    radius_x_y getCircleCenter(const radius_x_y a, const radius_x_y b, const radius_x_y c);

    void minCoverCircle(QVector<radius_x_y> p, int pointCount, radius_x_y& center, double& radius);//找最小覆盖圆(这里没有用全局变量p[], 因为是为了封装一个函数便于调用)

    //opencv max 最大内切圆
    int get_line_intersection(cv::Point3f& p0, cv::Point3f& p1, cv::Point3f& p2, cv::Point3f& p3);

    bool IsPointInPolygon(QVector<cv::Point3f>& polyline, cv::Point3f& pt);

    double DistancePointAPolygon(QVector<cv::Point3f>& polyline, cv::Point3f& pt);

    cv::Point3f GeometryFindPIA(QVector<cv::Point3f> polygon, float bounds[]);

    int  FindInscribedCircleCenter(QVector<cv::Point3f> polygon, double* output);

    void opencvmaxcovercirle(QVector<cv::Point3f> polygon);


    void calculateMax1Max2Max3(QVector<double>& nums);

    void maximalIncutCircle(QVector<Point> vec_p);

    double gaussian(double x, double sigma);

    QVector<double> gaussian_filter(QVector<double>& data, double sigma);

    QVector<double> filterBand(int begin,int end);

    void gaussianBand(QVector<double>& data);

    static int dcmp(double x)
    {
        if (fabs(x) < eps) return 0;
            else return x < 0 ? -1 : 1;
    }

    //运算符重载
    friend Vector operator + (Vector A, Vector B) { return Vector(A.x + B.x, A.y + B.y); }
    friend Vector operator - (Vector A, Vector B) { return Vector(A.x - B.x, A.y - B.y); }
    friend Vector operator * (Vector A, double p) { return Vector(A.x * p, A.y * p); }
    friend bool operator == (const Point& a, const Point& b)  {
        return dcmp(a.x - b.x) == 0 && dcmp(a.y - b.y) == 0;
    }

    double Dot(Vector A, Vector B) { return A.x * B.x + A.y * B.y; }
    double Cross(Vector A, Vector B) { return A.x * B.y - A.y * B.x; }
    Point Ist_LP(Line a, double a2, double b2, double c2);
    NODE get_abc(Line X) ;
    void cut(double a, double b, double c);
    bool HalfPlane(double D);
    void Ef() ;


    void pointPolyTest1(QVector<cv::Point2f> data);


public:
    complex_number* m_dft_vector_;                                     // 保存变换结果的容器

private:
    bool      m_has_dft_vector_;
    int       m_dft_vector_size_;                                                 // 变换结果的长度


public:
    QVector<double> amplitude_;
    double max1_,max2_,max3_;


    QVector<double> phase_angle_vec_;
    double center_x_;
    double center_y_;
    double radius_;

    radius_x_y radius_x_y_;
    QVector<radius_x_y> point_info_;

    radius_x_y tempPoint_;        //定义临时变量，存储最小二乘圆心坐标

    radius_x_y min_coverage_circle_;




    int n, M;


    QVector<Line> L;

    QVector<Point> vec_point_;
    QVector<Point> a;
    QVector<Point> p;
    QVector<Point> C;

    QVector<double> filter_band_;

    double incircle_center_x;
    double incircle_center_y;
    double incircle_radius;


};

#endif // FFT_H



