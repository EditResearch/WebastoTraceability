#ifndef _MODEL_H_
#define _MODEL_H_ 

#include <time.h>

typedef struct
{
    time_t timeout_logout;
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

#endif
