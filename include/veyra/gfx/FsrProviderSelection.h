// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <cstddef>
#include <span>
#include <string_view>

namespace veyra::gfx {

struct FsrProviderSelection {
    static constexpr size_t npos = size_t(-1);
    size_t index = npos;
    std::string_view family;
    bool fellBack = false;
    constexpr explicit operator bool() const { return index != npos; }
};

// The signed upscaler DLL can expose several implementations. RX 9000 asks
// for the 4.x ML family; all other adapters ask for 3.1. A missing ML provider
// falls back only to 3.1, never to an arbitrary first entry such as FSR 2.x.
inline FsrProviderSelection selectFsrUpscalerProvider(
    std::span<const char* const> names, bool preferMl) {
    const auto find = [&](std::string_view family) {
        for (size_t i = 0; i < names.size(); ++i) {
            const std::string_view name = names[i] ? names[i] : "";
            if (name.find(family) != std::string_view::npos) return i;
        }
        return FsrProviderSelection::npos;
    };
    if (preferMl) {
        if (const size_t index = find("4."); index != FsrProviderSelection::npos)
            return {index, "4.", false};
    }
    if (const size_t index = find("3.1."); index != FsrProviderSelection::npos)
        return {index, "3.1.", preferMl};
    return {};
}

} // namespace veyra::gfx
