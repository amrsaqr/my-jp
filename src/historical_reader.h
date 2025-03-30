//
//  Copyright 2025 Amr Saqr
//

#ifndef HISTORICAL_READER_H_
#define HISTORICAL_READER_H_

#include <istream>
#include <list>
#include <string>
using std::istream;
using std::list;
using std::string;

class HistoricalReader {
 public:
  explicit HistoricalReader(istream* input_stream);

  bool HasNextByte() const;

  char TestNextByte(bool skip_white_space = false);

  char GetNextByte(bool skip_white_space = false);

  string GetHistory() const;

 private:
  void SkipWhitespace();
  void UpdateHistory(char byte);

  istream* input_stream_;
  list<char> history_;
  size_t history_length_no_ws = 0;
  const size_t kMaxHistoryLength = 21;
};

#endif  // HISTORICAL_READER_H_
