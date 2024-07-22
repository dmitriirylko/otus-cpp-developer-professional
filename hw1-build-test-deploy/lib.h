#pragma once

#include <string>

#include "version.h"

/**
 * @brief Compares two versions represented as strings in format
 * 			"n1.n2.n3" like (1.12.52).
 *  @param v1 Version (string) to check.
 *  @param v2 Version (string) to check with.
 *  @return true if v1 > v2, false otherwise.
 *  @todo Error checks for input strings.
 */
bool isVersionGreater(std::string v1, std::string v2);

std::string version();