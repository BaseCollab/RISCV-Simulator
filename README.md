# RISCV Simulator

## Build
```shell
cd simulator
mkdir build && cd build
cmake .. -GNinja
ninja simulator
```

## Project naming rules

Actual naming rules can be found in `NAMING_RULES.md`.

## Formatting

Before commit to master every contributor should use clang-format: 
```shell
ninja clang-force-format
```
