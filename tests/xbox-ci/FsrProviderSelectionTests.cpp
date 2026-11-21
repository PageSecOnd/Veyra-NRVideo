// SPDX-License-Identifier: GPL-3.0-only
#include "veyra/gfx/FsrProviderSelection.h"

#include <array>
#include <cstdio>

int main() {
    using veyra::gfx::selectFsrUpscalerProvider;
    int failures = 0;
    const auto check = [&](bool value, const char* message) {
        if (!value) { std::printf("FAIL %s\n", message); ++failures; }
    };
    {
        const std::array<const char*,3> providers{"3.1.6", "4.1.1", "2.3.4"};
        const auto selected = selectFsrUpscalerProvider(providers, true);
        check(selected && selected.index == 1 && selected.family == "4." && !selected.fellBack,
              "RX 9000 selects the enumerated FSR 4 provider, not the first entry");
    }
    {
        const std::array<const char*,2> providers{"2.3.4", "3.1.6"};
        const auto selected = selectFsrUpscalerProvider(providers, true);
        check(selected && selected.index == 1 && selected.family == "3.1." && selected.fellBack,
              "RX 9000 explicitly falls back to FSR 3.1 when 4.x is absent");
    }
    {
        const std::array<const char*,3> providers{"4.1.1", nullptr, "3.1.5"};
        const auto selected = selectFsrUpscalerProvider(providers, false);
        check(selected && selected.index == 2 && !selected.fellBack,
              "cross-vendor path stays on FSR 3.1 even when 4.x is listed");
    }
    {
        const std::array<const char*,2> providers{"2.3.4", nullptr};
        check(!selectFsrUpscalerProvider(providers, false), "no arbitrary FSR 2.x fallback");
        check(!selectFsrUpscalerProvider(providers, true), "missing 4.x and 3.1 fails closed");
    }
    std::printf("FSR provider selection checks=%d failures=%d\n", 5, failures);
    return failures ? 1 : 0;
}
