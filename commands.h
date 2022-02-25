

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;


class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
};


typedef struct{
    TimeSeries* trainTS;
    TimeSeries* testTS;
    vector<AnomalyReport> hybridReport;
} Info;


class Command {
	string description;
protected:
    DefaultIO* dio;
public:
	Command(DefaultIO* dio, string description):dio(dio), description(description) {}
	virtual void execute(Info* info)=0;
	string getDescription() {
        return description;
	}
	virtual ~Command() = default;
};

class UploadCSV: public Command {
public:
    UploadCSV(DefaultIO* dio):Command(dio, "upload a time series csv file") {}
    virtual void execute(Info* info) {

        this->dio->write("Please upload your local train CSV file.\n");

        fstream trainOutCSV;
        trainOutCSV.open("anomalyTrain.csv", ios::out );
        string line;
        bool stop = false;

        while (!stop) {
            line = this->dio->read();
            if (line == "done" || line == "done\n") {
                stop = true;
            }
            else {
                trainOutCSV << line  <<endl;
            }
        }

        trainOutCSV.close(); // ido added

        this->dio->write("Upload complete.\n");
        info->trainTS = new TimeSeries("anomalyTrain.csv");

        // -------------------------------------------------------------------------
        this->dio->write("Please upload your local test CSV file.\n");

        fstream testOutCSV;
        testOutCSV.open("anomalyTest.csv", ios::out );

        stop = false;
        while (!stop) {
            line = this->dio->read();
            if (line == "done" || line == "done\n") {
                stop = true;
            }
            else {
                testOutCSV << line <<endl;
            }
        }

        testOutCSV.close(); // ido added
        this->dio->write("Upload complete.\n");
        info->testTS = new TimeSeries("anomalyTest.csv");
    }
};

class AlgorithmSettings: public Command {
public:
    AlgorithmSettings(DefaultIO* dio):Command(dio, "algorithm settings") {}
    virtual void execute(Info* info) {

        bool stop = false;

        while(!stop) {
            this->dio->write("The current correlation threshold is ");
            this->dio->write(info->testTS->get_threshold());
            this->dio->write("\nType a new threshold\n");

            float newThreshold;
            this->dio->read(&newThreshold);
            if(newThreshold>0 && newThreshold<=1){
                info->testTS->set_threshold(newThreshold);
                info->trainTS->set_threshold(newThreshold);
                stop=true;
            }
            else
                this->dio->write("please choose a value between 0 and 1.\n");

        }
    }
};

class DetectAnomalies: public Command {
public:
    DetectAnomalies(DefaultIO* dio):Command(dio, "detect anomalies") {}
    virtual void execute(Info* info) {

        HybridAnomalyDetector ad;
        ad.learnNormal(*info->trainTS);
        info->hybridReport = ad.detect(*info->testTS);

        this->dio->write("anomaly detection complete.\n");
    }
};

class DisplayResults: public Command {
public:
    DisplayResults(DefaultIO* dio):Command(dio, "display results") {}
    virtual void execute(Info* info) {

        int len = info->hybridReport.size();
        long time_step;
        string description;
        for (int i = 0; i < len ; ++i) {
            time_step = info->hybridReport[i].timeStep;
            description = info->hybridReport[i].description;
            this->dio->write(time_step);
            this->dio->write("\t" + description + "\n");
        }
        this->dio->write("Done.\n");
    }
};

class UploadAnomaliesAnalyzeResult: public Command {
public:
    UploadAnomaliesAnalyzeResult(DefaultIO* dio):Command(dio, "upload anomalies and analyze results") {}
    virtual void execute(Info* info) {

        this->dio->write("Please upload your local anomalies file.\n");

        // timeException = < <"s1", "e1">, <"s2", "e2"> ... >
        vector<vector<string>> timeException = this->upload("analyze.csv");
        this->dio->write("Upload complete.\n");

        float P = timeException.size();
        float N = info->testTS->get_raws_num();

        int lenE = timeException.size();
        for (int i = 0; i < lenE; ++i) {
            N -= ((stol(timeException[i][1]) - stol(timeException[i][0]) + 1));
        }
        //--------------------------------------------------------------------

        // timeReports < <s1, e1>, <s2, e2> ... >
        vector<vector<long>> timeReports;

        long startTime = info->hybridReport.at(0).timeStep;
        long endTime = startTime;
        string description = info->hybridReport.at(0).description;

        int lenR = info->hybridReport.size();
        for (int i = 1; i < lenR; ++i) {
            if (info->hybridReport.at(i).description == description) {
                endTime++;
            } else {
                vector<long> out;
                out.push_back(startTime);
                out.push_back(endTime);
                timeReports.push_back(out);

                startTime = info->hybridReport.at(i).timeStep;
                endTime = startTime;
                description = info->hybridReport.at(i).description;

            }
        }
        // for the last match
        vector<long> out;
        out.push_back(startTime);
        out.push_back(endTime);
        timeReports.push_back(out);


        // check matching
        float TP = 0;
        float FP = 0;

        lenR = timeReports.size();
        for (int i = 0; i < lenR; ++i) {
            int startReport = timeReports.at(i).at(0);
            int endReport = timeReports.at(i).at(1);
            int startException, endException;
            int stop = false;
            for (int j = 0; j < lenE && !stop  ; ++j) {
                startException = stoi(timeException.at(j).at(0));
                endException = stoi(timeException.at(j).at(1));
                if (((startReport >= startException) && (startReport <= endException))
                        || ((endReport >= startException) && (endReport <= endException))) {
                    TP++;
                    stop = true;
                } else if ((startReport <= startException) && (endReport >= endException))  {
                    TP++;
                    stop = true;
                }
            }
            // no match
            if (stop == false) {
                FP++;
            }
        }

        float TP_ratio =((int)(1000.0*TP/P))/1000.0f;
        float FP_ratio =((int)(1000.0*FP/N))/1000.0f;

        this->dio->write("True Positive Rate: ");
        this->dio->write(TP_ratio);
        this->dio->write("\nFalse Positive Rate: ");
        this->dio->write(FP_ratio);
        this->dio->write("\n");
    }
private:
    vector<vector<string>> upload(string path) {
        string line;
        char delim = ',';

        // < <s1, e1>, <s2, e2> ... >
        vector<vector<string>> v;
        bool stop = false;

        while (!stop) {
            line = this->dio->read();

            if (line == "done" || line == "done\n") {
                stop = true;
            } else {
                size_t start;
                size_t end = 0;
                // out is vector of the raw values.
                vector<string> out;

                while ((start = line.find_first_not_of(delim, end)) != std::string::npos)
                {
                    end = line.find(delim, start);
                    out.push_back(line.substr(start, end - start));
                }
                v.push_back(out);
            }
        }
        return v;
    }
};

class Exit: public Command {
public:
    Exit(DefaultIO* dio):Command(dio, "exit") {}
    virtual void execute(Info* info) {}
};
#endif /* COMMANDS_H_ */
