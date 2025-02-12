cmake_minimum_required(VERSION 3.10)

# Nom du projet et spécification du langage
project(projet_info VERSION 1.0 LANGUAGES CXX)

# Activer le standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Trouver OpenCV (cela va chercher la configuration d'OpenCV installée sur le système)

# Trouver GTK+3
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Ajouter les répertoires d'inclusion GTK+3
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

find_package(OpenCV REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)  # Cherche Qt5 Widgets

# Afficher les informations sur OpenCV trouvé
message(STATUS "OpenCV version : ${OpenCV_VERSION}")
message(STATUS "OpenCV include path : ${OpenCV_INCLUDE_DIRS}")

# ---- Définition des variables ----

# Charger les fichiers source dans une variable SOURCES
set(SOURCES
    src/main.cpp    # Fichier principal avec la fonction main
    src/lidar.cpp
    src/map.cpp
    src/robot.cpp  
    # src/my_functions.cpp  # Si vous avez d'autres fichiers source, décommentez ici
)
set(HEADERS 
            include/lidar.hpp
            include/map.hpp
            include/robot.hpp
)

# Ajouter les fichiers d'en-tête pour être trouvés par le compilateur
include_directories(include)

# ---- Configuration de la compilation ----

# Compilation avec des avertissements et des optimisations
add_compile_options(
    -Wall      # Afficher tous les avertissements
    -Wextra    # Avertissements supplémentaires
    -Wpedantic # Respect strict du standard
    -O3        # Optimisation maximale du code
)

# ---- Génération de l'exécutable ----

# Créer l'exécutable
add_executable(robot ${SOURCES})

# ---- Ajout des bibliothèques ----

# Exemple avec la bibliothèque mathématique standard "m" sous Linux
target_link_libraries(robot m)

# Lier l'exécutable avec les bibliothèques OpenCV
target_link_libraries(robot ${OpenCV_LIBS} Qt5::Widgets)
# Lier les bibliothèques GTK+3
target_link_libraries(robot ${GTK3_LIBRARIES})
target_link_libraries(robot Qt5::Widgets)  # Lie la bibliothèque Qt5 Widgets

