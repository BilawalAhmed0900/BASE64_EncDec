#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include "base64_encdec.h"

#define BITS_SET(howmany) ((1 << howmany) - 1)
#define TWO_BITS_SET  (BITS_SET(2))
#define FOUR_BITS_SET (BITS_SET(4))
#define SIX_BITS_SET  (BITS_SET(6))

#define LEFT_SHIFT(num, by)  (num << by)
#define RIGHT_SHIFT(num, by) (num >> by)

#pragma pack(push, 1)
struct int24_t
{
	uint8_t first, second, third;
};

/* To avoid naming conflict from stdint.h */
struct gint32_t
{
	uint8_t first, second, third, fourth;
};
#pragma pack(pop)

/*
	Fills gint32_t from int24_t
*/
void convert_24_to_32(struct int24_t *int24_T, struct gint32_t *gint32_T)
{
    memset(gint32_T, 0, sizeof(struct gint32_t));

    /*
		Take 6-bit and fill it to 8-bit(first zero-ing it)
    */ 
	gint32_T->first  = RIGHT_SHIFT(int24_T->first, 2);
    gint32_T->second = (LEFT_SHIFT((int24_T->first  & TWO_BITS_SET ), 4) | RIGHT_SHIFT(int24_T->second, 4));
    gint32_T->third  = (LEFT_SHIFT((int24_T->second & FOUR_BITS_SET), 2) | RIGHT_SHIFT(int24_T->third , 6));
    gint32_T->fourth = (int24_T->third & SIX_BITS_SET);
}

/*
	Fills int24_t from gint32_t
*/
void convert_32_to_24(struct int24_t *int24_T, struct gint32_t *gint32_T)
{
    memset(int24_T, 0, sizeof(struct int24_t));

    /*
		Take 8-bit and fill it to 6-bit removing 7th and 8th bit
    */
    
	int24_T->first  = (LEFT_SHIFT(gint32_T->first, 2) | RIGHT_SHIFT(gint32_T->second, 4));
    int24_T->second = (LEFT_SHIFT((gint32_T->second & FOUR_BITS_SET), 4) | RIGHT_SHIFT(gint32_T->third, 2));
    int24_T->third  = (LEFT_SHIFT((gint32_T->third  & TWO_BITS_SET),  6) | gint32_T->fourth);
}

/*
	Convert index from convert_24_to_32 to Base64 from table
*/
void index_to_base64(struct gint32_t *gint32_T)
{
	const char BASE64[] = 
	{
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
	};

	gint32_T->first  = BASE64[gint32_T->first];
	gint32_T->second = BASE64[gint32_T->second];
	gint32_T->third  = BASE64[gint32_T->third];
	gint32_T->fourth = BASE64[gint32_T->fourth];
}

size_t index_of(char *str, size_t length, char chr)
{
	size_t loop_counter = 0;
	for (; loop_counter < length; loop_counter++)
	{
		if (str[loop_counter] == chr)
		{
			return loop_counter;
		}
	}

	return -1;
}

/*
	Gives back padding count
*/
size_t base64_to_index(struct gint32_t *gint32_T)
{
	const char BASE64[] = 
	{
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
	};

	size_t padding_count = 0;

	padding_count = (gint32_T->first  != '=') ? padding_count : padding_count + 1;
	padding_count = (gint32_T->second != '=') ? padding_count : padding_count + 1;
	padding_count = (gint32_T->third  != '=') ? padding_count : padding_count + 1;
	padding_count = (gint32_T->fourth != '=') ? padding_count : padding_count + 1;

	gint32_T->first  = (gint32_T->first  != '=') ? index_of((char *)BASE64, sizeof(BASE64), gint32_T->first ) : 0;
	gint32_T->second = (gint32_T->second != '=') ? index_of((char *)BASE64, sizeof(BASE64), gint32_T->second) : 0;
	gint32_T->third  = (gint32_T->third  != '=') ? index_of((char *)BASE64, sizeof(BASE64), gint32_T->third ) : 0;
	gint32_T->fourth = (gint32_T->fourth != '=') ? index_of((char *)BASE64, sizeof(BASE64), gint32_T->fourth) : 0;

	return padding_count;
}

