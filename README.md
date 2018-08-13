# BASE64_EncDec
Encode to and from Base64

# Functions

	int32_t base64_encode(int8_t *input, size_t size, int8_t *output, size_t *capacity);
	int32_t base64_decode(int8_t *input, size_t size, int8_t *output, size_t *capacity);
	void base64_geterrormessage(int32_t error_number, int8_t *message, size_t capacity);

# Compilation
Available make options: `all`, `main`, `example`, `clean`


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