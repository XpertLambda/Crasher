#include "hash_engine.h"
#include "../common/utils.h"
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

struct hash_context {
    hash_type_t type;
    const EVP_MD *md;
    EVP_MD_CTX *ctx;
    unsigned char digest[EVP_MAX_MD_SIZE];
    char hex_output[EVP_MAX_MD_SIZE * 2 + 1];
};

static const char hex_chars[] = "0123456789abcdef";

static void bytes_to_hex_fast(const unsigned char *bytes, unsigned int len, char *hex) {
    for (unsigned int i = 0; i < len; i++) {
        hex[i * 2] = hex_chars[bytes[i] >> 4];
        hex[i * 2 + 1] = hex_chars[bytes[i] & 0x0F];
    }
    hex[len * 2] = '\0';
}

static bool verify_hash_generic(const char *plaintext, const char *hash, const EVP_MD *md) {
    if (!plaintext || !hash || !md) {
        return false;
    }

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) {
        return false;
    }

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len;
    bool result = false;

    if (EVP_DigestInit_ex(ctx, md, NULL) == 1 &&
        EVP_DigestUpdate(ctx, plaintext, strlen(plaintext)) == 1 &&
        EVP_DigestFinal_ex(ctx, digest, &digest_len) == 1) {
        
        char hex_output[EVP_MAX_MD_SIZE * 2 + 1];
        bytes_to_hex_fast(digest, digest_len, hex_output);
        
        result = (strcasecmp(hex_output, hash) == 0);
    }

    EVP_MD_CTX_free(ctx);
    return result;
}

bool verify_md5(const char *plaintext, const char *hash) {
    return verify_hash_generic(plaintext, hash, EVP_md5());
}

bool verify_sha1(const char *plaintext, const char *hash) {
    return verify_hash_generic(plaintext, hash, EVP_sha1());
}

bool verify_sha256(const char *plaintext, const char *hash) {
    return verify_hash_generic(plaintext, hash, EVP_sha256());
}

hash_verify_func_t get_hash_verifier(hash_type_t type) {
    switch (type) {
        case HASH_MD5:    return verify_md5;
        case HASH_SHA1:   return verify_sha1;
        case HASH_SHA256: return verify_sha256;
        default:          return NULL;
    }
}

hash_context_t* hash_context_create(hash_type_t type) {
    hash_context_t *ctx = safe_malloc(sizeof(hash_context_t));
    
    ctx->type = type;
    ctx->ctx = EVP_MD_CTX_new();
    
    if (!ctx->ctx) {
        free(ctx);
        return NULL;
    }
    
    switch (type) {
        case HASH_MD5:    ctx->md = EVP_md5(); break;
        case HASH_SHA1:   ctx->md = EVP_sha1(); break;
        case HASH_SHA256: ctx->md = EVP_sha256(); break;
        default:
            EVP_MD_CTX_free(ctx->ctx);
            free(ctx);
            return NULL;
    }
    
    return ctx;
}

void hash_context_destroy(hash_context_t *ctx) {
    if (ctx) {
        if (ctx->ctx) {
            EVP_MD_CTX_free(ctx->ctx);
        }
        free(ctx);
    }
}

bool hash_context_verify(hash_context_t *ctx, const char *plaintext, const char *hash) {
    if (!ctx || !plaintext || !hash) {
        return false;
    }

    unsigned int digest_len;
    
    if (EVP_DigestInit_ex(ctx->ctx, ctx->md, NULL) != 1 ||
        EVP_DigestUpdate(ctx->ctx, plaintext, strlen(plaintext)) != 1 ||
        EVP_DigestFinal_ex(ctx->ctx, ctx->digest, &digest_len) != 1) {
        return false;
    }
    
    bytes_to_hex_fast(ctx->digest, digest_len, ctx->hex_output);
    return (strcasecmp(ctx->hex_output, hash) == 0);
}
