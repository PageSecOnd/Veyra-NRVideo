// SPDX-License-Identifier: GPL-3.0-only
#include "veyra/Log.h"

namespace veyra::log {
void trace(const char*, const std::string&) {}
void info(const char*, const std::string&) {}
void warn(const char*, const std::string&) {}
void error(const char*, const std::string&) {}
bool verboseFrameLogs() { return false; }
} // namespace veyra::log
