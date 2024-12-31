#include "ui/windows.h"
#include "ui/colors.h"
#include <string.h>
#include <stdlib.h>

int yMax, xMax;

ATM_UI* init_ui() {
    ATM_UI* ui = malloc(sizeof(ATM_UI));
    if (!ui) return NULL;

    // Initialize ncurses
    initscr();
    getmaxyx(stdscr, yMax, xMax);
    curs_set(0);
    start_color();
    cbreak();
    noecho();

    // Initialize color pairs
    init_color_pairs();

    // Main window
    ui->window = newwin(LINES, COLS, 0, 0);
    box(ui->window, 0, 0);
    wbkgd(ui->window, COLOR_PAIR(MAIN_COLOR_PAIR));

    // Header
    ui->header = derwin(ui->window, 1, COLS - 4, 1, 2);
    wbkgd(ui->header, COLOR_PAIR(WINDOW_COLOR_PAIR));
    mvwprintw(ui->header, 0, xMax / 2 - 10, "ATM MANAGEMENT SYSTEM");

    // Status bar
    ui->status_bar.window = derwin(ui->window, 1, COLS - 4, LINES - 2, 2);
    wbkgd(ui->status_bar.window, COLOR_PAIR(WINDOW_COLOR_PAIR));
    update_connection_status(ui, DISCONNECTED, xMax - 18);

    refresh();
    wrefresh(ui->window);
    return ui;
}

void set_content(ATM_UI* ui, ContentType type, void* data) {
    switch (type)
    {
    case CONTENT_MAIN_MENU:
        ui->content.type = CONTENT_MAIN_MENU;
        ui->content.data.main_menu = (MainMenu*)data;
        break;

    case CONTENT_LOGIN_FORM:
        ui->content.type = CONTENT_LOGIN_FORM;
        ui->content.data.form = (FormData*)data;
        break;

    case CONTENT_REGISTER_FORM:
        ui->content.type = CONTENT_REGISTER_FORM;
        ui->content.data.form = (FormData*)data;
        break;
    
    default:
        break;
    }
}

void update_status(ATM_UI* ui, StatusType type, const char* message) {
    ui->status_bar.type = type;
    wclear(ui->status_bar.window);
    strncpy(ui->status_bar.message, message, sizeof(ui->status_bar.message));
    switch (type)   
    {
    case STATUS_INFO:
        wattron(ui->status_bar.window, A_BOLD);
        wattron(ui->status_bar.window, COLOR_PAIR(INFO_COLOR_PAIR) | A_REVERSE);
        mvwprintw(ui->status_bar.window, 0, 2, "  INFOS  ");
        wattroff(ui->status_bar.window, COLOR_PAIR(INFO_COLOR_PAIR) | A_REVERSE);
        wattroff(ui->status_bar.window, A_BOLD);
        break;

    case STATUS_SUCCESS:
        wattron(ui->status_bar.window, A_BOLD);
        wattron(ui->status_bar.window, COLOR_PAIR(SUCCESS_COLOR_PAIR) | A_REVERSE);
        mvwprintw(ui->status_bar.window, 0, 2, " SUCCESS ");
        wattroff(ui->status_bar.window, COLOR_PAIR(SUCCESS_COLOR_PAIR) | A_REVERSE);
        wattroff(ui->status_bar.window, A_BOLD);
        break;

    case STATUS_ERROR:
        wattron(ui->status_bar.window, A_BOLD);
        wattron(ui->status_bar.window, COLOR_PAIR(ERROR_COLOR_PAIR) | A_REVERSE);
        mvwprintw(ui->status_bar.window, 0, 2, "  ERROR  ");
        wattroff(ui->status_bar.window, COLOR_PAIR(ERROR_COLOR_PAIR) | A_REVERSE);
        wattroff(ui->status_bar.window, A_BOLD);
        break;

    case STATUS_WARN:
        wattron(ui->status_bar.window, A_BOLD);
        wattron(ui->status_bar.window, COLOR_PAIR(WARNING_COLOR_PAIR) | A_REVERSE);
        mvwprintw(ui->status_bar.window, 0, 2, " WARNING ");
        wattroff(ui->status_bar.window, COLOR_PAIR(WARNING_COLOR_PAIR) | A_REVERSE);
        wattroff(ui->status_bar.window, A_BOLD);
        break;
    default:
        break;
    }

    mvwprintw(ui->status_bar.window, 0, 12, "%s", message);
    wrefresh(ui->status_bar.window);
}

void update_connection_status(ATM_UI* ui, ConnectionStatus status, int x) {
    ui->status_bar.connection_status = status;
   // wclear(ui->status_bar.window);
   // wattron(ui->status_bar.window, COLOR_PAIR(INFO_COLOR_PAIR));
    mvwprintw(ui->status_bar.window, 0, x, "%s", status == CONNECTED ? "   Connected" : "Disconnected");
    //wattroff(ui->status_bar.window, COLOR_PAIR(INFO_COLOR_PAIR));
    wrefresh(ui->status_bar.window);
}

void cleanup_ui(ATM_UI* ui) {
    delwin(ui->status_bar.window);
    delwin(ui->header);
    delwin(ui->window);
    free(ui);
    endwin();
}

void cleanup_menu(MainMenu* menu) {
    delwin(menu->window);
    free(menu);
}

void cleanup_form(FormData* form) {
    unpost_form(form->form);
    free_form(form->form);
    for (int i = 0; i < form->num_fields; i++)
    {
        free_field(form->fields[i]);
    }
    free(form->fields);
    free(form->labels);
    free(form->buffers);
    delwin(form->window);
    free(form);
}