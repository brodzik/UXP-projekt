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
./client
```
**System Tests**
```
cd systemTests/scripts
chmod +x *.sh
```
run server:
```
cd build
./server
```
in another command prompt run tests:
```
cd build
./systemTest
```


## Example LINDA prompts
```
output((123, 1.25, "abc"))
read((int: 0), 30)
input((int: 1, string: "abc"), 0)
```
