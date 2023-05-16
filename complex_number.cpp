#include "complex_number.h"

complex_number::complex_number()
{
    m_rl = 0;
    m_im = 0;
}

complex_number::complex_number(double rl,double im)
{
    m_rl=rl;
    m_im=im;
}

complex_number::~complex_number()
{

}

void complex_number::SetValue(double rl, double im)
{

    m_rl=rl;
    m_im=im;
}
