#include "lidar.hpp"
#include <cmath>

Lidar::Lidar() : maxRange(250) {}

cv::Mat Lidar::getMeasurements(int x, int y, Map& map) {
    cv::Mat distances = cv::Mat::zeros(1, 360, CV_32F);
    for (int angle = 0; angle < 360; angle++) {
        float radian = angle * M_PI / 180.0;
        for (int r = 0; r < maxRange; r++) {
            int dx = static_cast<int>(x + r * cos(radian));
            int dy = static_cast<int>(y + r * sin(radian));
                //dx=dx/10; dy=dy/10;
            if (dx < 0 || dx >= map.getSizeCols() || dy < 0 || dy >= map.getSizeRows() || map.getPixelValue(dx, dy) == 0) {
                distances.at<float>(0, angle) = r; // Marque la distance jusqu'à l'obstacle
                break;
            }
        }
    }

    return distances;

}