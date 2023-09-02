#include "feature_nickgen.hpp"

std::string networking::features::c_feature_nickgen::get_nickname()
{
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, FEATURE_NICKGEN_SIZE_NICKNAMES);
       
    return m_random_nicknames[dist(rng)];
}
