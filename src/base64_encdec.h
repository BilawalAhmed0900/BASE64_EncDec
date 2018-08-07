#include <stdint.h>

#if defined _WIN32 || defined __CYGWIN__
  #ifdef LIBRARY_EXPORT
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #ifdef LIBRARY_EXPORT
        #define DLL_PUBLIC __attribute__ ((visibility ("default")))       
    #else
        #define DLL_PUBLIC  __attribute__ ((visibility ("hidden")))
    #endif
  #else
    #define DLL_PUBLIC
  #endif
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
DLL_PUBLIC int32_t base64_encode(int8_t *input, size_t size, int8_t *output, size_t *capacity);
DLL_PUBLIC int32_t base64_decode(int8_t *input, size_t size, int8_t *output, size_t *capacity);

/* If message is NULL, message is printed to stdout */
DLL_PUBLIC void base64_geterrormessage(int32_t error_number, int8_t *message, size_t capacity);


#ifdef __cplusplus
}
#endif
