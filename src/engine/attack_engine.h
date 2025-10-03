#ifndef ATTACK_ENGINE_H
#define ATTACK_ENGINE_H

#include "../common/types.h"

// Attack engine interface
attack_result_t* attack_engine_run(const char *hash, const attack_config_t *config);

// Individual attack methods
attack_result_t* dictionary_attack(const char *hash, const attack_config_t *config);
attack_result_t* brute_force_attack(const char *hash, const attack_config_t *config);
attack_result_t* random_attack(const char *hash, const attack_config_t *config);
attack_result_t* auto_attack(const char *hash, const attack_config_t *config);

// Result management
attack_result_t* create_attack_result(void);
void destroy_attack_result(attack_result_t *result);

#endif
