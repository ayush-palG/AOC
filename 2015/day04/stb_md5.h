#ifndef MD5_
#define MD5_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#define DIGEST_SIZE 16

const uint8_t shift_amount[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
			    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
			    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
			    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

const uint32_t key_constant[] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
			   0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
			   0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
			   0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
			   0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
			   0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
			   0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
			   0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
			   0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
			   0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
			   0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
			   0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
			   0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
			   0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
			   0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
			   0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

uint32_t get_non_linear_func(uint32_t b, uint32_t c, uint32_t d, size_t i);
size_t get_current_index(size_t i);
uint32_t left_rotate(uint32_t func, size_t rotation_count);
void change_endian(uint32_t *hex_input);

const char *get_message_digest_from_file(FILE *file);
const char *get_message_digest_from_memory(const char *padded_input_str, uint64_t input_size);
const char *md5(const char *input, char mode);

uint64_t get_file_size(FILE *file);
size_t get_padded_input_size(uint64_t size);
const char *write_padded_output_to_file(const char *input_file_path);
const char *write_padded_output_to_memory(const char *input_str);

#endif // MD5_

#ifdef MD5_IMPLEMENTATION

uint32_t get_non_linear_func(uint32_t b, uint32_t c, uint32_t d, size_t i)
{
  if (i <= 15) {
    return (b & c) | ((~b) & d);
  } else if (16 <= i && i <= 31) {
    return (d & b) | ((~d) & c);
  } else if (32 <= i && i <= 47) {
    return b ^ c ^ d;
  } else if (48 <= i && i <= 63) {
    return c ^ (b | (~d));
  }
  return 0;
}

size_t get_current_index(size_t i)
{
  if (i < 16) {
    return i;
  } else if (16 <= i && i < 32) {
    return (5*i + 1) % 16;
  } else if (32 <= i && i < 48) {
    return (3*i + 5) % 16;
  } else if (48 <= i && i < 64) {
    return (7 * i) % 16;
  }
  return 0;
}

uint32_t left_rotate(uint32_t func, size_t rotation_count)
{
  return ((func << (rotation_count & 31)) | ((func & 0xFFFFFFFF) >> (32 - (rotation_count & 31)))) & 0xFFFFFFFF;
}

void change_endian(uint32_t *hex_input)
{
  uint8_t arr[4] = {(*hex_input >> 8*3) & 0xFF,
		    (*hex_input >> 8*2) & 0xFF,
		    (*hex_input >> 8*1) & 0xFF,
		    (*hex_input >> 8*0) & 0xFF};

  memcpy(hex_input, arr, 4);
}

const char *get_message_digest_from_file(FILE *file)
{  
  uint64_t file_size = get_file_size(file);
  int32_t data[16];

  uint32_t a0 = 0x67452301;
  uint32_t b0 = 0xefcdab89;
  uint32_t c0 = 0x98badcfe;
  uint32_t d0 = 0x10325476;
  
  while ((uint64_t) ftell(file) != file_size) {
    fseek(file, 0, SEEK_CUR);
    fread(&data, sizeof(data), 1, file);
    
    uint32_t a = a0;
    uint32_t b = b0;
    uint32_t c = c0;
    uint32_t d = d0;

    for (size_t i = 0; i < 64; ++i) {
      uint32_t func = get_non_linear_func(b, c, d, i);
      size_t index = get_current_index(i);

      func += a + key_constant[i] + data[index];
      a = d;
      d = c;
      c = b;
      b += left_rotate(func, shift_amount[i]);
    }

    a0 += a;
    b0 += b;
    c0 += c;
    d0 += d;
  }

  change_endian(&a0);
  change_endian(&b0);
  change_endian(&c0);
  change_endian(&d0);

  char *digest_str = (char *) malloc(sizeof(char) * DIGEST_SIZE * 4);
  sprintf(digest_str, "%08x%08x%08x%08x", a0, b0, c0, d0);
  return digest_str;
}

const char *get_message_digest_from_memory(const char *padded_input_str, uint64_t input_size)
{ 
  int32_t data[16];
  size_t index = 0;

  uint32_t a0 = 0x67452301;
  uint32_t b0 = 0xefcdab89;
  uint32_t c0 = 0x98badcfe;
  uint32_t d0 = 0x10325476;
 
  while ((uint64_t) index != input_size) {
    memcpy(&data, &padded_input_str[index], 64);
    index += 64;
    
    uint32_t a = a0;
    uint32_t b = b0;
    uint32_t c = c0;
    uint32_t d = d0;

    for (size_t i = 0; i < 64; ++i) {
      uint32_t func = get_non_linear_func(b, c, d, i);
      size_t index = get_current_index(i);

      func += a + key_constant[i] + data[index];
      a = d;
      d = c;
      c = b;
      b += left_rotate(func, shift_amount[i]);
    }

    a0 += a;
    b0 += b;
    c0 += c;
    d0 += d;
  }

  change_endian(&a0);
  change_endian(&b0);
  change_endian(&c0);
  change_endian(&d0);

  char *digest_str = (char *) malloc(sizeof(char) * DIGEST_SIZE * 4);
  sprintf(digest_str, "%08x%08x%08x%08x", a0, b0, c0, d0);
  return digest_str;
}

const char *md5(const char *input, char mode)
{
  const char *digest;
  
  if (mode == 'f') {
    const char *input_file_path = write_padded_output_to_file(input);
    FILE *file = fopen(input_file_path, "rb");
    if (file == NULL) {
      fprintf(stderr, "ERROR: could not open output file %s: %s\n", input_file_path, strerror(errno));
      exit(1);
    }
  
    digest = get_message_digest_from_file(file);
  } else if (mode == 'm') {
    const char *input_str = write_padded_output_to_memory(input);
    uint64_t str_length = get_padded_input_size(strlen(input));
    digest = get_message_digest_from_memory(input_str, str_length);
  }
  
  return digest;
}

uint64_t get_file_size(FILE *file)
{
  if (fseek(file, 0, SEEK_END) < 0) {
    fprintf(stderr, "ERROR: could not read file %s\n", strerror(errno));
    exit(1);
  }

  uint64_t file_size = ftell(file);
  if (fseek(file, 0, SEEK_SET) < 0) {
    fprintf(stderr, "ERROR: could not read file %s\n", strerror(errno));
    exit(1);
  }

  return file_size;
}
 
uint64_t get_padded_input_size(uint64_t size)
{
  uint64_t padding_size = 64 - (size + 8) % 64;
  if (padding_size == 0) padding_size += 64;

  return size + padding_size + 8;
}

const char *write_padded_output_to_file(const char *input_file_path)
{
  FILE *input_file = fopen(input_file_path, "rb");
  if (input_file == NULL) {
    fprintf(stderr, "ERROR: could not open output file %s: %s\n", input_file_path, strerror(errno));
    exit(1);
  }

  const char *output_file_path = "padded_input";
  FILE *output_file = fopen(output_file_path, "wb");
  if (output_file == NULL) {
    fprintf(stderr, "ERROR: could not open output file %s: %s\n", output_file_path, strerror(errno));
    exit(1);
  }

  uint64_t file_size = get_file_size(input_file);
  size_t padding_size = 64 - (file_size + 8) % 64;
  if (padding_size == 0) padding_size += 64;

  char ch;
  while ((ch = fgetc(input_file)) != EOF) {
    fputc(ch, output_file);
  }
  
  int8_t padding_value = 0x80;
  while (padding_size--) {
    fputc(padding_value, output_file);
    padding_value = 0x00;
  }

  file_size *= 8;
  fwrite(&file_size, sizeof(file_size), 1, output_file);
  
  fclose(input_file);
  fclose(output_file);

  return output_file_path;
}

const char *write_padded_output_to_memory(const char *input_str)
{
  uint64_t str_length = strlen(input_str);
  uint64_t padded_input_size = get_padded_input_size(str_length);
  char *padded_input_str = (char *) malloc(sizeof(char) * padded_input_size);

  memcpy(padded_input_str, input_str, str_length);
  int8_t padding_value = 0x80;
  for (size_t i = 0; i < (padded_input_size - str_length - 8); ++i) {
    padded_input_str[str_length+i] = (char) padding_value;
    padding_value = 0x00;
  }

  str_length *= 8;
  memcpy(&padded_input_str[padded_input_size-8], &str_length, 8);

  return padded_input_str;
}

#endif // MD5_IMPLEMENTATION
