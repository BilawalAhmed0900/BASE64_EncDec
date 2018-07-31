#include <stdint.h>

#ifdef LIBRARY_EXPORT
	#define LIBRARY __declspec(dllexport)
#else
	#define LIBRARY __declspec(dllimport)
#endif

enum DLL_ERROR
{
	BASE64_ISOKAY, BASE64_NOTENOUGHMEMORY, BASE64_SIZEINVALID, 
	BASE64_DIGITINVALID, BASE64_PADDINGOVERFLOW
};

#ifdef __cplusplus
extern "C"
{
#endif


/*
	capacity will then be updated to the size written to output
	The last \0 should not be added to size in C-style string, espacially in decode
*/
LIBRARY int32_t base64_encode(int8_t *input, size_t size, int8_t *output, size_t *capacity);
LIBRARY int32_t base64_decode(int8_t *input, size_t size, int8_t *output, size_t *capacity);

/* If message is NULL, message is printed to stdout */
LIBRARY void    base64_geterrormessage(int8_t errno, int8_t *message, size_t capacity);


#ifdef __cplusplus
}
#endif