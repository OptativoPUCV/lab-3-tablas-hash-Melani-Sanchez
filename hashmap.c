#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if (map->size >= map->capacity) enlarge(map);
    
    long pos = hash(key, map->capacity);

    while(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL){
        if(is_equal(map->buckets[pos]->key, key)) {
            map->buckets[pos]->value = value;
            return;
        }
        pos = (pos + 1) % map->capacity;
    }
    
    Pair * new = createPair(key, value);
    map->buckets[pos] = new;
    map->size++;
    map->current = pos;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair **old_buckets = map->buckets;
    long old_capacity = map->capacity;
    int old_size = map->size;

    map->capacity *= 2;
    map->buckets = (Pair **)malloc(sizeof(Pair *) * map->capacity);
    map->size = 0;

    for(int i = 0; i < map->capacity; i++) {
        map->buckets[i] = NULL;
    }

    for(int i = 0; i < old_capacity; i++) {
        if(old_buckets[i] != NULL && old_buckets[i]->key != NULL) {
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
        }
    }

    free(old_buckets[i]);
}


HashMap * createMap(long capacity) {
    HashMap * new = (HashMap *)malloc(sizeof(HashMap));
    new->buckets = (Pair **)malloc(sizeof(Pair *) * capacity);
    new->capacity = capacity;
    new->size = 0;
    new->current = -1;
    
    for(int i = 0; i < capacity; i++){
        new->buckets[i] = NULL;
    }

    return NULL;
}

void eraseMap(HashMap * map,  char * key) {    
    long pos = hash(key, map->capacity);
    long original_pos = pos;

    while(map->buckets[pos] != NULL) {
        if(map->buckets[pos]->key != NULL && is_equal(map->buckets[pos]->key, key)) {
            map->buckets[pos]->key = NULL;
            map->size--;
            return;
        }
        pos = (pos + 1) % map->capacity;
        if(pos == original_pos) break;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    long pos = hash(key, map->capacity);
    long original_pos = pos;
    
    while(map->buckets[pos] != NULL) {
        if(map->buckets[pos]->key != NULL && is_equal(map->buckets[pos]->key, key)) {
            map->current = pos;
            return map->buckets[pos];
        }
        pos = (pos + 1) % map->capacity;
        if(pos == original_pos) break;
    }
    
    return NULL;
}

Pair * firstMap(HashMap * map) {
    for(int i = 0; i < map->capacity; i++) {
        if(map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    int inicio = (map->current + 1) % map->capacity;
    int i = inicio;
    
    while (1) {
        if(map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
        i = (i + 1) % map->capacity;
        if (i == inicio) break;
    }

    return NULL;
}
