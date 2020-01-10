#!/bin/sh

echo "Running golPlayground.."
echo ""

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"$PWD/Submodules"

chmod +x Binaries/golPlayground

./Binaries/golPlayground