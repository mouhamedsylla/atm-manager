#ifndef DB_H
#define DB_H

#include <sqlite3.h>

int init_db(const char *db_file);

void db_close();

int db_execute(const char *sql, ...);

#endif // DB_H
