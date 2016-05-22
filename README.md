# Ubiquity

## Project description

## Build

### Protobuf generation
```javascript
protoc -I={src_dir}\ --java_out={src_dir}\java --cpp_out={src_dir}\cpp {src_dir}\remote_command.proto
```

# Build scripts
## Install the requirements
```
$ conan install . -s compiler=clang -s compiler.version=3.7 -s compiler.libcxx=libstdc++11 --build googletest
$ conan install . -s compiler="Visual Studio" -s compiler.version=14 -s compiler.runtime=MT --build googletest
$ conan install . -s compiler=apple-clang -s compiler.version=7.3 -s compiler.libcxx=libstdc++ --build googletest
```

## Compile and run the tests
```
$ rm -rf build && mkdir build && cd build
$ cmake ../ -DBUILD_TEST=TRUE && cmake --build .
```
