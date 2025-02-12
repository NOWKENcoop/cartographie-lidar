#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <opencv2/opencv.hpp>
#include "lidar.hpp"
#include "map.hpp"

class Robot {
public:
    Robot();

    int getX() const;
    int getY() const;

    // Déplacer le robot de manière aléatoire
    void randomMove(Map& map, Lidar& lidar);

    void endSimulation();
    // Déplacement autonome avec Lidar pour éviter les obstacles et explorer la carte
    void autoMove(Map& map, Lidar& lidar) ; // Enlève `const` pour permettre la modification
    void FollowWall(int finX, int finY, Map& map);

private:
    int x, y;            // Position du robot
    int moveSpeed;       // Vitesse de déplacement du robot
};

#endif // ROBOT_HPP
