//
//  Copyright 2025 Amr Saqr
//

#include "json_number_parser.h"

JsonParsingResult JsonNumberParser::TryParseNumber(HistoricalReader* reader) {
  // if stream ended, then there is nothing to read
  if (!reader->HasNextByte()) {
    return JsonParsingResult::kTypeMismatch;
  }

  // if we don't read the minus sign or a digit, then the next token can't be a
  // number
  char last_byte = reader->TestNextByteSkipWhitespace();
  if (last_byte != '-' && !isdigit(last_byte)) {
    return JsonParsingResult::kTypeMismatch;
  }

  // at this point, it has to be a number, either valid or invalid
  reader->GetNextByteSkipWhitespace();

  // this is needed to catch numbers with leading zeroes
  bool first_digit_is_zero = last_byte == '0';

  // read the first part, the +/- whole integer part
  while (reader->HasNextByte() &&
         isdigit(reader->TestNextByteSkipWhitespace())) {
    // if we read another digit and there was a leading zero, this is an invalid
    // number
    if (first_digit_is_zero) {
      return JsonParsingResult::kInvalidTypeMatch;
    }

    last_byte = reader->GetNextByteSkipWhitespace();
  }

  // if the last byte is still a minus sign, this is an invalid number
  if (last_byte == '-') {
    return JsonParsingResult::kInvalidTypeMatch;
  }

  // read the optional fraction part
  char possible_dot_byte = reader->TestNextByteSkipWhitespace();
  if (possible_dot_byte == '.') {
    last_byte = reader->GetNextByteSkipWhitespace();

    // read the optional fraction digits
    while (reader->HasNextByte() &&
           isdigit(reader->TestNextByteSkipWhitespace())) {
      last_byte = reader->GetNextByteSkipWhitespace();
    }
  }

  // read the optional exponent part
  char possible_e_byte = reader->TestNextByteSkipWhitespace();
  if (possible_e_byte == 'e' || possible_e_byte == 'E') {
    last_byte = reader->GetNextByteSkipWhitespace();

    // at this point, there has to be a number after the exponent sign, with an
    // optional +/- sign

    // read the optional +/- sign
    char possible_sign_byte = reader->TestNextByteSkipWhitespace();
    if (possible_sign_byte == '+' || possible_sign_byte == '-') {
      last_byte = reader->GetNextByteSkipWhitespace();
    }

    // read the exponent value
    while (reader->HasNextByte() &&
           isdigit(reader->TestNextByteSkipWhitespace())) {
      last_byte = reader->GetNextByteSkipWhitespace();
    }
  }

  if (last_byte == 'e' || last_byte == 'E' || last_byte == '-' ||
      last_byte == '+') {
    return JsonParsingResult::kInvalidTypeMatch;
  }

  return JsonParsingResult::kValidTypeMatch;
}
