#ifndef DB_H
#define DB_H

#include <sqlite3.h>

int init_db(const char *db_file);

void db_close();

int db_execute(const char *sql, ...);

int db_query(const char *sql, int (*callback)(void *arg, int argc, char **argv, char **col_names), void *arg, ...);

#endif // DB_H
