// #include <ncurses.h>

// int main() {
//     initscr();
//     noecho();
//     curs_set(0);

//     int yMax, xMax;
//     getmaxyx(stdscr, yMax, xMax);
    
//     WINDOW *header = newwin()

//     WINDOW *win = newwin(yMax/2, xMax/2, yMax/4, xMax/4);

//     box(win, 0, 0);
        
//     mvwprintw(win, 0, 2, "ATM");

//     wgetch(win);
     
//     endwin();
//     return 0;
// }

#include "ui/window.h"
#include <stdlib.h>

int yMax, xMax;

ATM_UI* init_ui(void) {
    ATM_UI* ui = malloc(sizeof(ATM_UI));

    
    // Initialize ncurses
    initscr();
    getmaxyx(stdscr, yMax, xMax);
    curs_set(0);
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Color pairs
    init_pair(1, COLOR_GREEN, COLOR_BLACK);  // Headers
    init_pair(2, COLOR_BLACK, COLOR_GREEN); // Normal content
    //init_pair(3, COLOR_RED, COLOR_WHITE);   // Errors/Important

    // Create main window
    ui->main_win = newwin(LINES, COLS, 0, 0);
    //newwin(yMax/2, xMax/2, yMax/4, xMax/4);
    //newwin(LINES, COLS, 0, 0);
    box(ui->main_win, 0, 0);
    wbkgd(ui->main_win, COLOR_PAIR(1));

    // Create header window
    ui->main_header = derwin(ui->main_win, 1, COLS-4, 1, 2);
    wbkgd(ui->main_header, COLOR_PAIR(2));

    //wbkgd(ui->main_header, COLOR_PAIR(2));
    ui->menu_win = derwin(ui->main_win, 10, 40, LINES/2 - 5, COLS/2 - 20);
    box(ui->menu_win, 0, 0);
    wbkgd(ui->menu_win, COLOR_PAIR(1));
    mvwprintw(ui->menu_win, 0, 2, "MENU");
    mvwprintw(ui->menu_win, 2, 2, "[*] Login");
    mvwprintw(ui->menu_win, 4, 2, "[ ] Register");
    mvwprintw(ui->menu_win, 6, 2, "[ ] Exit");
    
    // // Create menu window
    // ui->menu_win = derwin(ui->main_win, 6, 20, 2, 2);
    // box(ui->menu_win, 0, 0);
    
    // // Create status window
    // ui->status_win = derwin(ui->main_win, 6, 40, 2, 25);
    // box(ui->status_win, 0, 0);
    
    // // Create operations window
    // ui->operations_win = derwin(ui->main_win, 8, COLS-6, 9, 2);
    // box(ui->operations_win, 0, 0);
    
    // // Create details window
    // ui->details_win = derwin(ui->main_win, 8, COLS-6, 18, 2);
    // box(ui->details_win, 0, 0);
    
    // // Create message window
    // ui->message_win = derwin(ui->main_win, 3, COLS-4, LINES-4, 2);
    // box(ui->message_win, 0, 0);
    
    refresh();
    wrefresh(ui->main_win);
    return ui;
}

void cleanup_ui(ATM_UI* ui) {
    delwin(ui->menu_win);
    // delwin(ui->status_win);
    // delwin(ui->operations_win);
    // delwin(ui->details_win);
    // delwin(ui->message_win);
    delwin(ui->main_header);
    delwin(ui->main_win);
    free(ui);
    endwin();
}

void show_message(ATM_UI* ui, const char* message, int y, int x) {
    werase(ui->main_header);
    mvwprintw(ui->main_header, y, x, "%s", message);
    wrefresh(ui->main_header);
}

void update_status(ATM_UI* ui, const char* username, const char* last_login) {
    // werase(ui->status_win);
    // box(ui->status_win, 0, 0);
    // mvwprintw(ui->status_win, 1, 2, "User: %s", username);
    // mvwprintw(ui->status_win, 2, 2, "Last login: %s", last_login);
    // wrefresh(ui->status_win);
}


int main() {
    ATM_UI* ui = init_ui();
    
    // Exemple d'utilisation
    update_status(ui, "Alice", "2024-12-22 14:30");
    show_message(ui, "ATM MANAGEMENT SYSTEM", 0, xMax/2 - 10);
    
    // Boucle principale
    int ch;
    while ((ch = getch()) != 'q') {
        // Gérer les entrées utilisateur
    }
    
    cleanup_ui(ui);
    return 0;
}