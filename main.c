#include <stdio.h>
#include "hash.c"

typedef struct Person
{
    char* name;
    char* last_name;
    uint8_t age;
}Person;


int main()
{
    HashMap* map = hashmap_create(16, fnv1a_hash);

    char* key = "John";
    Person value = {
        .name = "John",
        .last_name = "Doe",
        .age = 32,
    };

    hashmap_put(map, key, strlen(key), &value, sizeof(value));

    void* retrieved_value;
    size_t retrieved_len;
    if (hashmap_get(map, key, strlen(key), &retrieved_value, &retrieved_len))
    {
        Person* result = (Person*)retrieved_value;
        printf("name: %s\n", result->name);
        printf("last name: %s\n", result->last_name);
        printf("age: %hhu\n", result->age);

        free(retrieved_value);
    }
    else
    {
        printf("Key not found.\n");
    }

    hashmap_delete(map, key, strlen(key));

    if (hashmap_get(map, key, strlen(key), &retrieved_value, &retrieved_len))
    {
        Person* result = (Person*)retrieved_value;
        printf("name: %s\n", result->name);
        printf("last name: %s\n", result->last_name);
        printf("age: %hhu\n", result->age);

        free(retrieved_value);
    }
    else
    {
        printf("Key not found.\n");
    }

    hashmap_free(map);
    return 0;
}