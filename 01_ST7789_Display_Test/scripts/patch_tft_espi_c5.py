Import("env")

from pathlib import Path
import urllib.request

ENV_NAME = env.subst("$PIOENV")
LIBDEPS = Path(env.subst("$PROJECT_LIBDEPS_DIR")) / ENV_NAME
TFT_DIR = LIBDEPS / "TFT_eSPI"
PROC_DIR = TFT_DIR / "Processors"

BASE = "https://raw.githubusercontent.com/BruceDevices/firmware/ac869d3d99ba222fd2fe7f76b707e4929385bd4c/lib/TFT_eSPI/Processors"
FILES = {
    "TFT_eSPI_ESP32_C5.h": f"{BASE}/TFT_eSPI_ESP32_C5.h",
    "TFT_eSPI_ESP32_C5.c": f"{BASE}/TFT_eSPI_ESP32_C5.c",
}


def download_if_needed(url: str, target: Path) -> None:
    if target.exists() and target.stat().st_size > 1000:
        return
    target.parent.mkdir(parents=True, exist_ok=True)
    print(f"[预处理] 下载 ESP32-C5 TFT_eSPI 兼容文件：{target.name}")
    with urllib.request.urlopen(url, timeout=30) as response:
        target.write_bytes(response.read())


def patch_text(path: Path, old: str, new: str) -> None:
    text = path.read_text(encoding="utf-8")
    if new in text:
        return
    if old not in text:
        raise RuntimeError(f"无法识别 TFT_eSPI 文件结构：{path}")
    path.write_text(text.replace(old, new, 1), encoding="utf-8")
    print(f"[预处理] 已适配：{path.name}")


def apply_patch(*_args, **_kwargs) -> None:
    if not TFT_DIR.exists():
        raise RuntimeError(
            "尚未找到 TFT_eSPI 依赖目录。请先删除 .pio 后重新编译，"
            "PlatformIO 会先安装 lib_deps。"
        )

    for filename, url in FILES.items():
        download_if_needed(url, PROC_DIR / filename)

    patch_text(
        TFT_DIR / "TFT_eSPI.h",
        '#elif defined (ESP32)\n  #include "Processors/TFT_eSPI_ESP32.h"',
        '#elif defined(CONFIG_IDF_TARGET_ESP32C5)\n  #include "Processors/TFT_eSPI_ESP32_C5.h"\n#elif defined (ESP32)\n  #include "Processors/TFT_eSPI_ESP32.h"',
    )

    cpp_path = TFT_DIR / "TFT_eSPI.cpp"
    cpp = cpp_path.read_text(encoding="utf-8")
    marker = '#elif defined(CONFIG_IDF_TARGET_ESP32C5)'
    if marker not in cpp:
        old = '  #else\n    #include "Processors/TFT_eSPI_ESP32.c"'
        new = '  #elif defined(CONFIG_IDF_TARGET_ESP32C5)\n    #include "Processors/TFT_eSPI_ESP32_C5.c"\n  #else\n    #include "Processors/TFT_eSPI_ESP32.c"'
        if old not in cpp:
            raise RuntimeError(f"无法识别 TFT_eSPI.cpp 文件结构：{cpp_path}")
        cpp_path.write_text(cpp.replace(old, new, 1), encoding="utf-8")
        print("[预处理] 已适配：TFT_eSPI.cpp")


# lib_deps 通常在 extra_scripts 加载前已经安装；这里立即应用。
apply_patch()
