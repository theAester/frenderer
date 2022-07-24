g++ --std=c++17 -g -c common/vec3.cpp -o vec3.o &&
g++ --std=c++17 -g -c common/dir3.cpp -o dir3.o &&
g++ --std=c++17 -g -c object/camera.cpp -o camera.o &&
g++ --std=c++17 -g -c object/object.cpp -o object.o &&
g++ --std=c++17 -g -c object/light.cpp -o light.o &&
g++ --std=c++17 -g -c object/body.cpp -o body.o &&
g++ --std=c++17 -g -c scene/scene.cpp -o scene.o &&
g++ --std=c++17 -g -c main2.cpp -o main2.o &&
g++ vec3.o dir3.o object.o camera.o light.o body.o scene.o main2.o -o main2


