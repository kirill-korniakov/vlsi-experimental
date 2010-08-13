// GNU General Public License Agreement
// Copyright (C) 2004-2010 CodeCogs, Zyba Ltd, Broadwood, Holford, TA5 1DU, England.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by CodeCogs. 
// You must retain a copy of this licence in all copies. 
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
// ---------------------------------------------------------------------------------
//! Calculates the linear regression parameters and evaluates the regression line at arbitrary abscissas

/* EXAMPLE:

    double x[7] = { 1.5, 2.4, 3.2, 4.8,  5.0, 7.0,  8.43 };
    double y[7] = { 3.5, 5.3, 7.7, 6.2, 11.0, 9.5, 10.27 };
 
    Maths::Regression::Linear A(7, x, y);
    //Regression is y=a+bx; a is Intercept, b is Slope.	 
    cout << "    Slope = " << A.getSlope() << endl;
    cout << "Intercept = " << A.getIntercept() << endl << endl;
 
    cout << "Regression coefficient = " << A.getCoefficient() << endl;
 
    cout << endl << "Regression line values" << endl << endl;
    for (double i = 0.0; i <= 3; i += 0.6) 
    {
        cout << "x = " << setw(3) << i << "  y = " << A.getValue(i);
        cout << endl;
    }

    //Found here: https://www.codecogs.com/d-ox/maths/approximation/regression/linear.php
*/

#ifndef MATHS_REGRESSION_LINEAR_H
#define MATHS_REGRESSION_LINEAR_H

#include <assert.h>
#include <math.h>

namespace Maths
{

namespace Regression
{

//! Given a set of points, this class calculates the linear regression parameters and evaluates the regression line at arbitrary abscissas.

class Linear
{
public:

//! Class constructor

Linear(int n, double *x, double *y)
{

            // calculate the averages of arrays x and y
            double xa = 0, ya = 0;
            for (int i = 0; i < n; i++) {
                xa += x[i];
                ya += y[i];
            }
            xa /= n;
            ya /= n;

            // calculate auxiliary sums
            double xx = 0, yy = 0, xy = 0;
            for (int i = 0; i < n; i++) {
                double tmpx = x[i] - xa, tmpy = y[i] - ya;
                xx += tmpx * tmpx;
                yy += tmpy * tmpy;
                xy += tmpx * tmpy;
            }

            // calculate regression line parameters

            // make sure slope is not infinite
            assert(fabs(xx) != 0);

                m_b = xy / xx;
                m_a = ya - m_b * xa;
            m_coeff = (fabs(yy) == 0) ? 1 : xy / sqrt(xx * yy);

        }

//! Evaluates the linear regression function at the given abscissa.

double getValue(double x)
{
    return m_a + m_b * x;
}

//! Returns the slope of the regression line
double getSlope()
{
   return m_b;
}

//! Returns the intercept on the Y axis of the regression line
double getIntercept()
{
  return m_a;
}

//! Returns the linear regression coefficient

double getCoefficient()
{
  return m_coeff;
}

private:

double m_a, m_b, m_coeff;
};


//! A static function implementing the Linear Class for one off calculations

/*double Linear_once(int n, double *x, double *y, double a )
{
  // This function is created to enable an Instant Calculator on CodeCogs. 
  // You probably shouldn't be using this function otherwise. 

   Maths::Regression::Linear A(n, x, y);
   return A.getValue(a);
}*/

}

}

#endif

