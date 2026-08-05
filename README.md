# Build:
```bash
## cd /path/to/root
mkdir build && cd build

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja ..
## or
# cmake ..

ninja
## or
# cmake --build .

## for some IDEs: 
cd ..

## detect includes
ln -s build/compile_commands.json compile_commands.json

## detect cpp version
cat <<EOF > .clangd
CompileFlags:
  Add: [-std=c++20]
EOF

```


# Single line:
```bash
mkdir build && cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja .. && cd .. && ln -s build/compile_commands.json compile_commands.json && cd build && ninja 
```

```bash
mkdir build && cd build && cmake -G Ninja .. && ninja 
```
