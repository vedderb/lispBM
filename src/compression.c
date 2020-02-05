/*
    Copyright 2019 Joel Svensson	svenssonjoel@yahoo.se

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "compression.h"

#define  KEY  0
#define  CODE 1

/* The codes are generated using python script in utils directory */
#define NUM_CODES 66
#define MAX_KEY_LENGTH 7
#define MAX_CODE_LENGTH 7
char *codes[NUM_CODES][2] = {
    { "9", "101101" },
    { "8", "101000" },
    { "7", "101001" },
    { "6", "100111" },
    { "5", "101100" },
    { "4", "101010" },
    { "3", "110000" },
    { "2", "101011" },
    { "1", "101111" },
    { "0", "101110" },
    { " ", "110001" },
    { "'", "111011" },
    { "\\", "110010" },
    { "\"", "110011" },
    { "#", "111100" },
    { ".", "110110" },
    { ">", "110111" },
    { "<", "111000" },
    { "=", "111101" },
    { "/", "111001" },
    { "*", "110100" },
    { "-", "110101" },
    { "+", "111010" },
    { "nil", "000001" },
    { "))))", "001010" },
    { ")))", "001011" },
    { "))", "000010" },
    { ")", "000011" },
    { "((", "1111110" },
    { "(", "1111111" },
    { "cdr", "001000" },
    { "car", "001001" },
    { "cons", "000100" },
    { "let", "000101" },
    { "define", "001100" },
    { "closure", "1111100" },
    { "quote", "1111101" },
    { "list", "000110" },
    { "if", "000111" },
    { "lambda", "000000" },
    { "z", "001110" },
    { "y", "001111" },
    { "x", "011111" },
    { "w", "010100" },
    { "v", "100000" },
    { "u", "100001" },
    { "t", "011100" },
    { "s", "100100" },
    { "r", "011001" },
    { "q", "100010" },
    { "p", "100110" },
    { "o", "001101" },
    { "n", "010110" },
    { "m", "010000" },
    { "l", "011110" },
    { "k", "010101" },
    { "j", "010011" },
    { "i", "011000" },
    { "h", "100011" },
    { "g", "011011" },
    { "f", "010001" },
    { "e", "010010" },
    { "d", "100101" },
    { "c", "011010" },
    { "b", "011101" },
    { "a", "010111" }
    };


int length_max_compressible() {
  return MAX_KEY_LENGTH;
}

int length_max_decompressible() {
  return MAX_CODE_LENGTH;
}

int match_longest_key(char *string) {

  int longest_match_ix = -1;
  int longest_match_length = 0;
  int n = strlen(string);

  for (int i = 0; i < NUM_CODES; i ++) {
    int s_len = strlen(codes[i][KEY]);
    if (s_len <= n) {
      if (strncmp(codes[i][KEY], string, s_len) == 0) {
	if (s_len > longest_match_length) {
	  longest_match_ix = i;
	  longest_match_length = s_len;
	}
      }
    }
  }
  return longest_match_ix;
}

int match_longest_code(char *string, uint32_t start_bit, uint32_t total_bits) {

  uint32_t bits_left = total_bits - start_bit;
  int longest_match_ix = -1;
  int longest_match_length = 0;

  for (int i = 0; i < NUM_CODES; i++) {
    int s_len = strlen(codes[i][CODE]);
    if ((unsigned int)s_len <= bits_left) {
      bool match = true;
      for (uint32_t b = 0; b < (unsigned int)s_len; b ++) {
	uint32_t byte_ix = (start_bit + b) / 8;
	uint32_t bit_ix  = (start_bit + b) % 8;

	char *code_str = codes[i][CODE];

	if ((((string[byte_ix] & (1 << bit_ix)) ? '1' : '0') !=
	      code_str[b])) {
	  match = false;
	}
      }
      if (match && (s_len > longest_match_length)) {
	longest_match_length = s_len;
	longest_match_ix = i;
      }
    }
  }
  return longest_match_ix;
}

int compressed_length(char *string) {
  unsigned int i = 0;

  unsigned int n = strlen(string);
  int comp_len = 0; // in bits

  bool string_mode = false;
  bool gobbling_whitespace = false;

  while (i < n) {
    if (string_mode) {
      if (string[i] == '\"'  &&
	  !(string[i-1] == '\\')) {
	string_mode = false;
	comp_len += 8;
	i++;
      } else {
	comp_len += 8;
	i++;
      }

    } else {

      // Gobble up any comments
      if (string[i] == ';' ) {
	while (string[i] && string[i] != '\n') {
	  i++;
	}
	continue;
      }

      if ( string[i] == '\n' ||
	   string[i] == ' '  ||
	   string[i] == '\t' ||
	   string[i] == '\r') {
	gobbling_whitespace = true;
	i ++;
	continue;
      } else if (gobbling_whitespace) {
	gobbling_whitespace = false;
	i--;
      }

      if (string[i] == '\"') string_mode = true;

      int ix;
      if (isspace(string[i])) {
	ix = match_longest_key(" ");
      } else {
	ix = match_longest_key(string + i);
      }

      if (ix == -1)return -1;
      int code_len = strlen(codes[ix][1]);
      comp_len += code_len;
      i += strlen(codes[ix][0]);
    }
  }
  return comp_len;
}

void set_bit(char *c, char bit_pos, bool set) {
  char bval = 1 << bit_pos;
  if (set) {
    *c = *c | bval;
  } else {
    *c = *c & ~bval;
  }
}

