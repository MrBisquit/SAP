@echo off

mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build . --config Debug
cd ..