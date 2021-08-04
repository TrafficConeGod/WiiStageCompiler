mkdir build
cd build
g++ -I ../Source -c $(find ../Source -name '*.cpp') -std=c++17 -fpermissive $1
g++ -I ../Source $(find . -name '*.o') -o app $1
cd ..