/*                            ~~~~~~~~~~~~~~~~~ \\\\\\\\\\\\\\\\\\\\\\\\\ ~~~~~~~~~~~~~~                                         */
/*************************************************************************************************************/
/*******     ****         *****************|||||||||||||||||||||||||**************      ***********           **********************/
/*******    ****         *****   PROGRAMMATION D' UN ROBOT REFAISANT UNE CARTE  ***********       **************        ***********/
/*******     ****         *****************|||||||||||||||||||||||||**************      ***********           **********************/
/*************************************************************************************************************/
/*                            ~~~~~~~~~~~~~~~~~ \\\\\\\\\\\\\\\\\\\\\\\\\ ~~~~~~~~~~~~~~                                         */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <QApplication>
#include <QMessageBox>
#include "../include/robot.hpp"
#include "../include/lidar.hpp"
#include "../include/map.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    //----------------------------------------- Charger l'image de la carte initiale----------------------------------------------------------------------//
   
    std::string filepath = "../data/carte2.png";
    
    // -----------------------------------------Initialiser le robot , la map et  le lidar--------------------------------------------------------------//

    Robot robot;
    Lidar lidar;
    Map map(filepath); 
    
    //--------------------------------------analyse , interpretation et reconstruction de l'image----------------------------------------------// 

    while (true) {

        //-----------------------------------DEPLACEMENT ET FORME DU ROBOT LIDAR------------------------------------------------------------------//
        // Mettre à jour la position du robot
        robot.autoMove(map, lidar);
        
        // Obtenir les mesures du Lidar
        cv::Mat distances = lidar.getMeasurements(robot.getX(), robot.getY(), map);

        // Mettre à jour la carte
        map.updateMap(robot.getX(), robot.getY(), distances);
    
    //-----------------------------------INITIALISATION GLOBAL DE L'INTERFARCE----------------------------------------------------------------//
        cv::Mat reconstructedImage;
        reconstructedImage = map.getReconstructedMap();

        // Dessiner le robot sur la carte en rouge
        cv::circle(reconstructedImage, cv::Point(robot.getX(), robot.getY()), 8, cv::Scalar(0, 255, 0), -1); // Robot rouge
        //cv::circle(reconstructedImage, cv::Point(robot.getX(), robot.getY()), 4, cv::Scalar(0, 0, 0), -1); // Robot rouge

    //-----------------------------------MISE EN FORME ET RECONSTRUCTION DE LA MAP ENREIGISTRER---------------------------------------------//
    
        // Afficher l'image
        cv::imshow("~~~~~~~~~~~MAP RECONSTRUCTION~~~~~~~~~~~", reconstructedImage);

        // Sortir de la boucle si l'utilisateur appuie sur une touche
        if (cv::waitKey(30) >= 0) break; // Pause pour 30ms
    }

    return 0; // Terminer le programme
}       
//---------------------------------------fin du programmme----------------------------------------------------------------------------------//