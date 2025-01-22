# hash-c
A simple implementation of a hash map in C; built by [me](https://github.com/julio-alv), for learing purposes.

It leverages [FNV-1a](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function) for hashing, so this is *NOT* cryptographically secure.

_the max number of entries is initialized with the `hashmap_create` function and remains static, there is no mechanism to grow this number, yet_
_this could lead into issues where the collision handler is triggred and it writes into an invalid memory address, causing undefined behaviour_

## Building the project

```sh
clang main.c
```

## Running the project

```sh
./a.out
```
