#ifndef ATM_MENUS_H
#define ATM_MENUS_H

#include <ncurses.h>
#include <form.h>

// Structure pour le menu principal
typedef struct {
    char **items;
    int num_items;
    int current_item;
    WINDOW *window;
} MainMenu;

// Structure pour le menu utilisateur
typedef struct {
    char **items;
    int num_items;
    int current_item;
    WINDOW *window;
} UserMenu;

// Structure pour les formulaires (login, register, account)
typedef struct {
    FIELD **fields;
    FORM *form;
    int num_fields;
    int num_labels;
    WINDOW *window;
    char **labels;      // Textes des labels
    char **buffers;     // Buffers pour les données saisies
    int current_field;  // Champ actuellement sélectionné
} FormData;

// Structure pour les listes (comptes, transactions)
typedef struct {
    char **items;
    int num_items;
    int current_item;
    int start_index;    // Pour le scrolling
    WINDOW *window;
    WINDOW *detail_window;  // Fenêtre de détails
} ListData;

// Structure pour les transactions
typedef struct {
    FormData form;      // Formulaire de transaction
    ListData accounts;  // Liste des comptes
} TransactionData;

#endif