

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;
using std::istringstream;
using namespace std;

class TimeSeries{
    const char* CSVfileName;

    // only the title of the table
    std::vector<std::string> properties_name;

    //key = raw id (from 1), value = vector of the raw values (string values)
    std::map<int, std::vector<string>> csv_contents;

    // key = column id (from 1), value = pointer to the column values as float.
    std::map<int, float* > columns_float_values;

    float threshold;

public:

    TimeSeries() = default;
    /**
     * constructor of time series. get the csv file name and build the data base
     * @param CSVfileName the csv file name
     */
    TimeSeries(const char* CSVfileName){
        this->threshold = 0.9;
        this->CSVfileName = CSVfileName;
        load_csv(CSVfileName);
        load_columns_vectors();
    }

    /**
     * destructor
     */
    ~TimeSeries() =default;

    /**
     * get the column j from the csv contents table
     * @param j the column to get
     * @return column j from the csv file
     */
    const float* get_column (int j) const {
        return columns_float_values.at(j);
    }

    /**
     * @return the number of columns in our csv table
     */
    int get_columns_num () const{
        return this->properties_name.size();
    }


    /**
     * @return the number of raws in our csv table, without title!
     */
    int get_raws_num() const {
        return this->csv_contents.size();
    }

    /**
     * get the value of the j property in time i - A[i][j]
     * @param i - row, j - column
     * @return float value from row i and column j.
     */
    float get_i_j_value (int i, int j) const {
        return std::stof(this->csv_contents.at(i).at(j - 1));
    }

    /**
     * get the index of the column by name.
     * @param column_name is the title of the column.
     * @return j index
     */
    int get_column_index (string column_name) const {
        for (int j = 0; j < this->properties_name.size(); ++j) {
            if (this->properties_name[j] == column_name)
                return (j + 1);
        }

        // column is not in the table
        return 0;
    }

    /**
     * get threshold value
     * @return threshold
     */
    float get_threshold () const {
        return this->threshold;
    }

    /**
     * set threshold value
     * @param newT - new value
     */
     void set_threshold(float newT) {
        this->threshold = newT;
    }

    /**
     * get the properties name by address
     * @param index of column
     * @return name of column
     */
    const string get_column_name (int column) const {
        return this->properties_name.at(column - 1);
    }


private:

    /**
     * load csv content to maps.
     * @param filename is the name of the file to load.
     */
    void load_csv(const char* filename) {
        string line;
        char delim = ',';

        ifstream input_file(filename);

        int row_id = 0;
        // read the file line by line
        while (getline(input_file, line)) {

            size_t start;
            size_t end = 0;
            // out is vector of the raw values.
            std::vector<std::string> out;

            while ((start = line.find_first_not_of(delim, end)) != std::string::npos)
            {
                end = line.find(delim, start);
                out.push_back(line.substr(start, end - start));
            }

            //table title
            if (row_id == 0) {
                this->properties_name = out;
            } else {
                // content table.
                this->csv_contents[row_id] = out;
            }
            row_id++;
        }
    }

    /**
     * load from the csv contents map the suitable columns and add them to the columns map
     */
    void load_columns_vectors () {
        // run all column
        int columns = this->get_columns_num();
        int rows = this->get_raws_num();

        // run all columns
        for (int j = 1; j <= columns; j++) {
            float* column_values = new float[rows];

            // run all rows
            for(int i=1; i <= rows; i++) {
                column_values[i-1] = get_i_j_value(i, j);
            }

            // insert
            this->columns_float_values[j] = column_values;
        }
    }
};

#endif /* TIMESERIES_H_ */
