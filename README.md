# UXP-projekt
Client-server LINDA communcation.

## Build, Test, Run
```
mkdir build
cd build
cmake ..
make
```
```
ctest
```
```
./server
./client -i
```

## Example LINDA prompts
```
output((123, 1.25, "abc"))
read((int: 0), 30)
input((int: 1, string: "abc"), 0)
```
