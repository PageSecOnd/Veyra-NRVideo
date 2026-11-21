from pathlib import Path

root = Path(__file__).resolve().parents[2]
cmake = (root / "CMakeLists.txt").read_text(encoding="utf-8")
qml = cmake.index("if(VEYRA_BUILD_QML_UI")
moonlight = cmake.index("if(VEYRA_ENABLE_MOONLIGHT)", qml)
moonlight_test = cmake.index("target_link_libraries(veyra_moonlight_model_tests", moonlight)
moonlight_end = cmake.index("endif()", moonlight_test)
xbox = cmake.index("if(VEYRA_ENABLE_XBOX)", moonlight_end)
assert xbox > moonlight_end
assert "add_library(veyra_xbox_model" in cmake[xbox:cmake.index("endif()", xbox)]

build = (root / "scripts/build-ui-migration.cmd").read_text(encoding="utf-8")
for feature in ("REMOTEPLAY", "MOONLIGHT", "XBOX"):
    assert f"-DVEYRA_ENABLE_{feature}=ON" in build
    assert f"^VEYRA_ENABLE_{feature}:BOOL=ON$" in build

backend = (root / "src/gfx/FsrSrBackend.cpp").read_text(encoding="utf-8")
for contract in (
    "selectFsrUpscalerProvider",
    "FFX_API_DESC_TYPE_OVERRIDE_VERSION",
    "FFX_API_QUERY_DESC_TYPE_GET_PROVIDER_VERSION",
    "actual.versionId != selectedProvider",
):
    assert contract in backend

print("Xbox/QML/FSR build contracts: PASS")
