#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


E_Model
model_init(void)
{
    Model m;

    FILE * log_stream = fopen("log/")

    m.log = log_new(2, );
    int rc = sqlite3_open("db/database.db", &m.db);

    if(rc != SQLITE_OK)
        return E_Model_Left(ModelError_SQLite);
  
    return E_Model_Right(m);
}


Vector *
model_get_table_list(Model * self)
{
    char * sql = "SELECT name FROM sqlite_schema WHERE type ='table' AND name NOT LIKE 'sqlite_%';";
    sqlite3_stmt *res;
    Vector * vector = malloc(sizeof(Vector));

    vector->length = 0;

    int rc = sqlite3_prepare_v2(self->db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) 
        sqlite3_bind_int(res, 1, 3);
    else     
        return 0;
    
    while(sqlite3_step(res) == SQLITE_ROW)
    {
        char * s = (char*) sqlite3_column_text(res, 0);
        
        if(s != NULL)
        {
            vector->array = realloc(vector->array, sizeof(char*) * (vector->length + 1));
            
            vector->array[vector->length] = malloc(sizeof(strlen(s)));
            strcpy(vector->array[vector->length], s);

            vector->length++;
        }
    } 

    sqlite3_finalize(res);

    return vector;
}

void
model_finalize(Model * self)
{
    sqlite3_close(self->db);
}



