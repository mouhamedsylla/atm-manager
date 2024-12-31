// 1. Header files
#include "ui/colors.h"
#include "ui/menus.h"
#include "ui/windows.h"
#include "auth.h"
#include <stdlib.h>
#include <string.h>

// 2. Constants & Types definitions
static const char* MAIN_MENU_ITEMS[] = {
    "Login",
    "Register",
    "Exit"
};
static const int MAIN_MENU_ITEMS_COUNT = 3;


// 6. Menu creation functions implementation
void create_main_menu(ATM_UI* ui) {
    MainMenu* menu = malloc(sizeof(MainMenu));
    menu->items = (char**)MAIN_MENU_ITEMS;
    menu->num_items = MAIN_MENU_ITEMS_COUNT;
    menu->current_item = 0;

    int yMax, xMax;
    getmaxyx(ui->window, yMax, xMax);
    
    menu->window = derwin(ui->window,
                         10,                    
                         40,                    
                         yMax/2 - 5,           
                         xMax/2 - 20);
    
    box(menu->window, 0, 0);
    wbkgd(menu->window, COLOR_PAIR(MENU_COLOR_PAIR));
    keypad(menu->window, TRUE);

    set_content(ui, CONTENT_MAIN_MENU, menu);
}

// Création du formulaire de connexion
void create_login_form(ATM_UI* ui) {
    FormData* form = malloc(sizeof(FormData));
    form->num_fields = 3;
    form->num_labels = 3;
    form->fields = (FIELD**)malloc(sizeof(FIELD*) * form->num_fields);
    form->labels = (char**)malloc(sizeof(char*) * form->num_fields);
    form->buffers = (char**)malloc(sizeof(char*) * form->num_fields);
    form->current_field = 0;
    
    // Création des champs
    form->fields[0] = new_field(1, 30, 2, 12, 0, 0);
    form->fields[1] = new_field(1, 30, 5, 12, 0, 0);
    form->fields[2] = NULL;
    
    // Configuration des champs
    for (int i = 0; i < form->num_fields - 1; i++) {
        field_opts_off(form->fields[i], O_AUTOSKIP);
        set_field_opts(form->fields[i], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    }
    
    // Labels
    form->labels[0] = " LOGIN ";
    form->labels[1] = "Username: ";
    form->labels[2] = "Password: ";
    
    // Buffers
    form->buffers[0] = (char*)malloc(31);
    form->buffers[1] = (char*)malloc(31);
    
    set_content(ui, CONTENT_LOGIN_FORM, form);
    handle_login_form_input(ui);
}

void create_register_form(ATM_UI* ui) {
    FormData* form = malloc(sizeof(FormData));
    form->num_fields = 4;  // username, password, confirm, NULL
    form->num_labels = 4;  // titre + 3 labels
    form->fields = (FIELD**)malloc(sizeof(FIELD*) * form->num_fields);
    form->labels = (char**)malloc(sizeof(char*) * form->num_labels);
    form->buffers = (char**)malloc(sizeof(char*) * (form->num_fields - 1));
    form->current_field = 0;
    
    // Création des champs
    form->fields[0] = new_field(1, 30, 2, 12, 0, 0);  // Username
    form->fields[1] = new_field(1, 30, 4, 12, 0, 0);  // Password
    form->fields[2] = new_field(1, 30, 6, 12, 0, 0);  // Confirm
    form->fields[3] = NULL;
    
    // Configuration des champs
    for (int i = 0; i < form->num_fields - 1; i++) {
        field_opts_off(form->fields[i], O_AUTOSKIP);
        set_field_opts(form->fields[i], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    }
    
    // Labels
    form->labels[0] = " REGISTER ";
    form->labels[1] = "Username: ";
    form->labels[2] = "Password: ";
    form->labels[3] = "Confirm:  ";
    
    // Buffers pour les données
    form->buffers[0] = (char*)malloc(31);  // username
    form->buffers[1] = (char*)malloc(31);  // password
    form->buffers[2] = (char*)malloc(31);  // confirm
    
    set_content(ui, CONTENT_REGISTER_FORM, form);
    handle_register_form_input(ui);
}

// 7. Display functions implementation
void show_main_menu(ATM_UI* ui) {
    MainMenu* menu = ui->content.data.main_menu;
    
    // Titre du menu
    wattron(menu->window, COLOR_PAIR(MENU_COLOR_PAIR));
    mvwprintw(menu->window, 0, 2, " Main Menu ");
    wattroff(menu->window, COLOR_PAIR(MENU_COLOR_PAIR));
    
    // Affichage des options
    for (int i = 0; i < menu->num_items; i++) {
        if (i == menu->current_item) {
            wattron(menu->window, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
            mvwprintw(menu->window, i*2 + 2, 2, "> %s", menu->items[i]);
            wattroff(menu->window, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
        } else {
            mvwprintw(menu->window, i*2 + 2, 2, "  %s", menu->items[i]);
        }
    }
    
    // Mise à jour de la barre d'état
    update_status(ui, STATUS_INFO, "Use UP/DOWN arrows to navigate, ENTER to select");
    
    // Rafraîchissement de la fenêtre
    wrefresh(menu->window);
}

void show_login_form(ATM_UI* ui) {
    FormData* form = ui->content.data.form;

    // Main window Buttons
    mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 + 21, "[ Next ]");
    mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 - 29, "[ Back ]");
    wrefresh(ui->window);

    form->window = derwin(ui->window, 10, 60, LINES / 2 - 5, COLS / 2 - 30);
    box(form->window, 0, 0);
    wbkgd(form->window, COLOR_PAIR(MENU_COLOR_PAIR));

    form->form = new_form(form->fields);
    set_form_win(form->form, form->window);
    set_form_sub(form->form, derwin(form->window, 8, 58, 1, 1));
    post_form(form->form);

    // Labels
    for(int i = 0; i < form->num_labels; i++) {
        if (i == 0) { wattron(form->window, A_BOLD); }
        mvwprintw(form->window, i * 3, 2, "%s", form->labels[i]);
        if (i == 0) { wattroff(form->window, A_BOLD);}
    }

    // Message d'information contextuel
    update_status(ui, STATUS_INFO, "Use UP/DOWN to switch fields, ENTER to validate");

    wrefresh(form->window);
}

void show_register_form(ATM_UI* ui) {
    FormData* form = ui->content.data.form;

    // Boutons
    mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 + 21, "[ Next ]");
    mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 - 29, "[ Back ]");
    wrefresh(ui->window);

    // Création de la fenêtre du formulaire
    form->window = derwin(ui->window, 12, 60, LINES / 2 - 6, COLS / 2 - 30);
    box(form->window, 0, 0);
    wbkgd(form->window, COLOR_PAIR(MENU_COLOR_PAIR));

    // Configuration du formulaire
    form->form = new_form(form->fields);
    set_form_win(form->form, form->window);
    set_form_sub(form->form, derwin(form->window, 10, 58, 1, 1));
    post_form(form->form);

    // Affichage des labels
    wattron(form->window, A_BOLD);
    mvwprintw(form->window, 0, 2, "%s", form->labels[0]);
    wattroff(form->window, A_BOLD);

    int y = 3;
    for(int i = 1; i < form->num_labels; i++) {
        mvwprintw(form->window, y, 2, "%s", form->labels[i]);
        y += 2;
    }

    // Message d'information
    update_status(ui, STATUS_INFO, "Use UP/DOWN to switch fields, ENTER to validate");

    wrefresh(form->window);
}