void emit_string_char_code(char *compressed, char c, int *bit_pos) {

  for (int i = 0; i < 8; i ++) {
    int byte_ix = (*bit_pos) / 8;
    int bit_ix  = (*bit_pos) % 8;
    bool s = (c & (1 << i));
    set_bit(&compressed[byte_ix], bit_ix, s);
    *bit_pos = *bit_pos + 1;
  }
}

void emit_code(char *compressed, char *code, int *bit_pos) {
  int n = strlen(code);

  for (int i = 0; i < n; i ++) {
    int byte_ix = (*bit_pos) / 8;
    int bit_ix  = (*bit_pos) % 8;
    bool s = (code[i] == '1');
    set_bit(&compressed[byte_ix], bit_ix, s);
    *bit_pos = *bit_pos + 1;
  }
}

void emit_key(char *dest, char *key, int nk, uint32_t *char_pos) {

  for (int i = 0; i < nk; i ++) {
    dest[*char_pos] = key[i];
    *char_pos = *char_pos + 1;
  }
}

char read_character(char *src, uint32_t *bit_pos) {

  char c = 0;

  for (int i = 0; i < 8; i ++) {
    int byte_ix = (*bit_pos)/8;
    int bit_ix  = (*bit_pos)%8;
    bool s = src[byte_ix] & (1 << bit_ix);
    set_bit(&c, i, s);
    *bit_pos = *bit_pos + 1;
  }
  return c;
}

char *compression_compress(char *string, uint32_t *res_size) {

  uint32_t c_size_bits = compressed_length(string);
  uint32_t c_size_bytes = 4 + (c_size_bits/8+1);
  uint32_t header_value = c_size_bits;

  if (header_value == 0) return NULL;

  char *compressed = malloc(c_size_bytes);
  if (!compressed) return NULL;
  memset(compressed, 0, c_size_bytes);
  *res_size = c_size_bytes;
  int bit_pos = 0;

  compressed[0] = (unsigned char)header_value;
  compressed[1] = (unsigned char)(header_value >> 8);
  compressed[2] = (unsigned char)(header_value >> 16);
  compressed[3] = (unsigned char)(header_value >> 24);
  bit_pos = 32;

  bool string_mode = false;
  bool gobbling_whitespace = false;
  uint32_t n = strlen(string);
  uint32_t i = 0;

  while (i < n) {
    if (string_mode) {

      if (string[i] == '\"' &&
	  !(string[i-1] == '\\')) {
	emit_string_char_code(compressed, '\"', &bit_pos);
	i ++;
	string_mode = false;
	continue;
      } else {
	emit_string_char_code(compressed, string[i], &bit_pos);
	i++;
      }

    } else {

      // Gobble up any comments
      if (string[i] == ';' ) {
	while (string[i] && string[i] != '\n') {
	  i++;
	}
	continue;
      }

      // gobble up whitespaces
      if ( string[i] == '\n' ||
	   string[i] == ' '  ||
	   string[i] == '\t' ||
	   string[i] == '\r') {
	gobbling_whitespace = true;
	*(string + i) = ' ';
	i ++;
	continue;
      } else if (gobbling_whitespace) {
	gobbling_whitespace = false;
	i--;
      }

      /* Compress string-starting " character */
      if (string[i] == '\"') {
	string_mode = true;
      }
      int ix = match_longest_key(&string[i]);

      if (ix == -1) return NULL;

      emit_code(compressed, codes[ix][CODE], &bit_pos);

      i += strlen(codes[ix][0]);
    }
  }

  return compressed;
}


void compression_init_state(decomp_state *s, char *src) {
  memcpy(&s->compressed_bits, src, 4);
  s->i = 32;
  s->string_mode = false;
  s->last_string_char = 0;
  s->src = src;
}

uint32_t compression_decompress_incremental(decomp_state *s, char *dest_buff, uint32_t dest_n) {

  memset(dest_buff, 0, dest_n);
  uint32_t char_pos = 0;

  if (s->i < s->compressed_bits + 32) {
     if (s->string_mode) {
      char c = read_character(s->src, &s->i);
      if (c == '\"') {
	if (s->last_string_char != '\\') {
	  s->string_mode = false;
	}
      }
      s->last_string_char = c;
      dest_buff[0] = c;
      return 1;
    }

    int ix = match_longest_code(s->src, s->i, (s->compressed_bits + 32));
    if (ix == -1) {
      return 0;
    }

    if( strlen(codes[ix][KEY]) == 1 &&
	strncmp(codes[ix][KEY], "\"", 1) == 0) {
      s->string_mode = true;
      s->last_string_char = 0;
    }

    int n_bits_decoded = strlen(codes[ix][CODE]);
    emit_key(dest_buff, codes[ix][KEY], strlen(codes[ix][KEY]), &char_pos);
    s->i+=n_bits_decoded;
    return char_pos;

  } else {
    return 0;
  }

}

bool compression_decompress(char *dest, uint32_t dest_n, char *src) {

  bool ret = true;
  uint32_t char_pos = 0;

  char dest_buff[32];
  uint32_t num_chars = 0;
  decomp_state s;

  memset(dest, 0, dest_n);

  compression_init_state(&s, src);

  while ((num_chars = compression_decompress_incremental(&s, dest_buff, 32))) {

    for (uint32_t i = 0; i < num_chars; i ++) {
      dest[char_pos++] = dest_buff[i];
    }
  }
  return ret;
}
