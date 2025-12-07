IF EXIST "\build" RD "\build"
IF EXIST "tests.exe" DEL "tests.exe"
IF EXIST "qcs_*.exe" DEL "qcs_*.exe"

mkdir build
cd build
cmake -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
cmake --build . --config Release

cd ../
copy ".\build\tests.exe" .
copy ".\build\qcs_*.exe" .
