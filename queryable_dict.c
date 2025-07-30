#include "tp3.h"
#include "queryable_dict.h"

/*
 * Punto extra de queryable dict, suma 3 puntos como máximo.
 */

struct queryable_dictionary {
    dictionary_t* dict;
    list_t* entries;

};

struct list {
    node_t* first;
    node_t* last;
};

struct node {
    char* key;
    value_ptr_t* value_ptr;
    node_t* next;
    node_t* prev;
};

struct value_ptr {
    void* value;
    node_t* item;
};

struct dict_item {
    char* key;
    void* value;
};

/* Obtiene las claves del diccionario en el mismo orden en el que fueron insertadas.
 * Pre-condiciones:
 * - El diccionario existe
 * Post-condiciones:
 * - Retorna un vector de punteros a claves del mismo largo que el largo del diccionario.
 * Estas claves son copias de las originales.
 */
char *my_strdupp(const char *str) {
    size_t len = strlen(str);
    char *new_str = (char *) malloc(len + 1);
    if (new_str != NULL) {
        memcpy(new_str, str, len + 1);
    }
    return new_str;
}
char** dictionary_get_keys(queryable_dictionary_t* dict){
    char** keys = (char**)malloc(sizeof(char*) * dict->dict->count);
    if (!keys) return NULL;
    keys[0] = my_strdupp(dict->entries->first->key);
    node_t* aux = dict->entries->first;
    for (size_t i = 1; i < dict->dict->count; i++){
        keys[i] = my_strdupp(aux->next->key);
        aux = aux->next;
    }
    return keys;
}

/* Obtiene los valores del diccionario en el mismo orden en el que fueron insertados.
 * Pre-condiciones:
 * - El diccionario existe
 * - La función de copia existe
 * Post-condiciones:
 * - Retorna un vector de punteros a valores del mismo largo que el largo del diccionario.
 * Estos valores son copias de los del diccionario.
 */
void** dictionary_get_values(queryable_dictionary_t* dict, copy_value_f copy){
    void** values = (void**)malloc(sizeof(void*) * dict->dict->count);
    if (!values) return NULL;
    values[0] = dict->entries->first->value_ptr->value; //copy(dict->entries->first->value);
    node_t* aux = dict->entries->first;
    for (size_t i = 1; i < dict->dict->count; i++){
        values[i] = aux->next->value_ptr->value; //copy(aux->next->value);
        aux = aux->next;
    }
    return values;
}

/* Obtiene las claves y valores del diccionario en el mismo orden en el que fueron insertados.
 * Pre-condiciones:
 * - El diccionario existe
 * - La función de copia existe
 * Post-condiciones:
 * - Retorna un vector de punteros a structs que contienen una copia de la clave y del valor.
 */
dict_item_t** dictionary_get_items(queryable_dictionary_t* dict, copy_value_f copy){
    dict_item_t** items = (dict_item_t**)malloc(sizeof(dict_item_t*) * dict->dict->count);
    if (!items) return NULL;
    node_t* aux = dict->entries->first;
    for (size_t i = 0; i < dict->dict->count; i++){
        items[i]->key = aux->next->key;
        items[i]->value = aux->next->value_ptr->value; //copy(aux->next->value);
        aux = aux->next;
    }
    return items;
}

// Tienen el mismo comportamiento que el diccionario común y SIGUEN SIENDO O(1)
queryable_dictionary_t *qdictionary_create(destroy_f destroy){
    queryable_dictionary_t* dict = malloc(sizeof(queryable_dictionary_t));
    if (!dict) return NULL;

    dict->dict = dictionary_create(destroy);
    if (!dict->dict) {
        free(dict);
        return NULL;
    }
    dict->entries = calloc(INITIAL_SIZE, sizeof(list_t));
    if (!dict->entries) {
        dictionary_destroy(dict->dict);
        free(dict);
        return NULL;
    }
    return dict;
}

bool qdictionary_put(queryable_dictionary_t *dictionary, const char *key, void *value){
    value_ptr_t* value_ptr = calloc(1, sizeof(value_ptr_t));
    if (!value_ptr) return false;
    value_ptr->value = value;

    if (!dictionary_put(dictionary->dict, key, value_ptr)) return false;

    if (dictionary->dict->count <= 1){
        dictionary->entries->first = calloc(1, sizeof(node_t));
        if (!dictionary->entries->first) return false;
        dictionary->entries->first->key = (char*)key;
        dictionary->entries->first->value_ptr = value_ptr;
        value_ptr->item = dictionary->entries->first;
        dictionary->entries->first->prev = NULL;
        dictionary->entries->first->next = NULL;
        dictionary->entries->last = dictionary->entries->first;
        return true;
    }
    dictionary->entries->last->next = calloc(1, sizeof(node_t));
    if (!dictionary->entries->last->next) return false;
    dictionary->entries->last->next->key = (char*)key;
    dictionary->entries->last->next->value_ptr = value_ptr;
    value_ptr->item = dictionary->entries->last->next;
    dictionary->entries->last->next->prev = dictionary->entries->last;
    dictionary->entries->last->next->next = NULL;
    dictionary->entries->last = dictionary->entries->last->next;
    return true;
}
void *qdictionary_get(queryable_dictionary_t *dictionary, const char *key, bool *err){
    value_ptr_t* result = dictionary_get(dictionary->dict, key, err);
    return result->value;
}
bool qdictionary_delete(queryable_dictionary_t *dictionary, const char *key){
    bool deleted;
    void* result = qdictionary_pop(dictionary, key, &deleted);
    if (dictionary->dict->destroy) dictionary->dict->destroy(result);
    return !deleted;
}
void *qdictionary_pop(queryable_dictionary_t* dictionary, const char *key, bool *err){
    value_ptr_t* result = dictionary_pop(dictionary->dict, key, err);
    if (*err) return NULL;
    if (result->item->prev) result->item->prev->next = result->item->next;
    if (result->item->next) result->item->next->prev = result->item->prev;
    if (result->item == dictionary->entries->first) dictionary->entries->first = result->item->next;
    if (result->item == dictionary->entries->last) dictionary->entries->last = result->item->prev;
    free(result->item);
    void* value = result->value;
    free(result);
    return value;

}
bool qdictionary_contains(queryable_dictionary_t *dictionary, const char *key){
    return dictionary_contains(dictionary->dict, key);
}
size_t qdictionary_size(queryable_dictionary_t *dictionary){
    return dictionary->dict->count;
}

void destroy_list (list_t* list){
    if (!list) return;
    node_t* aux = list->first;
    node_t* aux2;
    while (aux){
        aux2 = aux->next;
        free(aux->value_ptr);
        free(aux);
        aux = aux2;
    }
    free(list);
    return;
}

void qdictionary_destroy(queryable_dictionary_t *dictionary){
    if (dictionary){
        dictionary_destroy(dictionary->dict);
        if (dictionary->entries){
            destroy_list(dictionary->entries);
        }
        free(dictionary);
    }
    return;
}