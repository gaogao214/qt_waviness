#ifndef COMPLEX_NUMBER_H
#define COMPLEX_NUMBER_H
#include <QDebug>

class complex_number
{
public:
    complex_number();
    complex_number(double rl, double im);

    ~complex_number(void);


public:
    // 重载四则运算符号
    inline complex_number operator +(const complex_number &c) {
        return complex_number(m_rl + c.m_rl, m_im + c.m_im);
    }
    inline complex_number operator -(const complex_number &c) {
        return complex_number(m_rl - c.m_rl, m_im - c.m_im);
    }
    inline complex_number operator *(const complex_number &c) {
        return complex_number(m_rl*c.m_rl - m_im*c.m_im, m_im*c.m_rl + m_rl*c.m_im);
    }

    inline complex_number operator /(const complex_number &c) {
        if ((0==c.m_rl) && (0==c.m_im)) {
            qDebug("11111 ComplexNumber    ERROR: divider is 0!");
            return complex_number(m_rl, m_im);
        }
        return complex_number((m_rl*c.m_rl + m_im*c.m_im) / (c.m_rl*c.m_rl + c.m_im*c.m_im),
            (m_im*c.m_rl - m_rl*c.m_im) / (c.m_rl*c.m_rl + c.m_im*c.m_im));
    }

    void   SetValue(double rl, double im);

public:
    double     m_rl;    // 实部, real part
    double     m_im;    // 虚部, imagery part

};

#endif // COMPLEX_NUMBER_H


