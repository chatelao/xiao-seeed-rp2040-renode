#!/bin/bash
# Absolute path to the directory containing this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

pip install platformio

# Build pioasm from source as it's not provided in the toolchain
if [ ! -f /usr/local/bin/pioasm ]; then
    # We need to find where the framework-arduinopico is installed
    # It might not be installed yet, so we'll install platformio first then trigger a fake run or use pio pkg install
    pio pkg install --global --platform "https://github.com/Seeed-Studio/platform-seeedboards.git"

    PIOASM_SRC_PATH=$(find ~/.platformio/packages/framework-arduinopico -name pioasm -type d | grep -v "\.git" | head -n 1)
    if [ -d "$PIOASM_SRC_PATH" ]; then
        mkdir -p /tmp/build_pioasm
        cd /tmp/build_pioasm
        cmake "$PIOASM_SRC_PATH"
        make -j$(nproc)
        mkdir -p ~/.local/bin
        cp pioasm ~/.local/bin/
        echo "PATH=\$PATH:\$HOME/.local/bin" >> ~/.bashrc
        export PATH=$PATH:$HOME/.local/bin
        cd -
        rm -rf /tmp/build_pioasm
    fi
fi

# Generate PIO headers if pioasm is available and .pio files exist
if command -v pioasm >/dev/null 2>&1; then
    for f in src/*.pio; do
        if [ -f "$f" ]; then
            pioasm "$f" "$f.h"
        fi
    done
fi
