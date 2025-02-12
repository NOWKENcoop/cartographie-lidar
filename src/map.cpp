#include "../include/map.hpp"

Map::Map(const std::string& filepath) {
    originalMap = cv::imread(filepath, cv::IMREAD_GRAYSCALE); // Utilisez GRAYSCALE pour une détection plus simple
    if (originalMap.empty()) {
        std::cerr << "[ERROR] Unable to load the map: " << filepath << std::endl;
    }
    explorationMap = cv::Mat(originalMap.size(), CV_8UC3, cv::Scalar(0, 0, 0)); // Initialiser explorationMap en noir
}

void Map::updateMap(int x, int y, const cv::Mat& lidarData) {
    for (int angle = 0; angle < 360; angle++) {
        float distance = lidarData.at<float>(0, angle);
        int LidarLength = 200;
        float radian = angle * M_PI / 180.0;

        for (int r = 1; r <= static_cast<int>(distance); r++) {
            // Compute the current position along the ray
            int posX = static_cast<int>(x + r * cos(radian));
            int posY = static_cast<int>(y + r * sin(radian));

            // Check if the position is within bounds
            if (posX >= 0 && posX < explorationMap.cols && posY >= 0 && posY < explorationMap.rows) {
                if (r == static_cast<int>(distance)) {
                    // At the end of the ray, mark obstacle in red
                    explorationMap.at<cv::Vec3b>(posY, posX) = cv::Vec3b(0, 0, 255);
                } else if(static_cast<int>(distance) != 0 && r!= static_cast<int>(distance)) {
                    // Along the ray, mark free space in white
                    explorationMap.at<cv::Vec3b>(posY, posX) = cv::Vec3b(255, 255, 255);
                } 
            }
        }
        if (static_cast<int>(distance)== 0){
            for (int r = 1; r <= LidarLength; r++){
               int posX = static_cast<int>(x + r * cos(radian));
               int posY = static_cast<int>(y + r * sin(radian));
                
               explorationMap.at<cv::Vec3b>(posY, posX) = cv::Vec3b(255, 255, 255);
            }
        }
       
    }

}

int Map::getSizeCols() const {
    return originalMap.cols;
}

int Map::getSizeRows() const {
    return originalMap.rows;
}

int Map::getPixelValue(int x, int y) const {
    return originalMap.at<uchar>(y, x);
}

bool Map::hasNoBlack() {
    // Parcours de tous les pixels de la map
    for (int y = 0; y < originalMap.rows; y++) {
        for (int x = 0; x < originalMap.cols; x++) {
            cv::Vec3b color = explorationMap.at<cv::Vec3b>(y, x); // Accès au pixel (B, G, R)
            if (color == cv::Vec3b(0, 0, 0)) {        // Si le pixel est noir
                return false;
            }
        }
    }
    return true; // Aucun pixel noir trouvé
}

bool Map::hasNoBlackWall(){
    for(int y = 20; y <originalMap.rows-20; y++){
        cv::Vec3b colorLeft = explorationMap.at<cv::Vec3b>(y, 20); // Accès au pixel (B, G, R)
        // cv::Vec3b colorRight = explorationMap.at<cv::Vec3b>(y, originalMap.cols-1); // Accès au pixel (B, G, R)
        if (colorLeft == cv::Vec3b(0, 0, 0)) {        // Si le pixel est noir
            return false;
        }
    }
    for(int x = 20; x <originalMap.cols-20; x++){
        cv::Vec3b colorUp = explorationMap.at<cv::Vec3b>(20, x); // Accès au pixel (B, G, R)
        // cv::Vec3b colorDown = explorationMap.at<cv::Vec3b>(originalMap.rows-1, x); // Accès au pixel (B, G, R)
        if (colorUp == cv::Vec3b(0, 0, 0)) {        // Si le pixel est noir
            return false;
        }
    }
    for(int y = 20; y <originalMap.rows-20; y++){
        // cv::Vec3b colorLeft = explorationMap.at<cv::Vec3b>(y, 1); // Accès au pixel (B, G, R)
        cv::Vec3b colorRight = explorationMap.at<cv::Vec3b>(y, originalMap.cols-20); // Accès au pixel (B, G, R)
        if ( colorRight == cv::Vec3b(0, 0, 0)) {        // Si le pixel est noir
            return false;
        }
    }
    for(int x = 20; x <originalMap.cols-20; x++){
        // cv::Vec3b colorUp = explorationMap.at<cv::Vec3b>(1, x); // Accès au pixel (B, G, R)
        cv::Vec3b colorDown = explorationMap.at<cv::Vec3b>(originalMap.rows-20, x); // Accès au pixel (B, G, R)
        if (colorDown == cv::Vec3b(0, 0, 0)) {        // Si le pixel est noir
            return false;
        }
    }
    return true;    
}

cv::Mat Map::getReconstructedMap() {
    return explorationMap.clone();
}