// 8. Input handling functions implementation
void handle_main_menu_input(ATM_UI* ui) {
    MainMenu* menu = ui->content.data.main_menu;
    int ch;
    show_main_menu(ui);
    
    while ((ch = wgetch(menu->window)) != 'q') {
        switch (ch) {
            case KEY_UP:
                if (menu->current_item > 0) {
                    menu->current_item--;
                }
                break;
                
            case KEY_DOWN:
                if (menu->current_item < menu->num_items - 1) {
                    menu->current_item++;
                }
                break;
                
            case 10: // ENTER
                if (menu->current_item == 0) {
                    create_login_form(ui);
                    return;
                } else if (menu->current_item == 1) {
                    create_register_form(ui);
                    return;
                } else if (menu->current_item == 2) {
                    cleanup_ui(ui);
                    exit(0);
                }
                return; // Sortie avec la sélection actuelle
        }
        show_main_menu(ui);
    }
}

int active_button_switch(ATM_UI* ui) {
    keypad(ui->window, TRUE);
    int highlight = 1;
    int choice;

    // Highlight initial
    wattron(ui->window, A_REVERSE);
    mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 + 21, "  Next  ");
    wattroff(ui->window, A_REVERSE);

    while (1) {
        choice = wgetch(ui->window);
        if(choice == KEY_RIGHT) {
            highlight = 1;
            update_status(ui, STATUS_INFO, "Press ENTER to continue or LEFT to go back");
            wattron(ui->window, A_REVERSE);
            mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 + 21, "  Next  ");
            wattroff(ui->window, A_REVERSE);
            mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 - 29, "[ Back ]");
            wrefresh(ui->window);
        } else if(choice == KEY_LEFT) {
            highlight = 0;
            update_status(ui, STATUS_INFO, "Press ENTER to go back or RIGHT to continue");
            wattron(ui->window, A_REVERSE);
            mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 - 29, "  Back  ");
            wattroff(ui->window, A_REVERSE);
            mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 + 21, "[ Next ]");
            wrefresh(ui->window);
        } else if(choice == 27) {
            highlight = 27;
            update_status(ui, STATUS_INFO, "Returning to form input");
            break;
        } else if(choice == 10) {
            if (highlight == 1) {
                update_status(ui, STATUS_INFO, "Processing...");
            } else {
                update_status(ui, STATUS_INFO, "Going back...");
            }
            break;
        }   
    }
    
    return highlight;
}

