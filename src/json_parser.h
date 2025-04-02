//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include <string>

class JsonParser {
 public:
  explicit JsonParser(const char* file_path);
  bool Parse(std::string* error_output) const;

 private:
  std::string file_path_;
};

#endif  // JSON_PARSER_H_
