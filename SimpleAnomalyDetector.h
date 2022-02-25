/**
Liron Balastra 206963316
Ido Aharon 319024600
**/

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

/**
 * struct of correlated features
 */
typedef struct {
    bool isHybrid;
    float xCenter;
    float yCenter;
    float radius;
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
} correlatedFeatures;


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
	vector<correlatedFeatures> correlated_features;
public:
    /**
     * constructor
     */

	SimpleAnomalyDetector();
    /**
     * destructor
     */
	virtual ~SimpleAnomalyDetector();

	/**
	 * the function gets TimeSeries and creates correlatedFeatures map of correlated columns.
	 * @param ts TimeSeries object of normal report, object of csv file.
	 */
	virtual void learnNormal(const TimeSeries& ts);

    /**
     * the function gets TimeSeries of new report and return all exceptions.
	 * @param ts TimeSeries object for report to check, object of csv file.
     * @return vector of all the exceptions in the report parameter .
     */
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);

	virtual void CalcLearnNormal(float max_correlation, const TimeSeries& ts, int i,
                            int correlative_column);

    Point** getPointsCorrelation(const float* first_property, const float* second_property, int size);
    /**
    * getter
    * @return the vector with the correlated column
    */
	vector<correlatedFeatures> getNormalModel(){
		return correlated_features;
	}

    virtual bool checkException (Point* p, correlatedFeatures current_feature);
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */