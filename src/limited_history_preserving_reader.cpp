//
//  Copyright 2025 Amr Saqr
//

#include "limited_history_preserving_reader.h"

#include <cstdio>
#include <string>

using std::ifstream;
using std::string;

LimitedHistoryPreservingReader::LimitedHistoryPreservingReader(
    ifstream* input_stream)
    : input_stream_(input_stream) {}

bool LimitedHistoryPreservingReader::HasNextByte() const {
  return input_stream_->good() && !input_stream_->eof();
}

char LimitedHistoryPreservingReader::TestNextByte(bool skip_white_space) {
  if (skip_white_space) {
    SkipWhitespace();
  }

  return static_cast<char>(input_stream_->peek());
}

char LimitedHistoryPreservingReader::GetNextByte(bool skip_white_space) {
  if (skip_white_space) {
    SkipWhitespace();
  }

  char byte = static_cast<char>(input_stream_->get());
  UpdateHistory(byte);
  return byte;
}

string LimitedHistoryPreservingReader::GetHistory() const {
  string strHistory(history_.begin(), history_.end());
  return strHistory;
}

void LimitedHistoryPreservingReader::SkipWhitespace() {
  while (HasNextByte() && isspace(input_stream_->peek())) {
    UpdateHistory(static_cast<char>(input_stream_->get()));
  }
}

void LimitedHistoryPreservingReader::UpdateHistory(char byte) {
  if (byte == EOF) {
    return;
  }

  history_.emplace_back(byte);

  if (!isspace(byte)) {
    ++history_length_no_ws;
  }

  if (history_length_no_ws > kMaxHistoryLength) {
    while (isspace(history_.front())) {
      history_.pop_front();
    }

    history_.pop_front();
    --history_length_no_ws;
  }
}
