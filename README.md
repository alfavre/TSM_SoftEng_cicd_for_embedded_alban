# TSM_PicoW_Sensor
Project using the PicoW board with VS Code, implementing a sensor application.

## Build
Project has `Debug`, `Release` and `Test` targets, using CMake Presets. 

Configure:
```
cmake --list-presets
cmake --preset Debug
cmake --preset Release
cmake --preset Test
```

Build:
```
cmake --build --list-presets
cmake --build --preset app-debug
cmake --build --preset app-release
cmake --build --preset app-test
```

Test:
```
ctest --list-presets
ctest --test-dir build/Test -R Led_1
```

Worflow:
```
cmake --workflow --list-presets
```
