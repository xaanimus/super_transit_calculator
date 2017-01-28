cd build/

if [ "$1" = "debug" ]
then
    echo "running cmake with debug"
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug ..
else
    echo "running cmake"
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release ..
fi

cp compile_commands.json ../compile_commands.json

