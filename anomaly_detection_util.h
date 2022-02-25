

#ifndef ANOMALYDETECTORUTIL_H_
#define ANOMALYDETECTORUTIL_H_

/**
 * returns the variance of X
 * @param x float array
 * @param size float array length
 * @return the variance of x
 */
float var(const float* x, int size);

/**
 * returns the covariance of X and Y
 * @param x first float array
 * @param y second float array
 * @param size float array length
 * @return the covariance of x and y
 */
float cov(const float* x,const float* y, int size);

/**
 * returns the Pearson correlation coefficient of X and Y
 * @param x first float array
 * @param y second float array
 * @param size float array length
 * @return Pearson correlation coefficient of X and Y
 */
float pearson(const float* x,const float* y, int size);

/**
 * Line class
 */
class Line{
public:
    float a,b;
    Line():a(0),b(0){}
    Line(float a, float b):a(a),b(b){}
    float f(float x){
        return a*x+b;
    }
};

/**
 * Point class
 */
class Point{
public:
    float x,y;
    Point(float x, float y):x(x),y(y){}
};

/**
 * performs a linear regression and return s the line equation
 * @param points points of linear regression
 * @param size the size of points array
 * @return the line of the linear regression
 */
Line linear_reg(Point** points, int size);

/**
 * returns the deviation between point p and the line equation of the points
 * @param p the point for measureing
 * @param points the points of line
 * @param size the size of points array
 * @return the distance between p and the line
 */
float dev(Point p, Point** points, int size);

/**
 * returns the deviation between point p and the line equation of the points
 * @param p the point
 * @param l the line
 * @return the distance between point p and line l
 */
float dev(Point p, Line l);

/**
 * return the max deviation found between an array of points to the line
 * @param Points the points array
 * @param size the size of the array
 * @param l the line
 * @return max deviation found between the points and the line
 */
float max_dev(Point** Points, int size, Line l);

#endif
