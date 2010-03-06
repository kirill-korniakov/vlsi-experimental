#include "Base64.h"

static inline bool is_base64(unsigned char c)
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

const string Base64::Chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/=";

string Base64::Encode(unsigned char const* bytes_to_encode, unsigned int in_len)
{
  string ret((in_len + 2) / 3 * 4, 0);

  unsigned int pos = 0;
  unsigned int index = 0;
  for (; pos < in_len - 3; pos += 3, index += 4)
  {
    ret[index] = Chars[(bytes_to_encode[pos] & 0xfc) >> 2];
    ret[index + 1] = Chars[((bytes_to_encode[pos] & 3) << 4) | ((bytes_to_encode[pos + 1] & 240) >> 4)];
    ret[index + 2] = Chars[((bytes_to_encode[pos + 1] & 15) << 2) | ((bytes_to_encode[pos + 2] & 0xc0) >> 6)];
    ret[index + 3] = Chars[bytes_to_encode[pos + 2] & 0x3f];
  }

  switch (in_len % 3)
  {
  case 1:
    ret[index] = Chars[(bytes_to_encode[pos] & 0xfc) >> 2];
    ret[index + 1] = Chars[(bytes_to_encode[pos] & 3) << 4];
    ret[index + 2] = Chars[0x40];
    ret[index + 3] = Chars[0x40];
    break;
  case 2:
    ret[index] = Chars[(bytes_to_encode[pos] & 0xfc) >> 2];
    ret[index + 1] = Chars[((bytes_to_encode[pos] & 3) << 4) | ((bytes_to_encode[pos + 1] & 240) >> 4)];
    ret[index + 2] = Chars[(bytes_to_encode[pos + 1] & 15) << 2];
    ret[index + 3] = Chars[0x40];
    break;
  }

  return ret;
}

string Base64::Decode(const char* encoded_string, unsigned int in_len)
{//WARNING: not tested
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  string ret;
  ret.reserve(in_len / 4 * 3);

  while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
  {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4)
    {
      for (i = 0; i <4; i++)
        char_array_4[i] = (unsigned char)Chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i)
  {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = (unsigned char)Chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++)
      ret += char_array_3[j];
  }

  return ret;
}