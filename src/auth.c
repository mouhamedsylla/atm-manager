#include <stdio.h>
#include <db.h>
#include <auth.h>
#include <string.h>

int create_user(const char *username, const char *password) {
    int rc = db_execute(SQL_INSERT_USER, username, password, NULL);
    if (rc != 0) {
        fprintf(stderr, "Error creating user\n");
    }
    return rc;
}

int get_user_by_username(const char *username, User *user) {
    return -1;
}

int login(const char *username, const char *password) {
    User user;
    if (get_user_by_username(username, &user) != 0) {
        return -1;
    }

    if (strcmp(user.password, password) != 0) {
        return -1;
    }

    return 0;
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