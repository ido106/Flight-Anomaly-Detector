/**
Liron Balastra 206963316
Ido Aharon 319024600
**/

#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "cmath"
#include <memory>
using std::unique_ptr;

SimpleAnomalyDetector::SimpleAnomalyDetector() = default;

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;

Point** SimpleAnomalyDetector::getPointsCorrelation(const float* first_property, const float* second_property, int size) {
    // create a size x point* array
    Point** points = new Point*[size];

    // made the points array
    for(int i = 0 ; i < size; i++) {
        points[i] = new Point(first_property[i], second_property[i]);
    }
    return points;
}
void SimpleAnomalyDetector::CalcLearnNormal(float max_correlation, const TimeSeries& ts, int i,
                                              int correlative_column) {
    const float threshold = ts.get_threshold();
    if (max_correlation > threshold) {
        // get the names of the properties
        string feature1 = ts.get_column_name(i);
        string feature2 = ts.get_column_name(correlative_column);


        // get the line reg between the two correlated properties
        Point** points = getPointsCorrelation(ts.get_column(i), ts.get_column(correlative_column),
                                              ts.get_raws_num());
        Line line_reg = linear_reg(points, ts.get_raws_num());

        // get the max distance between the line reg and the points of the correlated properties
        float max_deviation = max_dev(points, ts.get_raws_num(), line_reg);

        delete(points);

        // made the correlated features struct
        correlatedFeatures* current_cf = new correlatedFeatures();
        current_cf->lin_reg = line_reg;
        current_cf->corrlation = max_correlation;
        current_cf->feature1 = feature1;
        current_cf->feature2 = feature2;

        // multiply by 1.1 to avoid false alarms
        current_cf->threshold = (max_deviation * 1.1);
        current_cf->isHybrid = false;

        // add the correlated features
        (this->correlated_features).push_back(*current_cf);
    }
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {

    int num_column = ts.get_columns_num();

    for (int i = 1; i <= num_column ; i++) {
        float max_correlation = 0;
        int  correlative_column = -1;

        for (int j = i + 1; j <= num_column ; j++) {

            // fi - float array values of column i, fj - float array values of column j.
            const float* fi = ts.get_column(i);
            const float* fj = ts.get_column(j);

            // get the correlation between fi, fj
            float correlation = fabs(pearson(fi, fj, ts.get_raws_num()));

            if (correlation > max_correlation) {
                max_correlation = correlation;
                correlative_column = j;
            }
        }
        CalcLearnNormal(max_correlation, ts, i, correlative_column);
    }
}

bool SimpleAnomalyDetector::checkException (Point* p, correlatedFeatures current_feature) {
    float dev_result = dev(*p, current_feature.lin_reg);
    return (dev_result > current_feature.threshold);
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    std::vector<AnomalyReport> all_reports;
    for(correlatedFeatures current_feature : this->getNormalModel())
	 {

        // foreach every correlated feature in the normal model
         for(int i = 1 ; i <= ts.get_raws_num() ; i++) {

            // get column index by the name of the column
            int j_feature1 = ts.get_column_index(current_feature.feature1);
            int j_feature2 = ts.get_column_index(current_feature.feature2);

            // get the value of the two correlated features
            float feature1_value = ts.get_i_j_value(i, j_feature1);
            float feature2_value = ts.get_i_j_value(i, j_feature2);

            // measure the distance between the correlated features line and the current point
            Point* p = new Point(feature1_value, feature2_value);

            // ----------------------------------------------------------------/
            // check if there is an exception
            if (checkException(p, current_feature)) {
                std::string report_name = current_feature.feature1 + "-"+ current_feature.feature2;
                AnomalyReport* anomaly_report = new AnomalyReport(report_name, i);
                all_reports.push_back(*anomaly_report);
            }
        }
    }
    return all_reports;
}