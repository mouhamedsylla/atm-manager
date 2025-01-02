#include <stdio.h>
#include <stdarg.h>
#include <auth.h> 
#include <db.h>
#include <stdlib.h>
#include "ui/windows.h"

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


int db_query(const char *sql, int (*callback)(void *, int, char **, char **), void *arg, ...) {
    sqlite3_stmt *stmt = NULL;
    va_list args;

    if (!callback) {
        fprintf(stderr, "Error: callback function is NULL\n");
        return SQLITE_ERROR;
    }

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return SQLITE_ERROR;
    }

    va_start(args, arg);
    const char *param;
    int param_index = 1;
    while((param = va_arg(args, const char *)) != NULL) {
        if (sqlite3_bind_text(stmt, param_index, param, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            va_end(args);
            goto cleanup;
        }
        param_index++;
    }

    va_end(args);

        
    int col_count = sqlite3_column_count(stmt);
    int rc = SQLITE_OK;

    char **col_names = malloc(col_count * sizeof(char *));
    char **col_values = malloc(col_count * sizeof(char *));

    if (!col_names || !col_values) {
        fprintf(stderr, "Error allocating memory\n");
        rc = SQLITE_NOMEM;
        goto cleanup;
    }

    for (int i = 0; i < col_count; i++) {
        col_names[i] = (char *)sqlite3_column_name(stmt, i);
        if (!col_names[i]) {
            fprintf(stderr, "Error getting column name\n");
            rc = SQLITE_ERROR;
            goto cleanup;
        }
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        for (int i = 0; i < col_count; i++) {
            col_values[i] = (char *)sqlite3_column_text(stmt, i);      
        }

        if (callback(arg, col_count, col_names, col_values) != 0) {
            break;
        }
    }

cleanup:
    sqlite3_finalize(stmt);
    free(col_names);
    free(col_values);
    return rc;
    
}
