#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../src/base64_encdec.h"

#define APP_NAME "BASE64_ENC_DEC"

enum LOCAL_ERROR
{
    ISOKAY, EXTRA_PARAMETERS, STRING_FILE_MODE_BOTHUSED_NOTUSED, STRING_NOTENOUGH_MEMORY,
    FILE_INPUT_NOT_OPEN, FILE_OUTPUT_NOT_OPEN, BUFFER_NOTENOUGH_MEMORY
};

void print_help()
{
    fprintf(stdout, "Usage: " APP_NAME " (-f/-s) [-d] input [output]\n"
                    "\n"
                    "Positional argument:\n"
                    "  -f:\t\t file mode\n"
                    "  -s:\t\t string mode\n"
                    "  input:\t input string or file\n"
                    "  output:\t output file(string will be written to console)\n"
                    "\n"
                    "Optional argument:\n"
                    "  -d:\t\t decode from BASE64\n");
}

int8_t *file_read(FILE *pointer, size_t *read_bytes)
{
    int32_t file_size;
    fseek(pointer, 0, SEEK_END);
    file_size = ftell(pointer);
    fseek(pointer, 0, SEEK_SET);

    if (file_size > SIZE_MAX)
    {
        file_size = (int32_t)SIZE_MAX;
    }

    int8_t *buffer = (int8_t *)malloc((size_t)file_size);
    fread((char *)buffer, sizeof(int8_t), (size_t)file_size, pointer);

    *read_bytes = file_size;
    return buffer;
}

int32_t encode(int8_t *input, size_t size, int8_t *output, size_t *capacity, bool is_string_mode)
{
    if (is_string_mode)
    {
        int32_t result = base64_encode(input, strlen((char *)input), output, capacity);
        return result;
    }
    else
    {
        int32_t result = base64_encode(input, size, output, capacity);
        return result;
    }

    return -1;
}

int32_t decode(int8_t *input, size_t size, int8_t *output, size_t *capacity, bool is_string_mode)
{
    if (is_string_mode)
    {
        int32_t result = base64_decode(input, strlen((char *)input), output, capacity);
        return result;
    }
    else
    {
        int32_t result = base64_decode(input, size, output, capacity);
        return result;
    }

    return -1;
}

size_t roundUp(size_t numToRound, size_t multiple)
{
    if (multiple == 0)
        return numToRound;

    size_t remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    return numToRound + multiple - remainder;
}

int main(int argc, char const *argv[])
{
    bool file_mode = false;
    bool string_mode = false;
    bool is_decode = false;
    char *input = NULL;
    char *output = NULL;

    if (argc == 1)
    {
        print_help();
        return ISOKAY;
    }

    /* At zero, binary name is present */
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'f':
                    file_mode = true;
                    break;
                case 's':
                    string_mode = true;
                    break;
                case 'd':
                    is_decode = true;
                    break;

                default:
                    fprintf(stderr, "Extra parameter: %s\n", argv[i]);
                    return EXTRA_PARAMETERS;
            }
        }
        else if (input == NULL)
        {
            input = (char *)argv[i];
        }
        else if (output == NULL)
        {
            output = (char *)argv[i];
        }
        else
        {
            fprintf(stderr, "Extra parameter: %s\n", argv[i]);
            return EXTRA_PARAMETERS;
        }
    }

    /* Exclusive OR(XOR) is false when both are true or both are false */
    if ((file_mode ^ string_mode) == false)
    {
        fprintf(stderr, "Both string and file mode is used or both unused\n");

        print_help();
        return STRING_FILE_MODE_BOTHUSED_NOTUSED;
    }

    FILE *inptr = NULL, *outptr = NULL;
    if (file_mode == true)
    {
	inptr = fopen(input, "rb");
        if (inptr == NULL)
        {
            fprintf(stderr, "Cannot open input file\n");
            return FILE_INPUT_NOT_OPEN;
        }

	outptr = fopen(output, "wb");
        if (outptr == NULL)
        {
            fprintf(stderr, "Cannot create output file\n");
            return FILE_OUTPUT_NOT_OPEN;
        }
    }

    size_t input_size, output_capacity;
    int8_t *in_buffer = NULL, *out_buffer = NULL;
    if (file_mode == true)
    {
        in_buffer = file_read(inptr, &input_size);
    }
    else if (string_mode == true)
    {
        input_size = strlen(input);
        in_buffer = (int8_t *)input;
    }

    output_capacity = roundUp(input_size * 4 / 3, 4) + 1;
    out_buffer = (int8_t *)malloc(output_capacity);
    if (out_buffer == NULL)
    {
        fprintf(stderr, "Cannot allocate memory for output buffer\n");
        return BUFFER_NOTENOUGH_MEMORY;
    }
    memset(out_buffer, 0, output_capacity);

    int32_t result;
    if (is_decode == false)
    {
        result = encode(in_buffer, input_size, out_buffer, &output_capacity, string_mode);
    }
    else
    {
        result = decode(in_buffer, input_size, out_buffer, &output_capacity, string_mode);
    }

    if (result != BASE64_ISOKAY)
    {
        base64_geterrormessage(result, NULL, 0);
        return result;
    }

    if (file_mode == true)
    {
        fwrite(out_buffer, sizeof(int8_t), output_capacity, outptr);
    }
    else if (string_mode == true)
    {   
        fprintf(stdout, "%s\n\n", out_buffer);
    }
    
    free(out_buffer);
    if (string_mode == false)
    {
        free(in_buffer);
    }
    
    if (file_mode == true)
    {
        fclose(outptr);
        fclose(inptr);
    }

    base64_geterrormessage(ISOKAY, NULL, 0);
    return ISOKAY;
}
