
.SECONDEXPANSION:
main%: vec3.o dir3.o camera.o object.o light.o body.o scene.o $$@.o
	g++ vec3.o dir3.o object.o camera.o light.o body.o scene.o $@.o -o $@

vec3.o:
	g++ --std=c++17 -g -c common/vec3.cpp -o vec3.o
dir3.o:
	g++ --std=c++17 -g -c common/dir3.cpp -o dir3.o
camera.o:
	g++ --std=c++17 -g -c object/camera.cpp -o camera.o
object.o:
	g++ --std=c++17 -g -c object/object.cpp -o object.o
light.o:
	g++ --std=c++17 -g -c object/light.cpp -o light.o
body.o:
	g++ --std=c++17 -g -c object/body.cpp -o body.o
scene.o:
	g++ --std=c++17 -g -c scene/scene.cpp -o scene.o

main%.o:
	$(eval part=`echo $@ | sed -E 's/(.*).o/\1/'`)
	g++ --std=c++17 -g $(part).cpp -c -o $@

clean:
	rm *.o

