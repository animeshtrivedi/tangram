#include "base/sarray_binstream.hpp"

namespace xyz {

size_t SArrayBinStream::Size() const { return buffer_.size() - front_; }

void SArrayBinStream::AddBin(const char* bin, size_t sz) {
  buffer_.append_bytes(bin, sz);
}

void* SArrayBinStream::PopBin(size_t sz) {
  CHECK_LE(front_ + sz, buffer_.size());
  void* ret = &buffer_[front_];
  front_ += sz;
  return ret;
}

Message SArrayBinStream::ToMsg() const {
  Message msg;
  msg.AddData(buffer_);
  return msg;
}

void SArrayBinStream::FromMsg(const Message& msg) {
  CHECK_EQ(msg.data.size(), 1);
  FromSArray(msg.data[0]);
}

third_party::SArray<char> SArrayBinStream::ToSArray() {
  return buffer_;
}

}  // namespace xyz
