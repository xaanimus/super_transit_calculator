cd build/
echo "running cmake!"
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cp compile_commands.json ../compile_commands.json
