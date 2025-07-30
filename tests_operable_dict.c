#include "test_malloc.h"
#include "testing.h"
#include "operable_dict.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TESTEO CON MEMORIA ESTATICA

bool test_dictionary_update() {
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    
    dictionary_t *dict1 = dictionary_create(NULL);
    int value1 = 1;
    int value2 = 2;
    int value3 = 3;
    tests_result &= test_assert("Se puede insertar en dict1", dictionary_put(dict1, "key1", &value1));
    tests_result &= test_assert("Se puede insertar en dict1", dictionary_put(dict1, "key2", &value2));
    tests_result &= test_assert("Se puede insertar en dict1", dictionary_put(dict1, "key3", &value3));

    dictionary_t *dict2 = dictionary_create(NULL);
    int value4 = 4;
    int value5 = 5;
    tests_result &= test_assert("Se puede insertar en dict2", dictionary_put(dict2, "key2", &value4));
    tests_result &= test_assert("Se puede insertar en dict2", dictionary_put(dict2, "key3", &value5));

    tests_result &= test_assert("Se pudo actualizar el diccionario", dictionary_update(dict1, dict2));

    bool err;
    tests_result &= test_assert("El valor de key1 en dict1 es correcto",
                                dictionary_get(dict1, "key1", &err) == &value1);
    tests_result &= test_assert("El valor de key2 en dict1 es correcto",
                                dictionary_get(dict1, "key2", &err) == &value4);
    tests_result &= test_assert("El valor de key3 en dict1 es correcto",
                                dictionary_get(dict1, "key3", &err) == &value5);

    tests_result &= test_assert("El valor de key2 en dict2 es correcto",
                                dictionary_get(dict2, "key2", &err) == &value4);
    tests_result &= test_assert("El valor de key3 en dict2 es correcto",
                                dictionary_get(dict2, "key3", &err) == &value5);

    dictionary_destroy(dict1);
    dictionary_destroy(dict2);

    return tests_result;
}

bool test_update_fails () {
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    set_malloc_status(true);
    dictionary_t *dict1 = dictionary_create(NULL);
    int value1 = 1;
    tests_result &= test_assert("Se puede insertar en dict1", dictionary_put(dict1, "key1", &value1));
    size_t size = dict1->size;

    dictionary_t *dict2 = dictionary_create(NULL);
    int value4 = 4;
    tests_result &= test_assert("Se puede insertar en dict2", dictionary_put(dict2, "key2", &value4));

    set_malloc_status(false);
    tests_result &= test_assert("No se pudo actualizar el diccionario", !dictionary_update(dict1, dict2));
    set_malloc_status(true);

    bool err;
    tests_result &= test_assert("Dictionary 1 sigue igual", dictionary_get(dict1, "key1", &err) == &value1);
    tests_result &= test_assert("Dictionary 1 sigue igual", dictionary_get(dict1, "key2", &err) == NULL);
    tests_result &= test_assert("Dictionary 1 sigue igual", dict1->size == size);


    dictionary_destroy(dict1);
    dictionary_destroy(dict2);

    return tests_result;
}

bool test_dictionary_and() {
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;

    dictionary_t *dictionary1 = dictionary_create(NULL);
    if (dictionary1 == NULL) {
        printf("Error creating dictionary1\n");
        return false;
    }

    int value1 = 1, value2 = 2, value3 = 3;
    dictionary_put(dictionary1, "key1", &value1);
    dictionary_put(dictionary1, "key2", &value2);
    dictionary_put(dictionary1, "key3", &value3);

    dictionary_t *dictionary2 = dictionary_create(NULL);
    if (dictionary2 == NULL) {
        printf("Error creating dictionary2\n");
        dictionary_destroy(dictionary1);
        return false;
    }

    dictionary_put(dictionary2, "key2", &value2);
    dictionary_put(dictionary2, "key3", &value3);
    dictionary_put(dictionary2, "key4", &value3);

    dictionary_t *result = dictionary_and(dictionary1, dictionary2);
    if (result == NULL) {
        printf("Error performing dictionary_and\n");
        dictionary_destroy(dictionary1);
        dictionary_destroy(dictionary2);
        return false;
    }
    bool err;
    tests_result &= test_assert("El tamaño del resultado es correcto",
                                dictionary_size(result) == 2);
    tests_result &= test_assert("El resultado contiene la clave 'key2' con el valor correcto",
                                dictionary_get(result, "key2", &err) == &value2);
    tests_result &= test_assert("El resultado contiene la clave 'key3' con el valor correcto",
                                dictionary_get(result, "key3", &err) == &value3);
    tests_result &= test_assert("El resultado no contiene la clave 'key1'",
                                !dictionary_contains(result, "key1"));
    tests_result &= test_assert("El resultado no contiene la clave 'key4'",
                                !dictionary_contains(result, "key4"));

    dictionary_destroy(dictionary1);
    dictionary_destroy(dictionary2);
    dictionary_destroy(result);

    return tests_result;
}

bool test_dictionary_or() {
    printf("========== %s ==========\n", __func__);
    bool test_result = true;

    dictionary_t *dictionary1 = dictionary_create(NULL);
    int value1 = 1, value2 = 2, value3 = 3;
    dictionary_put(dictionary1, "key1", &value1);
    dictionary_put(dictionary1, "key2", &value2);

    dictionary_t *dictionary2 = dictionary_create(NULL);
    dictionary_put(dictionary2, "key2", &value3);
    dictionary_put(dictionary2, "key3", &value3);

    dictionary_t *result = dictionary_or(dictionary1, dictionary2);
    bool err;
    test_result &= test_assert("El resultado contiene la clave 'key1' y su valor es correcto",
                               dictionary_get(result, "key1", &err) == &value1);
    test_result &= test_assert("El resultado contiene la clave 'key2' y su valor es correcto",
                               dictionary_get(result, "key2", &err) == &value2);
    test_result &= test_assert("El resultado contiene la clave 'key3' y su valor es correcto",
                               dictionary_get(result, "key3", &err) == &value3);

    test_result &= test_assert("El tamaño del resultado es el esperado",
                               dictionary_size(result) == 3);

    dictionary_destroy(dictionary1);
    dictionary_destroy(dictionary2);
    dictionary_destroy(result);

    return test_result;
}


bool test_equals () {
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    set_malloc_status(true);
    dictionary_t* dict1 = dictionary_create(NULL);
    dictionary_t* dict2 = dictionary_create(NULL);
    bool tests_result = true;
    char* key1 = "key1", *key2 = "key2";
    int one = 1, two = 2;

    dictionary_put(dict1, key1, &one);
    dictionary_put(dict2, key1, &one);

    tests_result &= test_assert("Los diccionarios son iguales" ,dictionary_equals(dict1, dict2));

    dictionary_put(dict1, key2, &two);

    tests_result &= test_assert("Los diccionarios son distintos" , !dictionary_equals(dict1, dict2));
    dictionary_delete(dict1, key2);

    dictionary_put(dict2, key1, &two);
    tests_result = false;
    tests_result &= test_assert("Los diccionarios son distintos" ,dictionary_equals(dict1, dict2));
    tests_result = true;

    dictionary_destroy(dict1);
    dictionary_destroy(dict2);
    return tests_result;

}

int main(void) {
    srand(117);
    int return_code = 0;
    return_code += !test_dictionary_update();
    return_code += !test_update_fails();
    return_code += !test_dictionary_and();
    return_code += !test_dictionary_or();
    return_code += !test_equals();
    exit(return_code);
    return return_code;
}
