cd ./lib
del *.dll
cd ../
g++ -c -DDLL_OUT src/string_int.cpp -o lib/string_int.o
g++ -c -DDLL_OUT src/string_float.cpp -o lib/string_float.o
g++ -shared -o lib/ustring.dll lib/string_int.o lib/string_float.o -Wl,--out-implib,lib/ustring.a 
cd ./lib
del *.a
del *.o