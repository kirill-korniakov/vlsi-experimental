#ifndef __BASE64_H__
#define __BASE64_H__

#include "stdTypes.h"

class Base64
{
public:
  static const string Chars;

  static string Encode(unsigned char const* bytes_to_encode, unsigned int in_len);
  static string Decode(const char* encoded_string, unsigned int in_len);
};

#endif //__BASE64_H__