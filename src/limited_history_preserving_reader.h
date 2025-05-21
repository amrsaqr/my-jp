//
//  Copyright 2025 Amr Saqr
//

#ifndef LIMITED_HISTORY_PRESERVING_READER_H_
#define LIMITED_HISTORY_PRESERVING_READER_H_

#include <fstream>
#include <list>
#include <string>

class LimitedHistoryPreservingReader {
 public:
  explicit LimitedHistoryPreservingReader(std::ifstream* input_stream);

  bool HasNextByte() const;

  char TestNextByte(bool skip_white_space = false);

  char GetNextByte(bool skip_white_space = false);

  std::string GetHistory() const;

 private:
  void SkipWhitespace();
  void UpdateHistory(char byte);

  std::ifstream* input_stream_;
  std::list<char> history_;
  size_t history_length_no_ws = 0;
  const size_t kMaxHistoryLength = 21;
};

#endif  // LIMITED_HISTORY_PRESERVING_READER_H_
