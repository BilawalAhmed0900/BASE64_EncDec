clang -m32 -c -D LIBRARY_EXPORT -Ofast -Wall -Werror -o build\Base64_encdec_x86.o src\base64_encdec.c
clang -m32 -shared -D LIBRARY_EXPORT -Ofast -Wall -Werror -o build\Base64_encdec_x86.dll build\Base64_encdec_x86.o

clang -m64 -c -D LIBRARY_EXPORT -Ofast -Wall -Werror -o build\Base64_encdec_x64.o src\base64_encdec.c
clang -m64 -shared -D LIBRARY_EXPORT -Ofast -Wall -Werror -o build\Base64_encdec_x64.dll build\Base64_encdec_x64.o
pause