g++ --std=c++17 -g -c object/camera.cpp -o camera.o &
g++ --std=c++17 -g -c object/object.cpp -o object.o &
g++ --std=c++17 -g -c object/light.cpp -o light.o &
g++ --std=c++17 -g -c object/body.cpp -o body.o &
g++ --std=c++17 -g -c scene/scene.cpp -o scene.o &
g++ --std=c++17 -g -c main3.cpp -o main3.o 
g++ object.o camera.o light.o body.o scene.o main3.o -o main3
