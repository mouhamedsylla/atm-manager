// include/ui/windows.h
#ifndef ATM_WINDOWS_H
#define ATM_WINDOWS_H

#include <ncurses.h>

typedef struct {
    WINDOW *main_win;
    WINDOW *main_header;
    WINDOW *menu_win;
    // WINDOW *status_win;
    // WINDOW *operations_win;
    // WINDOW *details_win;
    WINDOW *message_win;
} ATM_UI;

// Initialize UI
ATM_UI* init_ui(void);
// Cleanup UI
void cleanup_ui(ATM_UI* ui);
// Update status window
void update_status(ATM_UI* ui, const char* username, const char* last_login);
// Show message in message window
void show_message(ATM_UI* ui, const char* message, int y, int x);
// update menu window
void update_menu(ATM_UI* ui, int highlight, char* choices[]);

#endif