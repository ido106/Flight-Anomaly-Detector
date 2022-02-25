

#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

void HybridAnomalyDetector::CalcLearnNormal(float max_correlation, const TimeSeries& ts, int i,
                                            int correlative_column) {
    const float max_threshold = ts.get_threshold();
    const float min_threshold = 0.5;

    //  0.9 < simple
    if(max_correlation > max_threshold) {
        SimpleAnomalyDetector::CalcLearnNormal(max_correlation, ts, i, correlative_column);

        // 0.5 < hybrid < 0.9
    } else if(max_correlation > min_threshold) {

        // get the names of the properties
        string feature1 = ts.get_column_name(i);
        string feature2 = ts.get_column_name(correlative_column);

        // get the circle between the two correlated properties
        Point** points = getPointsCorrelation(ts.get_column(i), ts.get_column(correlative_column),
                                              ts.get_raws_num());

        Circle circle = findMinCircle(points, ts.get_raws_num());
        delete(points);

        // made the correlated features struct
        correlatedFeatures* current_cf = new correlatedFeatures();
        current_cf->corrlation = max_correlation;
        current_cf->feature1 = feature1;
        current_cf->feature2 = feature2;
        current_cf->threshold = (circle.radius * 1.1);             // multiply by 1.1 to avoid false alarms
        current_cf->isHybrid = true;
        current_cf->radius = circle.radius;
        current_cf->xCenter = circle.center.x;
        current_cf->yCenter = circle.center.y;
        // add the correlated features
        (this->correlated_features).push_back(*current_cf);
    }
}

bool HybridAnomalyDetector::checkException (Point* p, correlatedFeatures current_feature) {
    if (current_feature.isHybrid) {
        // calculate x,y gaps between two points
        float x_gap = current_feature.xCenter - p->x;
        float y_gap = current_feature.yCenter - p->y;

        // calculate distance between two points with pitagoras
        float distance = sqrtf(x_gap * x_gap + y_gap * y_gap);

        // return true if the distance is bigger than the threshold
        if (distance > current_feature.threshold) {
            return true;
        }
        // else return false
        return false;
    }
    return SimpleAnomalyDetector::checkException(p, current_feature);
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}
