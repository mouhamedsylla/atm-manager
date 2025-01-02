#include "ui/windows.h"
#include "db.h"


int main() {
    ATM_UI* ui = init_ui();

        // Initialisation de la base de données
    if (init_db("../data/atm.db") != 0) {
        while ((getch()) != 'q')
        {
            /* code */
        }
        
        cleanup_ui(ui);
        return 1;
    }

    create_main_menu(ui);
    handle_main_menu_input(ui);

    cleanup_ui(ui);
    return 0;
}