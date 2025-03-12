//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_PARSING_RESULT_H_
#define JSON_PARSING_RESULT_H_

enum class JsonParsingResult {
  kTypeMismatch = 0,
  kValidTypeMatch,
  kInvalidTypeMatch
};

#endif  // JSON_PARSING_RESULT_H_
