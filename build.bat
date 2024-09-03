g++ -c -DDEFINE_DLL include/Int_2048.cpp -o lib/Int_2048.o
g++ -c -DDEFINE_DLL include/N2Array.cpp -o lib/N2Array.o
g++ -shared -o lib/Int_2048_lib.dll lib/Int_2048.o lib/N2Array.o -Wl,--out-implib,lib/Int_2048_lib.a
pause