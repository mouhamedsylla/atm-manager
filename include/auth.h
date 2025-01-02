#ifndef AUTH_H
#define AUTH_H

#include <stdint.h>
#include <time.h>
#include <stdbool.h>

/**
 * User structure.
 * This structure represents a user in the database.
 * @field user_id The user's ID.
 * @field username The user's username.
 * @field password The user's password.
 * @field created_at The date and time the user was created.
 */
typedef struct {
    int32_t user_id;
    char username[51];
    char password[256];
    time_t created_at;
} User;

/**
 * Login data structure.
 * This structure represents the data entered by the user during login.
 * @field username The username entered by the user.
 * @field password The password entered by the user.
 */
typedef struct {
    char username[51];
    char password[256];
} LoginData;


/**
 * Register data structure.
 * This structure represents the data entered by the user during registration.
 * @field username The username entered by the user.
 * @field password The password entered by the user.
 * @field confirm The confirmation password entered by the user.
 */
typedef struct {
    char username[51];
    char password[256];
    char confirm_password[256];
} RegisterData;

/**
 * Create the users table in the database.
 */
#define SQL_CREATE_USERS_TABLE \
    "CREATE TABLE IF NOT EXISTS users ("\
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "\
    "username TEXT NOT NULL UNIQUE, "\
    "password TEXT NOT NULL, "\
    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "\
    "CONSTRAINT unique_username CHECK (username IS NOT NOT NULL AND trim(username) != ''));"

/**
 * SQL statement to insert a new user into the database.
 */
#define SQL_INSERT_USER \
    "INSERT INTO users (username, password) VALUES (?, ?);"

/**
 * SQL statement to select a user by username.
 */
#define SQL_SELECT_USER_BY_USERNAME \
    "SELECT id, username, password, created_at FROM users WHERE username = ?;"


/**
 * Insert a new user into the database.
 * @param username The username of the user.
 * @param password The password of the user.
 */
int create_user(const char *username, const char *password);

/**
 * Retrieve a user from the database by username.
 * @param username The username of the user.
 * @param user The user structure to populate.
 */
int get_user_by_username(const char *username, User *user);

/**
 * Authenticate a user by username and password.
 * @param username The username of the user.
 * @param password The password of the user.
 * @return 0 if the user is authenticated, -1 otherwise.
 */
bool login_user(LoginData *data, char* error_msg);


/**
 * Register a new user.
 * @param data The registration data
 * @param error_msg The error message to populate.
 */
bool register_user(RegisterData *data, char* error_msg);

/**
 * Validate login data.
 * @param data The login data to validate.
 * @param error_msg The error message to populate.
 * @return true if the data is valid, false otherwise.
 */
bool validate_login_data(LoginData* data, char* error_msg);

/**
 * Validate register data.
 * @param data The register data to validate.
 * @param error_msg The error message to populate.
 * @return true if the data is valid, false otherwise.
 */
bool validate_register_data(RegisterData* data, char* error_msg);

#endif // AUTH_H