// Modification du handle_login_form_input
void handle_login_form_input(ATM_UI* ui) {
    FormData* form = ui->content.data.form;
    LoginData login_data;
    char error_msg[100];
    int ch;
    int password_len = 0;
    bool editing = true;

    show_login_form(ui);

    while(editing) {
        curs_set(1);
        pos_form_cursor(form->form);
        keypad(form->window, TRUE);

        while ((ch = wgetch(form->window)) != 10)
        {
            switch (ch) {
                case KEY_DOWN:
                    form_driver(form->form, REQ_NEXT_FIELD);
                    form_driver(form->form, REQ_END_LINE);
                    form->current_field = (form->current_field + 1) % 2;
                    break;

                case KEY_UP:
                    form_driver(form->form, REQ_PREV_FIELD);
                    form_driver(form->form, REQ_END_LINE);
                    form->current_field = (form->current_field - 1 + 2) % 2;
                    break;
                
                case KEY_BACKSPACE:
                case 127:
                    if (form->current_field == 1 && password_len > 0) {
                        password_len--;
                        form->buffers[1][password_len] = '\0';
                        set_field_buffer(form->fields[1], 0, "");
                        for (int i = 0; i < password_len; i++) {
                            form_driver(form->form, '*');
                        }
                    } else {
                        form_driver(form->form, REQ_DEL_PREV);
                        form_driver(form->form, REQ_END_FIELD);
                    }
                    break;
                default:
                    if (ch >= 32 && ch <= 126) {
                        if (form->current_field == 1 && password_len < 30) {
                            form->buffers[1][password_len++] = ch;
                            form->buffers[1][password_len] = '\0';
                            form_driver(form->form, '*');
                        } else {
                            form_driver(form->form, ch);
                        }
                    }
                    break;
            }
            wrefresh(form->window);
        }

        curs_set(0);

        if (ch == 10) {
            form_driver(form->form, REQ_VALIDATION);
            update_status(ui, STATUS_INFO, "Use LEFT/RIGHT to switch buttons, ENTER to select, ESC to return to form");
            
            int button_action = active_button_switch(ui);
            if(button_action == 1) {
                // Récupération des données
                strncpy(login_data.username, form->buffers[0], 30);
                strncpy(login_data.password, form->buffers[1], 30);
                
                // Validation
                if (!validate_login_data(&login_data, error_msg)) {
                    update_status(ui, STATUS_ERROR, error_msg);
                    continue;
                }

                // TODO: Appel à la fonction de login DB
                // if (db_login_user(&login_data, error_msg)) {
                //     update_status(ui, STATUS_SUCCESS, "Login successful!");
                //     editing = false;
                // } else {
                //     update_status(ui, STATUS_ERROR, error_msg);
                //     continue;
                // }
            } else if (button_action == 0) {
                // Back
                update_status(ui, STATUS_INFO, "Returning to main menu...");
                editing = false;
            } else if (button_action == 27) {
                update_status(ui, STATUS_INFO, "Back to form - Use UP/DOWN to switch fields");
                mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 + 21, "[ Next ]");
                mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 - 29, "[ Back ]");
                wrefresh(ui->window);
                continue;
            }
        } 
        else if (ch == 27) { // ESC
            update_status(ui, STATUS_INFO, "Back to form - Use UP/DOWN to switch fields");
            show_login_form(ui);
            continue;
        }
    }

    
    unpost_form(form->form);
    free_form(form->form);
    for(int i = 0; i < form->num_fields ; i++)
        free_field(form->fields[i]);

    ungetch(ch);
}

