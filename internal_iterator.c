#include "internal_iterator.h"

/*
 * Punto extra de internal iterator, suma 1 punto como máximo.
 */


/*
 * Itera cada par clave-valor del diccionario pasandoselo a la función de iteración.
 * Recibe un parámetro extra que puede contener cualquier cosa para permitirle a la función guardar resultados.
 */
void iterate(dictionary_t* dict, iterate_f f, void* extra){
    for (int i = 0; i < dict->size; i++){
        if (dict->table[i].key != NULL && !dict->table[i].deleted){
            if (f(dict->table[i].key, dict->table[i].value, extra)){
                return;
            }
        }
    }
    return;
}
