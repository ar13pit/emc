#!/usr/bin/env bash

BIN=bin
BUILD=build

# Remove directories bin/ and build/
if [ -d "$BIN" ]; then
    rm -r $BIN
fi

if [ -d "$BUILD" ]; then
    rm -r $BUILD
fi

# Make binaries in bin/
mkdir $BIN $BUILD
cd $BUILD
cmake ..
make
cd ..
rm -r $BUILD

# Execute "main"
$BIN/main
