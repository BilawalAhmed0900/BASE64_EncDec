# BASE64_EncDec
Encode to and from Base64

# Functions

	int32_t base64_encode(int8_t *input, size_t size, int8_t *output, size_t *capacity);
	int32_t base64_decode(int8_t *input, size_t size, int8_t *output, size_t *capacity);
	void base64_geterrormessage(int32_t error_number, int8_t *message, size_t capacity);

# Compilation

## Clang
	clang -m32 -c -D LIBRARY_EXPORT -Ofast -Wall -Werror -o build\Base64_encdec_x86.o src\base64_encdec.c
	clang -m32 -shared -D LIBRARY_EXPORT -Ofast -Wall -Werror -o build\Base64_encdec_x86.dll build\Base64_encdec_x86.o

## GCC
	gcc -m32 -c -D LIBRARY_EXPORT -Ofast -Wall -Werror -o build\Base64_encdec_x86.o src\base64_encdec.c
	gcc -m32 -shared -D LIBRARY_EXPORT -Ofast -Wall -Werror -o build\Base64_encdec_x86.dll build\Base64_encdec_x86.o

# Linking to binary

## Clang
	clang -c -o file.o file.c
	clang -o file.exe file.o Base64_encdec_x86.o Base64_encdec_x86.lib

## GCC
	gcc -c -o file.o file.c
	gcc -o file.exe file.o Base64_encdec_x86.o Base64_encdec_x86.o


# Example
A example is written in the example folder

## Usage
	Usage: BASE64_ENC_DEC (-f/-s) [-d] input [output]

	Positional argument:
	  -f:            file mode
	  -s:            string mode
	  input:         input string or file
	  output:        output file(string will be written to console)

	Optional argument:
	  -d:            decode from BASE64