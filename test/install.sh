#!/bin/bash
set -e

# Absolute path to the directory containing this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Install Renode 1.16.1
RENODE_VERSION="1.16.1"
RENODE_TAR="renode-${RENODE_VERSION}.linux-portable.tar.gz"
RENODE_URL="https://github.com/renode/renode/releases/download/v${RENODE_VERSION}/${RENODE_TAR}"
RENODE_DIR="${PROJECT_ROOT}/test/renode"

if [ ! -d "$RENODE_DIR" ]; then
    echo "Downloading and installing Renode ${RENODE_VERSION}..."
    wget "$RENODE_URL"
    mkdir -p "$RENODE_DIR"
    tar -xzf "$RENODE_TAR" -C "$RENODE_DIR" --strip-components=1
    rm "$RENODE_TAR"
else
    echo "Renode already installed in $RENODE_DIR"
fi

# Install Robot Framework and other dependencies
pip install -r "${PROJECT_ROOT}/test/requirements.txt"
