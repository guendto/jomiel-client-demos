/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "docopt/docopt.h"

namespace jomiel {

using opts_t = std::map<std::string, docopt::value>;
using strvect_t = std::vector<std::string>;

} // namespace jomiel

// vim: set ts=2 sw=2 tw=72 expandtab:
