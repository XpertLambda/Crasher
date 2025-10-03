#ifndef HASH_ENGINE_H
#define HASH_ENGINE_H

#include "../common/types.h"

// Hash verification functions
bool verify_md5(const char *plaintext, const char *hash);
bool verify_sha1(const char *plaintext, const char *hash);
bool verify_sha256(const char *plaintext, const char *hash);

// Get hash verification function for given type
hash_verify_func_t get_hash_verifier(hash_type_t type);

// Optimized hash computation with pre-allocated context
typedef struct hash_context hash_context_t;

hash_context_t* hash_context_create(hash_type_t type);
void hash_context_destroy(hash_context_t *ctx);
bool hash_context_verify(hash_context_t *ctx, const char *plaintext, const char *hash);

#endif
