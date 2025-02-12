#ifndef MAP_HPP
#define MAP_HPP

#include <opencv2/opencv.hpp>

class Map {
public:
    Map(const std::string& filepath);
    void updateMap(int x, int y, const cv::Mat& lidarData);
    int getSizeCols() const;
    int getSizeRows() const;
    int getPixelValue(int x, int y) const;
    bool hasNoBlack();
    bool hasNoBlackWall();
    cv::Mat getReconstructedMap();  

private:
    cv::Mat explorationMap;
    cv::Mat originalMap;
};

#endif
