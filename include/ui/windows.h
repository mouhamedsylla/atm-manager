#ifndef ATM_WINDOWS_H
#define ATM_WINDOWS_H

#include "ui/menus.h"

/**
 * Status types.
 * This enumeration represents the different types of status messages that can be displayed in the UI.
 */
typedef enum {
    STATUS_INFO,
    STATUS_WARN,
    STATUS_ERROR,
    STATUS_SUCCESS
} StatusType;


/**
 * Connection status.
 * This enumeration represents the different connection statuses that can be displayed in the UI.
 */
typedef enum {
    CONNECTED,
    DISCONNECTED
} ConnectionStatus;


/**
 * Content types.
 * This enumeration represents the different types of content that can be displayed in the UI.
 */
typedef enum {
    CONTENT_MAIN_MENU,
    CONTENT_LOGIN_FORM,
    CONTENT_REGISTER_FORM,
    CONTENT_USER_MENU,
    CONTENT_ACCOUNT_FORM,
    CONTENT_TRANSACTION_FORM,
    CONTENT_ACCOUNT_LIST,
} ContentType;



/**
 * Main menu structure.
 * This structure represents the main menu of the application.
 * @field items The menu items.
 * @field num_items The number of menu items.
 * @field current_item The index of the currently selected item.
 * @field window The window of the menu.
 */
typedef struct {
    WINDOW *window;      
    ContentType type;
    union {
        MainMenu *main_menu;        
        UserMenu *user_menu;        
        FormData *form;             
        ListData *list;             
        TransactionData *transaction;
    } data;
} Content;


/**
 * Status bar structure.
 * This structure represents the status bar at the bottom of the UI.
 * @field window The window of the status bar
 * @field type The type of message currently displayed
 * @field message The message currently displayed
 * @field connection_status The connection status
 */
typedef struct {
    WINDOW *window;           
    StatusType type;        
    char message[256];       
    ConnectionStatus connection_status;  
} StatusBar;

/**
 * User interface structure.
 * This structure represents the user interface of the application.
 * @field main_window The main window of the application.
 * @field header_window The header window of the application.
 * @field content The current content displayed in the application.
 * @field message_window The message window of the application.
 */
typedef struct {
    WINDOW *window;    
    WINDOW *header;  
    Content content;        
    StatusBar status_bar;
} ATM_UI;


/**
 * Initialise the user interface.
 * @return The user interface structure.
 */
ATM_UI* init_ui(void);

/**
 * Change the content displayed in the UI.
 * @param ui The user interface structure.
 * @param type The type of content to display.
 * @param data The content-specific data.
 */
void set_content(ATM_UI* ui, ContentType type, void* data);

/**
 * Update the status bar message
 * @param ui The user interface structure
 * @param type Type of message (INFO, WARN, ERROR)
 * @param message The message to display
 */
void update_status(ATM_UI* ui, StatusType type, const char* message);

/**
 * Update the connection status
 * @param ui The user interface structure
 * @param status New connection status
 */
void update_connection_status(ATM_UI* ui, ConnectionStatus status, int x);

/**
 * Refresh the status bar
 * @param ui The user interface structure
 */
void refresh_status_bar(ATM_UI* ui);

void cleanup_ui(ATM_UI* ui);

// Functions de création des menus
void create_main_menu(ATM_UI* ui);
void create_login_form(ATM_UI* ui);
void create_register_form(ATM_UI* ui);
void create_user_menu(ATM_UI* ui);
void create_account_form(ATM_UI* ui);
void create_transaction_form(ATM_UI* ui);
void create_account_list(ATM_UI* ui);

// Functions de gestion des menus
void handle_main_menu_input(ATM_UI* ui);
void handle_login_form_input(ATM_UI* ui);
void handle_register_form_input(ATM_UI* ui);
void handle_user_menu_input(ATM_UI* ui);
void handle_list_input(ATM_UI* ui);
int active_button_switch(ATM_UI* ui);

// Display functions
void show_main_menu(ATM_UI* ui);
void show_login_form(ATM_UI* ui);
void show_register_form(ATM_UI* ui);

// Functions de nettoyage
void cleanup_menu(MainMenu* menu_data);
void cleanup_form(FormData* form_data);
void cleanup_list(ListData* list_data);


#endif