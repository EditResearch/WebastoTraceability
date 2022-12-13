#ifndef _MODEL_H_
#define _MODEL_H_ 

#include <time.h>
#include <sqlite3.h>
#include <log.h>


typedef struct
{
    size_t length;
    void ** array;
}Vector;


typedef struct
{
    time_t logout_timer;
    sqlite3 * db;
    Log * log;
    
    char logged_user[64];
}Model;

#define Model(...)(Model){__VA_LIST__}


typedef enum
{
    ModelError_SQLite
}ModelError;


typedef struct
{
    enum
    {   
        E_Model_L
        , E_Model_R
    }either;

    union
    {
        ModelError left;
        Model      right;
    };
}E_Model;

#define E_Model_Right(T)(E_Model){.either = E_Model_R, .right = T}
#define E_Model_Left(T)(E_Model){.either = E_Model_L, .left = T}



E_Model
model_init(void);


Vector *
model_get_table_list(Model * self);


Vector *
model_get_table_columns(Model * self, char * table);

void
model_finalize(Model * self);

#endif
