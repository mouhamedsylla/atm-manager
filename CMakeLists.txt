cmake_minimum_required(VERSION 3.10)

project(atm C)

# Recherche des packages nécessaires
find_package(Curses REQUIRED)

# Configure les répertoires d'inclusion
include_directories(${CURSES_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collecte des sources
file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")

# Création de l'exécutable
add_executable(atm ${SOURCES} ${HEADERS})

# Configuration des répertoires d'inclusion
target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ${CURSES_INCLUDE_DIRS}
)

# Liaison avec les bibliothèques
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        ${CURSES_LIBRARIES}
        form  # Ajout de la bibliothèque form
)