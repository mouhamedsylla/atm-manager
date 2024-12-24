#include "ui/windows.h"
#include <stdlib.h>

#define HEADER_COLOR_PAIR 1
#define CONTENT_COLOR_PAIR 2
#define ERROR_COLOR_PAIR 3
#define INFOS_COLOR_PAIR 4

int yMax, xMax;

ATM_UI* init_ui(void) {
    ATM_UI* ui = malloc(sizeof(ATM_UI));

    // Initialisation ncurses
    initscr();
    getmaxyx(stdscr, yMax, xMax);
    curs_set(0);
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Définition des couleurs
    init_pair(HEADER_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(CONTENT_COLOR_PAIR, COLOR_BLACK, COLOR_GREEN);
    init_pair(ERROR_COLOR_PAIR, COLOR_RED, COLOR_WHITE);

    // Fenêtre principale
    ui->main_win = newwin(LINES, COLS, 0, 0);
    box(ui->main_win, 0, 0);
    wbkgd(ui->main_win, COLOR_PAIR(HEADER_COLOR_PAIR));

    // En-tête
    ui->main_header = derwin(ui->main_win, 1, COLS - 4, 1, 2);
    wbkgd(ui->main_header, COLOR_PAIR(CONTENT_COLOR_PAIR));

    // Menu
    ui->menu_win = derwin(ui->main_win, 10, 40, LINES / 2 - 5, COLS / 2 - 20);
    box(ui->menu_win, 0, 0);
    wbkgd(ui->menu_win, COLOR_PAIR(HEADER_COLOR_PAIR));
    mvwprintw(ui->menu_win, 0, 2, "Welcome to ATM");

    keypad(ui->menu_win, TRUE);

    // Fenêtre des messages
    ui->message_win = derwin(ui->main_win, 1, COLS - 4, LINES - 2, 2);
    wbkgd(ui->message_win, COLOR_PAIR(CONTENT_COLOR_PAIR));

    refresh();
    wrefresh(ui->main_win);
    return ui;
}

void cleanup_ui(ATM_UI* ui) {
    delwin(ui->menu_win);
    delwin(ui->main_header);
    delwin(ui->main_win);
    delwin(ui->register_win);
    free(ui);
    endwin();
}

void show_message(ATM_UI* ui, const char* message, int y, int x) {
    werase(ui->main_header);
    mvwprintw(ui->main_header, y, x, "%s", message);
    wrefresh(ui->main_header);
}

void show_instructions(ATM_UI* ui) {
    init_color(COLOR_YELLOW, 996, 996, 199);
    init_pair(INFOS_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);

    wattron(ui->message_win, COLOR_PAIR(INFOS_COLOR_PAIR) | A_REVERSE);
    mvwprintw(ui->message_win, 0, 2, " INFOS ");
    wattroff(ui->message_win, COLOR_PAIR(INFOS_COLOR_PAIR) | A_REVERSE);

    mvwprintw(ui->message_win, 0, 11, "Use ");
    wattron(ui->message_win, A_BOLD);
    wprintw(ui->message_win, "ARROW UP");
    wattroff(ui->message_win, A_BOLD);
    wprintw(ui->message_win, " and ");
    wattron(ui->message_win, A_BOLD);
    wprintw(ui->message_win, "ARROW DOWN");
    wattroff(ui->message_win, A_BOLD);
    wprintw(ui->message_win, " keys to select. Press ");
    wattron(ui->message_win, A_BOLD);
    wprintw(ui->message_win, "ENTER");
    wattroff(ui->message_win, A_BOLD);
    wprintw(ui->message_win, " to confirm.");
    wrefresh(ui->message_win);
}

int update_menu(ATM_UI* ui, int highlight, char* choices[], int num_choices) {
    int choice;

    while (1) {
        for (int i = 0; i < num_choices; i++) {
            if (i == highlight) {
                mvwprintw(ui->menu_win, i * 2 + 2, 2, "[*]");
                wattron(ui->menu_win, A_REVERSE);
                mvwprintw(ui->menu_win, i * 2 + 2, 6, "%s", choices[i]);
                wattroff(ui->menu_win, A_REVERSE);
            } else {
                mvwprintw(ui->menu_win, i * 2 + 2, 2, "[ ] %s", choices[i]);
            }
        }

        choice = wgetch(ui->menu_win);
        switch (choice) {
            case KEY_UP:
                highlight = (highlight == 0) ? num_choices - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == num_choices - 1) ? 0 : highlight + 1;
                break;
            case 10: // ENTER key
                return highlight;
            default:
                break;
        }
    }
}