int32_t base64_encode(int8_t *input, size_t size, int8_t *output, size_t *capacity)
{
	/* 
		Each base64 digit represents exactly 6 bits of data. 
	   	Three 8-bit bytes (i.e., a total of 24 bits) can therefore be represented by four 6-bit(stored in 8-bit each) base64 digits 

	   	Source: https://en.wikipedia.org/wiki/Base64
	*/
	if (*capacity < (size * 4 / 3))
	{
		return BASE64_NOTENOUGHMEMORY;
	}

	size_t read = 0;
	size_t written = 0;
	while (size)
	{
		struct int24_t int24_T = { 0, 0, 0 };
		struct gint32_t gint32_T = { 0, 0, 0, 0};

		size_t padding_bytes = 0;
		size_t to_be_read = (size >= 3) ? sizeof(int24_T) : size;

		memcpy(&int24_T, &input[read], to_be_read);
		read += to_be_read; 	

		/* If even one bit is read then that byte has not to be padded */
		padding_bytes = 4 - ceil((double)(to_be_read * 8) / 6);
		size -= to_be_read;

		convert_24_to_32(&int24_T, &gint32_T);
		index_to_base64(&gint32_T);

		switch((ptrdiff_t)padding_bytes)
		{
			/* If we have 4 padding byte than 3, 2 and 1 have to be set too */
			case 4: gint32_T.first  = '=';
			case 3: gint32_T.second = '=';
			case 2: gint32_T.third  = '=';
			case 1: gint32_T.fourth = '=';
			case 0: break;

			/* This should never happen */
			default:
				return BASE64_PADDINGOVERFLOW;
		}

		memcpy(&output[written], &gint32_T, sizeof(gint32_T));
		written += sizeof(gint32_T);
	}

	*capacity = written;
	return BASE64_ISOKAY;
}

int32_t base64_decode(int8_t *input, size_t size, int8_t *output, size_t *capacity)
{
	if (*capacity < (size * 3 / 4))
	{
		return BASE64_NOTENOUGHMEMORY;
	}

	/*
		Base64 string is padded to multiple of 4 bytes
	*/
	if (size % 4 != 0)
	{
		return BASE64_SIZEINVALID;
	}

	size_t read = 0;
	size_t written = 0;
	while(size)
	{
		struct int24_t int24_T = { 0, 0, 0 };
		struct gint32_t gint32_T = { 0, 0, 0, 0 };

		memcpy(&gint32_T, &input[read], sizeof(struct gint32_t));
		read += sizeof(struct gint32_t);
		size -= sizeof(struct gint32_t);

		size_t padding_count = base64_to_index(&gint32_T);
		if (gint32_T.first == (uint8_t)-1 || gint32_T.second == (uint8_t)-1 
			|| gint32_T.third == (uint8_t)-1 || gint32_T.fourth == (uint8_t)-1)
		{
			/*
				-1 is given when digit is not found in the array of BASE64
			*/
			return BASE64_DIGITINVALID;
		}

		size_t to_write_bytes = 3 - ceil((double)(padding_count * 6) / 8);

		if ((ptrdiff_t)to_write_bytes > 0)
		{
			convert_32_to_24(&int24_T, &gint32_T);
			memcpy(&output[written], &int24_T, to_write_bytes);
			written += to_write_bytes;	
		}	
		else
		{
			/* Should never happen */
			return BASE64_PADDINGOVERFLOW;
		}
	}

	*capacity = written;
	return BASE64_ISOKAY;
}

void base64_geterrormessage(int32_t error_number, int8_t *message, size_t capacity)
{
	const char messages[][64] = 
	{
		"Error code 0: Everything alright",
		"Error code 1: Output doesn\'t have enough capacity",
		"Error code 2: Input size is not multiple of 4",
		"Error code 3: Data contain digit not found in BASE64 array",
		"Error code 4: Illegal padding occuring"
	};

	if (message != NULL)
	{
		strncpy_s((char *)message, capacity, messages[error_number], capacity);
	}
	else
	{
		fprintf(stdout, "%s\n", messages[error_number]);
	}
}