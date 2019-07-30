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

#ifndef CPP_SRC_DEMO_TYPES_H_
#define CPP_SRC_DEMO_TYPES_H_

#include <vector>
#include <string>
#include <map>

#include "docopt/docopt.h"

namespace jomiel {

using opts_t = std::map<std::string, docopt::value>;
using strvect_t = std::vector<std::string>;

}  // namespace jomiel

#endif  // CPP_SRC_DEMO_TYPES_H_

// vim: set ts=2 sw=2 tw=72 expandtab:
