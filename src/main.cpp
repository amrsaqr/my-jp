//  Copyright 2025 Amr Saqr

#include <iostream>
#include <string>

#include "json_parser.h"
using std::cout;
using std::endl;
using std::string;

int main(const int argc, const char* const argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <input_file_path>" << endl;
    return 1;
  }

  JsonParser json_parser;
  string error_output;
  const string file_path(argv[1]);

  bool successful_parsing = json_parser.Parse(file_path, &error_output);
  if (successful_parsing) {
    cout << "Valid JSON" << endl;
  } else {
    cout << error_output << endl;
  }

  return successful_parsing ? 0 : 1;
}
