#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Entry structure to store key-value pairs
typedef struct Entry
{
    void* key;
    size_t key_len;
    void* value;
    size_t value_len;
    struct Entry* next; // For handling collisions
} Entry;

typedef struct HashMap
{
    Entry** entries;
    size_t num_entries;
    size_t size;
    size_t(*hash_func)(const void* key, size_t key_len); // Hashing function
} HashMap;

// Generate a hash (using FNV-1a)
size_t fnv1a_hash(const void* key, size_t key_len)
{
    const uint8_t* data = (const uint8_t*)key;
    size_t hash = 2166136261u;
    for (size_t i = 0; i < key_len; i++)
    {
        hash ^= data[i];
        hash *= 16777619;
    }
    return hash;
}

HashMap* hashmap_create(size_t num_entries, size_t(*hash_func)(const void*, size_t))
{
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    map->entries = (Entry**)calloc(num_entries, sizeof(Entry*));
    map->num_entries = num_entries;
    map->size = 0;
    map->hash_func = hash_func;
    return map;
}

void hashmap_put(HashMap* map, const void* key, size_t key_len, const void* value, size_t value_len)
{
    size_t hash = map->hash_func(key, key_len) % map->num_entries;
    Entry* entry = map->entries[hash];
    Entry* prev = NULL;

    while (entry)
    {
        if (entry->key_len == key_len && memcmp(entry->key, key, key_len) == 0)
        {
            free(entry->value);
            entry->value = malloc(value_len);
            memcpy(entry->value, value, value_len);
            entry->value_len = value_len;
            return;
        }
        prev = entry;
        entry = entry->next;
    }

    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    new_entry->key = malloc(key_len);
    new_entry->value = malloc(value_len);
    memcpy(new_entry->key, key, key_len);
    memcpy(new_entry->value, value, value_len);
    new_entry->key_len = key_len;
    new_entry->value_len = value_len;
    new_entry->next = NULL;

    if (prev)
    {
        prev->next = new_entry;
    }
    else
    {
        map->entries[hash] = new_entry;
    }
    map->size++;
}

void hashmap_delete(HashMap* map, const void* key, size_t key_len) {
    size_t hash = map->hash_func(key, key_len) % map->num_entries;
    Entry* entry = map->entries[hash];
    Entry* prev = NULL;

    while (entry)
    {
        if (entry->key_len == key_len && memcmp(entry->key, key, key_len) == 0)
        {
            if (prev && entry->next) {
                prev->next = entry->next;
            }
            else if (prev) {
                prev->next = NULL;
            }
            else {
                map->entries[hash] = entry->next;
            }

            free(entry->key);
            free(entry->value);
            free(entry);

            map->size--;
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

bool hashmap_get(HashMap* map, const void* key, size_t key_len, void** value, size_t* value_len)
{
    size_t hash = map->hash_func(key, key_len) % map->num_entries;
    Entry* entry = map->entries[hash];

    while (entry)
    {
        if (entry->key_len == key_len && memcmp(entry->key, key, key_len) == 0)
        {
            *value = malloc(entry->value_len);
            memcpy(*value, entry->value, entry->value_len);
            *value_len = entry->value_len;
            return true;
        }
        entry = entry->next;
    }
    return false;
}

void hashmap_free(HashMap* map)
{
    for (size_t i = 0; i < map->num_entries; i++)
    {
        Entry* entry = map->entries[i];
        while (entry)
        {
            Entry* next = entry->next;
            free(entry->key);
            free(entry->value);
            free(entry);
            entry = next;
        }
    }
    free(map->entries);
    free(map);
}