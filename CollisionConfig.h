#pragma once
#include <cstdint>

//“–‚½‚è”»’è—pƒtƒ‰ƒO
const uint32_t kCollisionAttributePlayer = 0b1;//~0001(1)
const uint32_t kCollisionAttributeHand = 0b1 << 1;//~0010(2)
const uint32_t kCollisionAttributeEnemy = 0b1 << 2;//~0100(4)