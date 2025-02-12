#include "robot.hpp"
#include <cstdlib>
#include <ctime>
#include "lidar.hpp"
#include <cmath>
#include <random>
#include <iostream>
#include <gtk/gtk.h>
#include <QMessageBox>  // Pour utiliser QMessageBox
#include <QApplication> // Si vous ne l'avez pas déjà, pour gérer l'application Qt
#include <QPushButton>
#include <queue>
#include <unordered_map>
#include <functional>
#include <cmath>
#include <limits>

Robot::Robot() : x(300), y(200), moveSpeed(2) { // Initialiser la position et la vitesse
}
int number = 0;
int Robot::getX() const { return x; }
int Robot::getY() const { return y; }

void Robot::randomMove(Map& map, Lidar& lidar) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0.0, 1.0); // Générer des nombres entre 0 et 1

    cv::Mat distances = lidar.getMeasurements(x, y, map);
    const float minObstacleDistance = 30.0; // Distance minimale pour éviter des collisions
    const float maxObstacleDistance = 100.0; // Distance maximale pour choisir un obstacle
    float distance = 0.0;
    int randomAngle = 0;

    // Initialiser les coordonnées de départ
    const int startX = x;
    const int startY = y;

    // Essayer plusieurs angles jusqu'à trouver un déplacement valide
    for (int attempts = 0; attempts < 20; attempts++) {
        randomAngle = static_cast<int>(distrib(gen) * 360); // Angle aléatoire
        distance = distances.at<float>(0, randomAngle);

        // Vérifier que la distance est dans les limites
        if (maxObstacleDistance > distance &&  distance > minObstacleDistance) break ;
    }

    // Calculer les composants du déplacement
    float radian = randomAngle * M_PI / 180.0;
    float dx = moveSpeed * cos(radian);
    float dy = moveSpeed * sin(radian);

    // Suivre la direction choisie tant que la distance minimale est respectée
    while (distance > minObstacleDistance) {
        int newX = x + static_cast<int>(dx);
        int newY = y + static_cast<int>(dy);

        // Vérifier la validité de la nouvelle position
        if (newX > 0 && newX < map.getSizeCols() && newY > 0 && newY < map.getSizeRows() && map.getPixelValue(newX, newY) != 0) {
            x = newX;
            y = newY;
            std::cout << "Position: (" << startX << ", " << startY << ")" << std::endl;

            // Recalculer la distance restante au point initial
            distance = std::sqrt((x - startX) * (x - startX) + (y - startY) * (y - startY));
        } else {
            // Si la position est invalide, arrêter le déplacement
            break;
        }
    }
}

void Robot:: FollowWall(int finX, int finY, Map& map){

    int deltaX = finX - x;
    int deltaY = finY - y;
    float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    
    // Calcul du déplacement proportionnel
    if (distance > 0) {
        float angle = std::atan2(deltaY, deltaX);
        std::cout << "Angle est : " << angle*(180/M_PI) << std::endl;
        float dx = moveSpeed * cos(angle);
        float dy = moveSpeed * sin(angle);
        
        int newX = x + static_cast<int>(dx);
        int newY = y + static_cast<int>(dy);
        
        // Validation de la nouvelle position
        if (newX > 0 && newX < map.getSizeCols() && newY > 0 && newY < map.getSizeRows() && map.getPixelValue(newX, newY) != 0) {
            x = newX;
            y = newY;
            std::cout << "i" << std::endl;
        }
    }
}


void Robot::endSimulation() {
    QMessageBox messageBox;
    messageBox.setWindowTitle("VOULEZ VOUS QUITTER");
    messageBox.setText("la carte est reconstituée (quitter ou continuer )");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);

    int response = messageBox.exec();
    if (response == QMessageBox::Yes) {
        std::cout << "Simulation fermée." << std::endl;
        exit(0);  // Ferme le programme
    }
}



void Robot::autoMove(Map& map, Lidar& lidar) {
    // Obtenir les mesures du LiDAR
    cv::Mat distances = lidar.getMeasurements(x, y, map);
    int closestObstacleAngle = -1;
    float minDistance = 20.0;

    // Trouver l'obstacle le plus proche
    for (int angle = 0; angle < 360; angle++) {
        float distance = distances.at<float>(0, angle);
        if (distance < minDistance && distance > 0) {
            minDistance = distance;
            closestObstacleAngle = angle;
        }
    }
    if (minDistance< 30.0 && minDistance>10.0) {
        int followWallAngle = (closestObstacleAngle + 90) % 360; // 90° pour suivre le mur
        int targetX = x + static_cast<int>(moveSpeed * cos(followWallAngle * M_PI / 180));
        int targetY = y + static_cast<int>(moveSpeed * sin(followWallAngle * M_PI / 180));

        FollowWall(targetX, targetY, map);
       // minDistance+=10;
    }
    else{
        randomMove(map,lidar);
    }

    // Une fois les murs explorés, se diriger vers le centre de la carte
    if (map.hasNoBlackWall() && !map.hasNoBlack()) {
        randomMove(map, lidar);
    }

    // Terminer la simulation si tout est exploré
    if (map.hasNoBlack()) {
        endSimulation();
    }
}