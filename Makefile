
.SECONDEXPANSION:
main%: build/vec3.o build/dir3.o build/camera.o build/object.o build/light.o build/body.o build/scene.o build/$$@.o
	g++ build/vec3.o build/dir3.o build/object.o build/camera.o build/light.o build/body.o build/scene.o build/$@.o -o build/$@

build/vec3.o:
	g++ --std=c++17 -g -c common/vec3.cpp -o build/vec3.o
build/dir3.o:
	g++ --std=c++17 -g -c common/dir3.cpp -o build/dir3.o
build/camera.o:
	g++ --std=c++17 -g -c object/camera.cpp -o build/camera.o
build/object.o:
	g++ --std=c++17 -g -c object/object.cpp -o build/object.o
build/light.o:
	g++ --std=c++17 -g -c object/light.cpp -o build/light.o
build/body.o:
	g++ --std=c++17 -g -c object/body.cpp -o build/body.o
build/scene.o:
	g++ --std=c++17 -g -c scene/scene.cpp -o build/scene.o

build/main%.o:
	$(eval part=`echo $@ | sed -E 's/.*\/(.*).o/\1/'`)
	g++ --std=c++17 -g $(part).cpp -c -o $@

clean:
	rm build/*.o

