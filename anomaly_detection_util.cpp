/**
Liron Balastra 206963316
Ido Aharon 319024600
**/

#include "anomaly_detection_util.h"
#include <cmath>
using namespace std;

// local functions
float mean(const float* x, int size);
float standard_deviation(const float* x, int size);

/**
 * return min value
 * @param x the values array
 * @param size size of array
 * @return min value
 */
float mean(const float* x, int size) {
    float m = 0;
    for (int i = 0; i < size; ++i) {
        m += x[i];
    }
    return m *  (1 / float (size));
}

/**
 * return standard deviation value.
 * @param x the floats array
 * @param size size of array
 * @return the standard deviation of x
 */
float standard_deviation(const float* x, int size) {
    return sqrtf(var(x, size));
}

/**
 * return variance value of floats array
 * @param x the floats array
 * @param size array length
 * @return the variance value
 */
float var(const float* x, int size) {

    // m-mean, v-var.
    float m = mean(x, size);
    float v = 0;

    for (int i = 0; i < size; ++i) {
        v += powf(x[i], 2);
    }
    v = v *  (1 / float (size));
    v = v - powf(m, 2);
    return v;
}

float cov(const float* x,const float* y, int size) {
    float xMean = mean(x, size);
    float yMean = mean(y, size);
    float cov = 0;
    for (int i = 0; i < size; ++i) {
        cov += (x[i] - xMean) * (y[i] - yMean);
    }
    return cov * (1 / float (size));
}

float pearson(const float* x,const float* y, int size) {
    return cov(x, y, size) / (standard_deviation(x, size) * standard_deviation(y, size));
}

Line linear_reg(Point** points, int size) {
    float xArr[size];
    float yArr[size];

    for (int i = 0; i < size; ++i) {
        xArr[i] = points[i]->x;
        yArr[i] = points[i]->y;
    }

    float a, b;
    a = cov(xArr, yArr, size) / var(xArr, size);
    b = mean(yArr, size) - a * mean(xArr, size);
    return Line(a,b);
}

float dev(Point p, Line l) {
    float expectedY = (l.a * p.x) + l.b;
    return fabs(p.y - expectedY);
}

float dev(Point p, Point** points, int size) {
    Line l = linear_reg(points, size);
    float expectedY = (l.a * p.x) + l.b;
    return fabs(p.y - expectedY);
}

float max_dev(Point** Points, int size, Line l) {
    float max_dev = 0;
    // iterate all points
    for(int i = 0; i < size; i++) {
        float current_dev = dev(*(Points[i]), l);
        if(current_dev > max_dev) {
            max_dev = current_dev;
        }
    }
    return max_dev;
}
