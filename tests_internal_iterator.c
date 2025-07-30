#include "test_malloc.h"
#include "testing.h"
#include "internal_iterator.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool print_key_value(const char* key, void* value, void* extra) {
    printf("Clave: %s, Valor: %d\n", key, *(int*)value);
    int* counter = (int*)extra;
    (*counter)++;
    return false; 
}

bool run_iteration_test_0() {
    bool tests_result = true;
    dictionary_t* dict = dictionary_create(NULL);
    int counter = 0;
    iterate(dict, print_key_value, &counter);
    
    int expected_counter = 0;
    tests_result &= test_assert("No se itero", counter == expected_counter);
    dictionary_destroy(dict);
    return tests_result;
}

bool run_iteration_tests_put (){
    bool tests_result = true;
    dictionary_t* dict = dictionary_create(NULL);
    int one = 1, two = 2, three = 3;
    char *key1 = "Key1", *key2 = "Key2", *key3 = "Key3";
    dictionary_put(dict, key1, &one);
    dictionary_put(dict, key2, &two);
    dictionary_put(dict, key3, &three);
    int counter = 0;
    iterate(dict, print_key_value, &counter);
    int expected_counter = 3;
    tests_result &= test_assert("Se itero", counter == expected_counter);
    dictionary_destroy(dict);
    return true;
}

int main(void) {
    srand(117);
    int return_code = 0;
    return_code += run_iteration_test_0();
    return_code += run_iteration_tests_put();
    exit(return_code);
    return return_code;
}