void register_menu(ATM_UI* ui) {
    ui->register_win = derwin(ui->main_win, 10, 60, LINES / 2 - 5, COLS / 2 - 30);
    box(ui->register_win, 0, 0);
    wbkgd(ui->register_win, COLOR_PAIR(HEADER_COLOR_PAIR));
    wrefresh(ui->register_win);

    mvwprintw(ui->register_win, 0, 2, "Register");
    mvwprintw(ui->register_win, 2, 2, "Username: ");
    mvwprintw(ui->register_win, 4, 2, "Password: ");
    mvwprintw(ui->register_win, 6, 2, "Confirm password: ");
    wrefresh(ui->register_win);
}

void login_menu(ATM_UI* ui) {
    ui->register_win = derwin(ui->main_win, 10, 60, LINES / 2 - 5, COLS / 2 - 30);
    box(ui->register_win, 0, 0);
    wbkgd(ui->register_win, COLOR_PAIR(HEADER_COLOR_PAIR));
    wrefresh(ui->register_win);

    mvwprintw(ui->register_win, 0, 2, "Login");
    mvwprintw(ui->register_win, 3, 2, "Username: ");
    mvwprintw(ui->register_win, 6, 2, "Password: ");
    wrefresh(ui->register_win);

}

void set_button(ATM_UI* ui) {
    mvwprintw(ui->main_win, LINES / 2 + 6, COLS / 2 + 21, "[ Next ]");
    mvwprintw(ui->main_win, LINES / 2 + 6, COLS / 2 - 29, "[ Back ]");
    wrefresh(ui->main_win);

    keypad(ui->main_win, TRUE);
    int choice;

    while (1)
    {
        choice = wgetch(ui->main_win);
        if(choice == KEY_RIGHT) {
            wattron(ui->main_win, A_REVERSE);
            mvwprintw(ui->main_win, LINES / 2 + 6, COLS / 2 + 21, "  Next  ");
            wattroff(ui->main_win, A_REVERSE);
            mvwprintw(ui->main_win, LINES / 2 + 6, COLS / 2 - 29, "[ Back ]");
            wrefresh(ui->main_win);
        } else if(choice == KEY_LEFT) {
            wattron(ui->main_win, A_REVERSE);
            mvwprintw(ui->main_win, LINES / 2 + 6, COLS / 2 - 29, "  Back  ");
            wattroff(ui->main_win, A_REVERSE);
            mvwprintw(ui->main_win, LINES / 2 + 6, COLS / 2 + 21, "[ Next ]");
            wrefresh(ui->main_win);
        } else if(choice == 10) {
            break;
        }   
    }
    

}

int main() {
    ATM_UI* ui = init_ui();

    // Message d'accueil
    show_message(ui, "ATM MANAGEMENT SYSTEM", 0, xMax / 2 - 10);
    show_instructions(ui);

    // Gestion du menu
    int highlight = 0;
    char* choices[] = { "Login", "Register", "Exit" };
    int num_choices = sizeof(choices) / sizeof(choices[0]);
    highlight = update_menu(ui, highlight, choices, num_choices);

    wclear(ui->menu_win);
    wrefresh(ui->menu_win);
    delwin(ui->menu_win);
    wrefresh(ui->main_win);
    

    switch (highlight)
        {
        case 1:
            register_menu(ui);
            set_button(ui);
            break;
        case 0:
            login_menu(ui);
            set_button(ui);
            break;
        default:
            break;
        }


    int ch;
    while (1) {
        if(highlight == 2) {
            break;
        }

        ch = getch();
        if(ch == 'q') {
            break;
        }
    }

    cleanup_ui(ui);
    return 0;
}
