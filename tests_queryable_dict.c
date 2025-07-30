#include "test_malloc.h"
#include "testing.h"
#include "queryable_dict.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* copy (void*){
    return NULL;
}

bool test_create_and_destroy_qdict() {
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    queryable_dictionary_t *dict = qdictionary_create(free);
    tests_result &= test_assert("El diccionario fue creado", dict != NULL);
    qdictionary_destroy(dict);
    return tests_result;
}

bool test_create_failed_qdict (){
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    set_malloc_status(false);
    queryable_dictionary_t *dict = qdictionary_create(free);
    set_malloc_status(true);
    tests_result &= test_assert("El diccionario no fue creado", dict == NULL);
    qdictionary_destroy(dict);
    return tests_result;
}

bool test_get_keys() {
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    set_malloc_status(true);
    queryable_dictionary_t *dict = qdictionary_create(NULL);

    int one = 1, two = 2, three = 3;
    bool key1 = qdictionary_put(dict, "key1", &one);
    bool key2 = qdictionary_put(dict, "key2", &two);
    bool key3 = qdictionary_put(dict, "key3", &three);

    tests_result &= test_assert("Se inserto key1", key1);
    tests_result &= test_assert("Se inserto key2", key2);
    tests_result &= test_assert("Se inserto key3", key3);

    char **keys = dictionary_get_keys (dict);

    tests_result &= test_assert("La cantidad es 3", qdictionary_size(dict) == 3);

    tests_result &= test_assert("Key 1 esta primera" ,strcmp(keys[0], "key1") == 0);
    tests_result &= test_assert( "Key 2 esta segunda" ,strcmp(keys[1], "key2") == 0);
    tests_result &= test_assert("Key 3 esta tercera" ,strcmp(keys[2], "key3") == 0);

    int zero = 0;
    void **values = dictionary_get_values(dict, copy(&zero));

    for (size_t i = 0; i < qdictionary_size(dict); i++) {
        tests_result &= test_assert("Los valores son correctos", *(int*)values[i] == *(int*)&one);
        one ++;
    }

    qdictionary_put(dict, "key1", &two);

    bool err;
    tests_result &= test_assert("Valor de key 1 correcto", qdictionary_get(dict, "key1", &err) == &two);

    for (size_t i = 0; i < qdictionary_size(dict); i++) {
        free(keys[i]);
    }
    free(values);
    free(keys);
    qdictionary_destroy(dict);

    return tests_result;
}

bool test_put_pop (){
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_results = true;
    set_malloc_status(true);
    queryable_dictionary_t *dict = qdictionary_create(NULL);
    int one = 1, two = 2, three = 3; 

    bool key1 = qdictionary_put(dict, "key1", &one);
    bool key2 = qdictionary_put(dict, "key2", &two);
    bool key3 = qdictionary_put(dict, "key3", &three);

    tests_results &= test_assert("Se inserto key1", key1);
    tests_results &= test_assert("Se inserto key2", key2);
    tests_results &= test_assert("Se inserto key3", key3);

    char** keys = dictionary_get_keys(dict);
    tests_results &= test_assert("Key 1 esta primera" ,strcmp(keys[0], "key1") == 0);
    tests_results &= test_assert( "Key 2 esta segunda" ,strcmp(keys[1], "key2") == 0);
    tests_results &= test_assert("Key 3 esta tercera" ,strcmp(keys[2], "key3") == 0);

    for (size_t i = 0; i < qdictionary_size(dict); i++) {
        free(keys[i]);
    }
    free(keys);

    bool del_key2 = qdictionary_delete(dict, "key2");

    tests_results &= test_assert("Se borro key2", del_key2);

    char **keys2 = dictionary_get_keys(dict);
    tests_results &= test_assert("Key 1 esta primera" ,strcmp(keys2[0], "key1") == 0);
    tests_results &= test_assert("Key 3 esta segunda" ,strcmp(keys2[1], "key3") == 0);

    for (size_t i = 0; i < qdictionary_size(dict); i++) {
        free(keys2[i]);
    }
    free(keys2);

    qdictionary_destroy(dict);

    return tests_results;

}


int main(void) {
    srand(117);
    int return_code = 0;
    return_code += !test_create_and_destroy_qdict();
    return_code += !test_create_failed_qdict();
    return_code += !test_get_keys();
    return_code += !test_put_pop();
    exit(return_code);
    return return_code;
}
