/**
Liron Balastra 206963316
Ido Aharon 319024600
**/

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();

    void CalcLearnNormal(float max_correlation, const TimeSeries& ts, int i,
                         int correlative_column);

    bool checkException (Point* p, correlatedFeatures current_feature);

};


#endif /* HYBRIDANOMALYDETECTOR_H_ */
