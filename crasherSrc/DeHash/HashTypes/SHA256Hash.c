#include "../../Include.h"

int SHA256Hash(const char *content, const char *key) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;

    OpenSSL_add_all_digests();

    md = EVP_get_digestbyname("sha256");

    if (!md) {
        fprintf(stderr, "Unknown digest\n");
        return 0;  // Indicate failure
    }

    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, content, strlen(content));
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);

    char *hashed_content = malloc(md_len * 2 + 1);
    if (!hashed_content) {
        perror("Memory allocation failed");
        return 0;  // Indicate failure
    }

    for (i = 0; i < md_len; i++) {
        sprintf((char*)&(hashed_content[i * 2]), "%02x", md_value[i]);
    }

    int match = strcmp(hashed_content, key) == 0;

    free(hashed_content);

    return match;  // Return 1 for match, 0 for mismatch
}

