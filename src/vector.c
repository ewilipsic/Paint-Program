#include"things.h"

vector GiveVector(){
    Vector2 *arr = (Vector2*)malloc(10*sizeof(Vector2));
    vector ret = {arr,0,10};
    return ret;
}

void Vector_Grow50(vector* vec,int index){
    Vector2* temp =  (Vector2*)malloc((((index+1)/50 + 1)*50)*sizeof(Vector2));
    if(temp == NULL){
        printf("cbbb\n");
        fflush(stdout);
    }
    for(int i = 0;i<vec->len;i++){
        temp[i]= (vec->arr)[i];
    }
    vec->cap = ((index+1)/50 + 1)*50;
    free((vec->arr));
    vec->arr = temp;
}

void Vector_Add50(vector* vec,Vector2 point,int index){
    if(index + 1>vec->cap){
        Vector_Grow50(vec,index);
    }
    (vec->arr)[index] = point;
    vec->len = index + 1;
}

void Vector_Add50e(vector* vec,Vector2 point){

    if(vec->len + 1>vec->cap){
        Vector_Grow50(vec,vec->len);
    }

    (vec->arr)[vec->len] = point;
    vec->len = vec->len + 1;

}

void Vector_Grow(vector* vec,int index){
    Vector2* temp =  (Vector2*)malloc((index + 1)*sizeof(Vector2));
    for(int i = 0;i<vec->len;i++){
        temp[i]= (vec->arr)[i];
    }
    vec->cap = index + 1;
    free((vec->arr));
    vec->arr = temp;
}

void Vector_Add(vector* vec,Vector2 point,int index){
    if(index + 1>vec->cap){
        Vector_Grow(vec,index);
    }
    (vec->arr)[index] = point;
    vec->len = index + 1;
}

Vector2 Vector_pop(vector* vec){
    vec->len = vec->len - 1;
    return (vec->arr)[vec->len];
}

void Vector_Empty(vector* vec){
    vector temp = GiveVector();
    free(vec->arr);
    *vec = temp;
}

VectorRing GiveVectorRing(){
    Ring *arr = (Ring*)malloc(10*sizeof(Ring));
    VectorRing ret = {arr,0,10};
    return ret;
}

void VectorRing_Grow50(VectorRing* vec,int index){
    Ring* temp =  (Ring*)malloc((((index+1)/50 + 1)*50)*sizeof(Ring));
    if(temp == NULL){
        printf("cbbb\n");
        fflush(stdout);
    }
    for(int i = 0;i<vec->len;i++){
        temp[i]= (vec->arr)[i];
    }
    vec->cap = ((index+1)/50 + 1)*50;
    free((vec->arr));
    vec->arr = temp;
}

void VectorRing_Add50(VectorRing* vec,Ring point,int index){
    if(index + 1>vec->cap){
        VectorRing_Grow50(vec,index);
    }
    (vec->arr)[index] = point;
    vec->len = index + 1;
}

void VectorRing_Add50e(VectorRing* vec,Ring point){

    if(vec->len + 1>vec->cap){
        VectorRing_Grow50(vec,vec->len);
    }

    (vec->arr)[vec->len] = point;
    vec->len = vec->len + 1;

}

void VectorRing_Grow(VectorRing* vec,int index){
    Ring* temp =  (Ring*)malloc((index + 1)*sizeof(Ring));
    for(int i = 0;i<vec->len;i++){
        temp[i]= (vec->arr)[i];
    }
    vec->cap = index + 1;
    free((vec->arr));
    vec->arr = temp;
}

void VectorRing_Add(VectorRing* vec,Ring point,int index){
    if(index + 1>vec->cap){
        VectorRing_Grow(vec,index);
    }
    (vec->arr)[index] = point;
    vec->len = index + 1;
}

Ring VectorRing_pop(VectorRing* vec){
    if(vec->len>0){
    vec->len = vec->len - 1;
    return (vec->arr)[vec->len];
    }
    Ring ret;
    return ret;
}

void VectorRing_Empty(VectorRing* vec){
    if(vec->len == 0){
    VectorRing temp = GiveVectorRing();
    free(vec->arr);
    *vec = temp;
    }
}

void VectorRing_Insert(VectorRing* vec,Ring ring,int index){
    if(index >= vec->cap){
        VectorRing_Grow(vec,index);
    }
    if(1+vec->len<vec->cap){
        VectorRing_Grow(vec,vec->len);
    }
    for(int i = vec->len;i>index;i--){
        vec->arr[i] = vec->arr[i-1];
    }
    vec->arr[index] = ring;
    vec->len = vec->len + 1;
}