// Modification du handle_register_form_input
void handle_register_form_input(ATM_UI* ui) {
    FormData* form = ui->content.data.form;
    RegisterData register_data;
    char error_msg[100];
    int ch;
    int password_len = 0;
    int confirm_len = 0;
    bool editing = true;

    show_register_form(ui);

    while(editing) {
        curs_set(1);
        pos_form_cursor(form->form);
        keypad(form->window, TRUE);

        while ((ch = wgetch(form->window)) != 10) {
            switch (ch) {
                case KEY_DOWN:
                    form_driver(form->form, REQ_NEXT_FIELD);
                    form_driver(form->form, REQ_END_LINE);
                    form->current_field = (form->current_field + 1) % 3;
                    break;

                case KEY_UP:
                    form_driver(form->form, REQ_PREV_FIELD);
                    form_driver(form->form, REQ_END_LINE);
                    form->current_field = (form->current_field - 1 + 3) % 3;
                    break;
                
                case KEY_BACKSPACE:
                case 127:
                    if (form->current_field == 1 && password_len > 0) {
                        password_len--;
                        form->buffers[1][password_len] = '\0';
                        set_field_buffer(form->fields[1], 0, "");
                        for (int i = 0; i < password_len; i++) {
                            form_driver(form->form, '*');
                        }
                    } else if (form->current_field == 2 && confirm_len > 0) {
                        confirm_len--;
                        form->buffers[2][confirm_len] = '\0';
                        set_field_buffer(form->fields[2], 0, "");
                        for (int i = 0; i < confirm_len; i++) {
                            form_driver(form->form, '*');
                        }
                    } else {
                        form_driver(form->form, REQ_DEL_PREV);
                        form_driver(form->form, REQ_END_LINE);
                    }
                    break;

                default:
                    if (ch >= 32 && ch <= 126) {
                        if (form->current_field == 0 && strlen(form->buffers[0]) < 30) {
                            form->buffers[0][strlen(form->buffers[0])] = ch;
                            form_driver(form->form, ch);
                        } else if (form->current_field == 1 && password_len < 30) {
                            form->buffers[1][password_len++] = ch;
                            form->buffers[1][password_len] = '\0';
                            form_driver(form->form, '*');
                        } else if (form->current_field == 2 && confirm_len < 30) {
                            form->buffers[2][confirm_len++] = ch;
                            form->buffers[2][confirm_len] = '\0';
                            form_driver(form->form, '*');
                        } else {
                            form_driver(form->form, ch);
                        }
                    }
                    break;
            }
            wrefresh(form->window);
        }

        curs_set(0);
        
        if (ch == 10) {
            form_driver(form->form, REQ_VALIDATION);
            update_status(ui, STATUS_INFO, "Use LEFT/RIGHT to switch buttons, ENTER to select, ESC to return to form");
            
            int button_action = active_button_switch(ui);
            if (button_action == 1) {
                // Récupération des données
                strncpy(register_data.username, form->buffers[0], 30);
                strncpy(register_data.password, form->buffers[1], 30);
                strncpy(register_data.confirm_password, form->buffers[2], 30);

                mvwprintw(ui->window, 2, 2, "%s: %s", form->labels[0], register_data.username);
                mvwprintw(ui->window, 4, 2, "%s: %s", form->labels[1], register_data.password);
                mvwprintw(ui->window, 6, 2, "%s: %s", form->labels[2], register_data.confirm_password);
                wrefresh(ui->window);

                // Validation
                if (!validate_register_data(&register_data, error_msg)) {
                    update_status(ui, STATUS_ERROR, error_msg);
                    continue;
                }

                // TODO: Appel à la fonction d'enregistrement DB
                if (register_user(&register_data, error_msg)) {
                    update_status(ui, STATUS_SUCCESS, "Registration successful!");
                    editing = false;
                } else {
                    update_status(ui, STATUS_ERROR, error_msg);
                    continue;
                }
            } else if (button_action == 0) {
                update_status(ui, STATUS_INFO, "Returning to main menu...");
                editing = false;
            } else if (button_action == 27) {
                update_status(ui, STATUS_INFO, "Back to form - Use UP/DOWN to switch fields");
                mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 + 21, "[ Next ]");
                mvwprintw(ui->window, LINES / 2 + 6, COLS / 2 - 29, "[ Back ]");
                wrefresh(ui->window);
                continue;
            }
        }
    }

    // Nettoyage
    unpost_form(form->form);
    free_form(form->form);
    for(int i = 0; i < form->num_fields - 1; i++) {
        free_field(form->fields[i]);
        free(form->buffers[i]);
    }
    free(form->fields);
    free(form->labels);
    free(form->buffers);
    free(form);
}