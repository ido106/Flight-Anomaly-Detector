cmake_minimum_required(VERSION 3.17)
project(ex1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(ex1 anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h MainTrain.cpp SimpleAnomalyDetector.cpp
        SimpleAnomalyDetector.h timeseries.cpp timeseries.h minCircle.cpp minCircle.h HybridAnomalyDetector.cpp
        HybridAnomalyDetector.h CLI.cpp CLI.h commands.h Server.cpp Server.h)