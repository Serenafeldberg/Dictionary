#include <stdbool.h>
#include "operable_dict.h"

/*
 * Punto extra de operable dict, suma 3 puntos como máximo.
 */

/*
 * Inserta o pisa en dictionary1 todas las claves y valores que están en dictionary2.
 * Las claves se mantienen independientes entre ambos diccionarios, pero los valores no.
 * (las claves nuevas de agregar dictionary2 en dictionary1 deben ser punteros distintos
 * pero los valores son el mismo puntero).
 * La operación es atómica, si falla dictionary1 queda en el mismo estado que estaba antes del update.
 * Devuelve true si funcionó, false si fayó.
 */

bool dict_copy (dictionary_t* dict1, dictionary_t* dict2){
    for (int i = 0; i < dict2->size; i++){
        if (dict2->table[i].key != NULL && !dict2->table[i].deleted){
            bool result = dictionary_put(dict1, dict2->table[i].key, dict2->table[i].value);
            if (!result) return false;
        }
    }
    return true;
}

bool dictionary_update(dictionary_t *dictionary1, dictionary_t *dictionary2){
    dictionary_t* dict = dictionary_create(dictionary1->destroy);
    if (!dict) return false;

    if (!dict_copy(dict, dictionary1)){
        dictionary_destroy(dict);
        return false;
    }
    if (!dict_copy(dict, dictionary2)){
        dictionary_destroy(dict);
        return false;
    }
    if (!dict_copy(dictionary1, dict)){
        dictionary_destroy(dict);
        return false;
    }

    dictionary_destroy(dict);
    return true;

}

/*
 * Combina ambos diccionarios en uno nuevo que contiene solo las claves que están presentes en ambos.
 * En todos los casos se conservan los valores del dictionary1.
 * Devuelve NULL si falla.
 */
dictionary_t* dictionary_and(dictionary_t *dictionary1, dictionary_t *dictionary2){
    dictionary_t* dict = dictionary_create(dictionary1->destroy);
    if (!dict) return NULL;


    for (size_t i = 0; i < dictionary1->size; i++){
        if (dictionary1->table[i].key != NULL && !dictionary1->table[i].deleted){
            if (dictionary_contains(dictionary2, dictionary1->table[i].key)){
                bool result = dictionary_put(dict, dictionary1->table[i].key, dictionary1->table[i].value);
                if (!result){
                    dictionary_destroy(dict);
                    return NULL;
                }
            }
        }
    }
    return dict;
}

/*
 * Combina ambos diccionarios en uno nuevo que contiene todas las claves de ambos.
 * En caso de que ambos tengan una misma clave se conserva el valor de dictionary1.
 * Devuelve NULL si falla.
 */
dictionary_t* dictionary_or(dictionary_t *dictionary1, dictionary_t *dictionary2){
    dictionary_t* dict = dictionary_create(dictionary1->destroy);
    if (!dict) return NULL;

    if (!dict_copy(dict, dictionary2)){
        dictionary_destroy(dict);
        return NULL;
    }

    if (!dict_copy(dict, dictionary1)){
        dictionary_destroy(dict);
        return NULL;
    }
    return dict;
}

/*
 * Devuelve true si ambos diccionarios son iguales. Dos diccionarios son iguales sí:
 *  - Las claves son iguales aunque puedan tener distinta posición en memoria.
 *  - Los values son los mismos punteros para cada clave
 *  - Tienen la misma cantidad de claves
 */
bool dictionary_equals(dictionary_t *dictionary1, dictionary_t *dictionary2){
    if (dictionary1->count != dictionary2->count) return false;

    for (int i = 0; i < dictionary1->size; i++){
        entry_t* entry1 = &dictionary1->table[i];
        if (entry1->key != NULL && !entry1->deleted) {
            if (!dictionary_contains(dictionary2, entry1->key)) return false;
        }
    }
    return true;
}
