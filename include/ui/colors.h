#ifndef COLORS_H
#define COLORS_H

#include <ncurses.h>


// Définition des indices de couleurs personnalisées
#define COLOR_ORANGE 8
#define COLOR_TEAL   9
#define COLOR_PINK   10
#define COLOR_YELLOW_CUSTOM 11

// Définition des paires de couleurs
#define MAIN_COLOR_PAIR      1
#define WINDOW_COLOR_PAIR    2  
#define ERROR_COLOR_PAIR     3  
#define INFO_COLOR_PAIR      4  
#define SUCCESS_COLOR_PAIR   5  
#define WARNING_COLOR_PAIR   6 
#define HIGHLIGHT_COLOR_PAIR 7
#define MENU_COLOR_PAIR      8
#define BUTTON_COLOR_PAIR    9  
#define INPUT_COLOR_PAIR     10

/**
 * Initialize color pairs
 * This function initializes the color pairs used in the application.
 */
static inline void init_color_pairs(void) {
    if (can_change_color()) {
        /**
         * Initialize custom colors (RGB values are in the range 0-1000)
         */
        init_color(COLOR_ORANGE, 1000, 500, 0);    
        init_color(COLOR_YELLOW_CUSTOM, 996, 996, 199);
    }
    
    // Initialiser les paires de couleurs
    init_pair(MAIN_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(WINDOW_COLOR_PAIR, COLOR_BLACK, COLOR_GREEN);
    init_pair(ERROR_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(INFO_COLOR_PAIR, COLOR_YELLOW_CUSTOM, COLOR_BLACK);
    init_pair(SUCCESS_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(WARNING_COLOR_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(HIGHLIGHT_COLOR_PAIR, COLOR_BLACK, COLOR_GREEN);
    init_pair(MENU_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(BUTTON_COLOR_PAIR, COLOR_BLACK, COLOR_CYAN);
    init_pair(INPUT_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);
}

#endif