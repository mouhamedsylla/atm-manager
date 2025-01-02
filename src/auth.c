#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <db.h>
#include <auth.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>

int create_user(const char *username, const char *password) {
    int rc = db_execute(SQL_INSERT_USER, username, password, NULL);
    if (rc != 0) {
        fprintf(stderr, "Error creating user\n");
    }
    return rc;
}


int get_user_callback(void *data, int argc, char **argv, char **azColName) {
    User *user = (User *)data;

    user->user_id = atoi(azColName[0]);
    
    strncpy(user->username, azColName[1], sizeof(user->username) - 1);
    user->username[sizeof(user->username) - 1] = '\0';
    
    strncpy(user->password, azColName[2], sizeof(user->password) - 1);
    user->password[sizeof(user->password) - 1] = '\0';
    
    struct tm tm = {0};
    if (strptime(azColName[3], "%Y-%m-%d %H:%M:%S", &tm) != NULL) {
        user->created_at = mktime(&tm);
    } else {
        user->created_at = time(NULL);
    }

    return SQLITE_OK;
}

int get_user_by_username(const char *username, User *user) {
    int rc = db_query(SQL_SELECT_USER_BY_USERNAME, get_user_callback, user, username, NULL);

    if (rc != SQLITE_DONE) {
        return -1;  
    }
    return 0;
}

bool login_user(LoginData *data, char* error_msg) {
    User user;

    if (get_user_by_username(data->username, &user) != SQLITE_OK) {
        strcpy(error_msg, "User not found");
        return false;
    }

    if (strcmp(user.password, data->password) != 0) {
        strcpy(error_msg, "Invalid password");
        return false;
    }

    return true;
}

bool register_user(RegisterData *data, char* error_msg) {
    if (create_user(data->username, data->password) != 0) {
        strcpy(error_msg, "Error creating user");
        return false;
    }
    return true;
}

bool validate_login_data(LoginData* data, char* error_msg) {
    if (strlen(data->username) < 3) {
        strcpy(error_msg, "Username must be at least 3 characters long");
        return false;
    }
    if (strlen(data->password) < 6) {
        strcpy(error_msg, "Password must be at least 6 characters long");
        return false;
    }
    return true;
}

bool validate_register_data(RegisterData* data, char* error_msg) {
    if (strlen(data->username) < 3) {
        strcpy(error_msg, "Username must be at least 3 characters long");
        return false;
    }
    if (strlen(data->password) < 6) {
        strcpy(error_msg, "Password must be at least 6 characters long");
        return false;
    }
    if (strcmp(data->password, data->confirm_password) != 0) {
        strcpy(error_msg, "Passwords do not match");
        return false;
    }
    return true;
}
