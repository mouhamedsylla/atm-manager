#include <stdio.h>
#include <stdarg.h>
#include <auth.h> 
#include <db.h>

static sqlite3 *db = NULL;

int init_db(const char *db_file) {
    if (sqlite3_open(db_file, &db) != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    if (db_execute(SQL_CREATE_USERS_TABLE, NULL) != 0) {
        fprintf(stderr, "Error creating users table\n");
        return -1;
    }
    return 0;
}


void db_close() {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}

int db_execute(const char *sql, ...) {
    sqlite3_stmt *stmt = NULL;
    va_list args;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    va_start(args, sql);
    int param_index = 1;
    const char *param;
    while ((param = va_arg(args, const char *)) != NULL) {
        if (sqlite3_bind_text(stmt, param_index, param, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            fprintf(stderr, "Error binding parameter: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            va_end(args);
            return -1;
        }
        param_index++;
    }
    va_end(args);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    return 0;
}

