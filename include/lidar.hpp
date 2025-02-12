#ifndef LIDAR_HPP
#define LIDAR_HPP

#include <opencv2/opencv.hpp>
#include "map.hpp"

class Lidar {
public:
    Lidar();
    cv::Mat getMeasurements(int x, int y, Map& map);

private:
    int maxRange; // Portée maximale en mètres (ici pixels)
};

#endif
