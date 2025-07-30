#include "tp3.h"

// struct entry {
//     char *key;
//     void *value;
//     bool deleted;
// };

// struct dictionary {
//     size_t size;
//     size_t count;
//     entry_t *table;
//     destroy_f destroy;
// };

dictionary_t *dictionary_create(destroy_f destroy) {
    dictionary_t *dictionary = (dictionary_t*)malloc(sizeof(dictionary_t));
    if (!dictionary) {
        return NULL;
    }

    dictionary->size = INITIAL_SIZE;
    dictionary->count = 0;
    dictionary->table = (entry_t*)calloc(dictionary->size, sizeof(entry_t));
    if (!dictionary->table) {
        free(dictionary);
        return NULL;
    }

    dictionary->destroy = destroy;
    return dictionary;
}

unsigned long hash_funcc(const char* key, size_t size) { //SUM
    int hash_value = 0;
    for (int i = 0; i < strlen(key); i++) {
        hash_value += key[i];
    }
    return hash_value % size;
}

unsigned long hash_funccc(const char *key, size_t table_size) { //DJB2
    size_t hash = 0;
    size_t i = 0;

    while (key[i] != '\0') {
        hash = (hash << 5) + key[i];
        i++;
    }

    return hash % table_size;
}

unsigned long hash_func_fnv(const char *key, size_t table_size) { //FNV
    const size_t FNV_PRIME = 1099511628211UL;
    const size_t FNV_OFFSET_BASIS = 14695981039346656037UL;
    size_t hash = FNV_OFFSET_BASIS;
    size_t i = 0;

    while (key[i] != '\0') {
        hash ^= (size_t)key[i];
        hash *= FNV_PRIME;
        i++;
    }
    
    size_t hash_val = hash % table_size;
    if (hash_val < 0) {
        hash_val += table_size;
    }
    return hash_val;
}

unsigned long hash_func(const char* key, size_t size) { //MURMUR
    unsigned int seed = 0;
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    unsigned long h = seed ^ strlen(key);

    const unsigned char* data = (const unsigned char*)key;

    while (*data) {
        unsigned int k = *data++;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h % size;
}



entry_t* find_entry (dictionary_t* dict, const char* key, size_t size) {
    size_t index = hash_func(key, size) % size;
    entry_t* entry = &dict->table[index];
    while (entry->key != NULL){
        if (!entry->deleted && strcmp(entry->key, key) == 0) {
            return entry;
        }
        if (entry->deleted && !dictionary_contains(dict, key)){
            return entry;
        }
        entry = &dict->table[(++index) % size];
    }
    return entry;
}

bool dictionary_resize(dictionary_t *dict, size_t new_size) {
    if (dict == NULL || new_size <= dict->count) {
        return false;
    }

    entry_t *new_table = calloc(new_size, sizeof(entry_t));
    if (new_table == NULL) {
        return false;
    }

    for (size_t i = 0; i < dict->size; i++) {
        if (dict->table[i].key != NULL && !dict->table[i].deleted) {
            long int new_index = hash_func(dict->table[i].key, new_size) % new_size;
            entry_t *new_entry = &new_table[new_index];
            while (new_entry->key != NULL) {
                new_entry = &new_table[(++new_index) % new_size];
            }
            *new_entry = dict->table[i];
        }
    }
    free(dict->table);
    dict->table = new_table;
    dict->size = new_size;

    return true;
}

char *my_strdup(const char *str) {
    size_t len = strlen(str);
    char *new_str = (char *) malloc(len + 1);
    if (new_str != NULL) {
        memcpy(new_str, str, len + 1);
    }
    return new_str;
}

bool put_entry (dictionary_t* dict, entry_t* entry, const char* key, void* value){
    entry->key = my_strdup(key);
    if (entry->key == NULL) {
        return false;
    }
    entry->value = value;
    entry->deleted = false;
    dict->count++;
    return true;
}

bool dictionary_put(dictionary_t *dict, const char *key, void *value){
    float load_factor = (float)dict->count / (float)dict->size;
    if (load_factor >= LOAD_FACTOR) {
        if (!dictionary_resize(dict, dict->size * 2)) return false;
    }

    entry_t *entry = find_entry(dict, key, dict->size);

    if (entry->key){
        if (!entry->deleted) {
            if (dict->destroy != NULL) {
                dict->destroy(entry->value);
            }
            entry->value = value;
            entry->deleted = false;
            return true;
        }
        if (entry->deleted && !dictionary_contains(dict, key)){
            return put_entry(dict, entry, key, value);
        }
    }

    return put_entry(dict, entry, key, value);
}

entry_t* get_entry (dictionary_t *dict, const char *key) {
    long int index = hash_func(key, dict->size) % dict->size;
    entry_t *entry = &dict->table[index];
    while (entry->key != NULL) {
        if (!entry->deleted && strcmp(entry->key, key) == 0) {
            return entry;
        }
        entry = &dict->table[(++index) % dict->size];
    }
    return NULL;
}

void *dictionary_get(dictionary_t *dict, const char *key, bool *err){

    entry_t *entry = get_entry(dict, key);
    if (entry != NULL) {
        *err = false;
        return entry->value;
    } 
    *err = true;
    return NULL;
};

bool dictionary_delete(dictionary_t *dictionary, const char *key) {
    bool result;
    void* value = dictionary_pop(dictionary, key, &result);
    if (dictionary->destroy != NULL) {
        dictionary->destroy(value);
    }
    return !result;
};

void *dictionary_pop(dictionary_t *dict, const char *key, bool *err){

    entry_t *entry = get_entry(dict, key);
    if (entry != NULL) {
        entry->deleted = true;
        dict->count--;
        free(entry->key);
        *err = false;
        return entry->value;
    }
    *err = true;
    return NULL;
}

bool dictionary_contains(dictionary_t *dict, const char *key) {
    bool result;
    dictionary_get(dict, key, &result);
    return !result;
}

size_t dictionary_size(dictionary_t *dictionary){
    return dictionary->count;
}

void dictionary_destroy(dictionary_t *dictionary){
    if (dictionary == NULL) return;
    if (dictionary->table) {
        for (int i = 0; i < dictionary->size; i++) {
            if (dictionary->table[i].key != NULL && !dictionary->table[i].deleted) {
                free(dictionary->table[i].key);
                if (dictionary->destroy) dictionary->destroy(dictionary->table[i].value);
            }
        }
        free(dictionary->table);
    }

    free(dictionary);
};
