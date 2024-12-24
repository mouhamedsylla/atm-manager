#include "ui/windows.h"
#include <stdlib.h>
#include <string.h>
#include <form.h>

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

void set_button(ATM_UI* ui) {
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

void mask_password_field(FIELD *field) {
    // Get the current content of the field
    char *temp = field_buffer(field, 0);
    if (!temp) return;

    // Create a string of asterisks the same length as the content
    size_t len = strlen(temp);
    char *masked = malloc(len + 1);
    memset(masked, '*', len);
    masked[len] = '\0';

    // Set the field buffer to show asterisks
    set_field_buffer(field, 0, masked);
    free(masked);
}

void register_menu(ATM_UI* ui) {
    FIELD *fields[7];
    FORM *form;
    char password_buffer[31] = {0};  // Store actual password
    char confirm_buffer[31] = {0};   // Store confirm password
    int password_len = 0;
    int confirm_len = 0;
    int current_field = 0;

    mvwprintw(ui->main_win, LINES / 2 + 6, COLS / 2 + 21, "[ Next ]");
    mvwprintw(ui->main_win, LINES / 2 + 6, COLS / 2 - 29, "[ Back ]");
    wrefresh(ui->main_win);

    ui->register_win = derwin(ui->main_win, 10, 60, LINES / 2 - 5, COLS / 2 - 30);
    box(ui->register_win, 0, 0);
    wbkgd(ui->register_win, COLOR_PAIR(HEADER_COLOR_PAIR));
    
    fields[0] = new_field(1, 30, 1, 12, 0, 0);  // Username
    fields[1] = new_field(1, 30, 3, 12, 0, 0);  // Password
    fields[2] = new_field(1, 30, 5, 12, 0, 0);  // Confirm password
    fields[3] = NULL;

    for(int i = 0; i < 3; i++) {
        //set_field_back(fields[i], A_UNDERLINE);
        field_opts_off(fields[i], O_AUTOSKIP);
        set_field_opts(fields[i], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    }

    form = new_form(fields);
    set_form_win(form, ui->register_win);
    set_form_sub(form, derwin(ui->register_win, 8, 58, 1, 1));
    post_form(form);

    mvwprintw(ui->register_win, 0, 2, "Register");
    mvwprintw(ui->register_win, 2, 2, "Username:");
    mvwprintw(ui->register_win, 4, 2, "Password:");
    mvwprintw(ui->register_win, 6, 2, "Confirm:");
    
    curs_set(1);
    pos_form_cursor(form);
    wrefresh(ui->register_win);

    int ch;
    keypad(ui->register_win, TRUE);
    while((ch = wgetch(ui->register_win)) != KEY_RIGHT && ch != KEY_LEFT) {
        switch(ch) {
            case KEY_DOWN:
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                current_field = (current_field + 1) % 3;
                break;

            case KEY_UP:
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                current_field = (current_field - 1 + 3) % 3;
                break;

            case KEY_BACKSPACE:
            case 127:
                if (current_field == 1 && password_len > 0) {
                    password_len--;
                    password_buffer[password_len] = '\0';
                    set_field_buffer(fields[1], 0, "");
                    for(int i = 0; i < password_len; i++) {
                        form_driver(form, '*');
                    }
                } else if (current_field == 2 && confirm_len > 0) {
                    confirm_len--;
                    confirm_buffer[confirm_len] = '\0';
                    set_field_buffer(fields[2], 0, "");
                    for(int i = 0; i < confirm_len; i++) {
                        form_driver(form, '*');
                    }
                } else {
                    form_driver(form, REQ_DEL_PREV);
                }
                break;

            default:
                if (ch >= 32 && ch <= 126) {  // Printable characters
                    if (current_field == 1 && password_len < 30) {
                        password_buffer[password_len++] = ch;
                        password_buffer[password_len] = '\0';
                        form_driver(form, '*');
                    } else if (current_field == 2 && confirm_len < 30) {
                        confirm_buffer[confirm_len++] = ch;
                        confirm_buffer[confirm_len] = '\0';
                        form_driver(form, '*');
                    } else {
                        form_driver(form, ch);
                    }
                }
                break;
        }
        wrefresh(ui->register_win);
    }

    curs_set(0);

    set_button(ui);

    // Free resources
    unpost_form(form);
    free_form(form);
    for(int i = 0; i < 3; i++)
        free_field(fields[i]);

    ungetch(ch);
}

void login_menu(ATM_UI* ui) {
    FIELD *fields[3];
    FORM *form;
    char password_buffer[31] = {0};  // Store actual password
    int password_len = 0;
    int current_field = 0;

    mvwprintw(ui->main_win, LINES / 2 + 6, COLS / 2 + 21, "[ Next ]");
    mvwprintw(ui->main_win, LINES / 2 + 6, COLS / 2 - 29, "[ Back ]");
    wrefresh(ui->main_win);

    ui->register_win = derwin(ui->main_win, 10, 60, LINES / 2 - 5, COLS / 2 - 30);
    box(ui->register_win, 0, 0);
    wbkgd(ui->register_win, COLOR_PAIR(HEADER_COLOR_PAIR));

    fields[0] = new_field(1, 30, 2, 12, 0, 0);  // Username
    fields[1] = new_field(1, 30, 5, 12, 0, 0);  // Password
    fields[2] = NULL;

    for(int i = 0; i < 2; i++) {
        //set_field_back(fields[i], A_UNDERLINE);
        field_opts_off(fields[i], O_AUTOSKIP);
        set_field_opts(fields[i], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    }

    form = new_form(fields);
    set_form_win(form, ui->register_win);
    set_form_sub(form, derwin(ui->register_win, 8, 58, 1, 1));
    post_form(form);

    mvwprintw(ui->register_win, 0, 2, "Login");
    mvwprintw(ui->register_win, 3, 2, "Username:");
    mvwprintw(ui->register_win, 6, 2, "Password:");
    
    curs_set(1);
    pos_form_cursor(form);
    wrefresh(ui->register_win);


    int ch;
    keypad(ui->register_win, TRUE);
    while((ch = wgetch(ui->register_win)) != KEY_RIGHT && ch != KEY_LEFT) {
        switch(ch) {
            case KEY_DOWN:
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                current_field = (current_field + 1) % 2;
                break;

            case KEY_UP:
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                current_field = (current_field - 1 + 2) % 2;
                break;

            case KEY_BACKSPACE:
            case 127:
                if (current_field == 1 && password_len > 0) {
                    password_len--;
                    password_buffer[password_len] = '\0';
                    set_field_buffer(fields[1], 0, "");
                    for(int i = 0; i < password_len; i++) {
                        form_driver(form, '*');
                    }
                } else {
                    form_driver(form, REQ_DEL_PREV);
                }
                break;

            default:
                if (ch >= 32 && ch <= 126) {  // Printable characters
                    if (current_field == 1 && password_len < 30) {
                        password_buffer[password_len++] = ch;
                        password_buffer[password_len] = '\0';
                        form_driver(form, '*');
                    } else {
                        form_driver(form, ch);
                    }
                }
                break;
        }
        wrefresh(ui->register_win);
    }

    curs_set(0);

    set_button(ui);

    // Free resources
    unpost_form(form);
    free_form(form);
    for(int i = 0; i < 2; i++)
        free_field(fields[i]);

    ungetch(ch);
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
            //set_button(ui);
            break;
        case 0:
            login_menu(ui);
            //set_button(ui);
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
