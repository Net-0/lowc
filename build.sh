#!/bin/bash
set -euo pipefail

input_dir="./src/cmd"
output_dir="./bin"
mkdir -p "$output_dir"

for src in "$input_dir"/*.c; do
    name=$(basename "$src" .c)
    clang \
        -ffreestanding \
        -fno-builtin \
        -fno-stack-protector \
        -nostdlib \
        -static \
        -Wswitch-enum -Werror=switch-enum \
        -fms-extensions \
        -Wl,-e,_start,-z,stack-size=536870912 \
        "$src" \
        -o "$output_dir/$name"
done
