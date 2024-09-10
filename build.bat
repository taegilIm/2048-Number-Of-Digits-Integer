g++ -c -DDEFINE_DLL include/Int_inf.cpp -o include/lib/Int_inf.o
g++ -c -DDEFINE_DLL include/N2Array.cpp -o include/lib/N2Array.o
g++ -shared -o include/lib/Int_inf_lib.dll include/lib/Int_inf.o include/lib/N2Array.o -Wl,--out-implib,include/lib/Int_inf_lib.a
pause