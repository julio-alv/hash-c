# hash-c
A simple implementation of a hash map in C; built by me, for learing purposes.

It leverages the FNV-1a algorithm for hashing, so it is *NOT* cryptographically secure.

_the max number of entries is initialized with the `hashmap_create` function and remains static, there is no mechanism to grow this number yet_
_this could lead into issues where the collision handler is triggred and it writes into an invalid memory address, causing undefined behaviour_

## Building the project

```sh
clang main.c
```

## Running the project

```sh
./a.out
```