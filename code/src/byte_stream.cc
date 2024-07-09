#include "byte_stream.hh"
#include <algorithm>
using namespace std;
// You will need to add private members to the class declaration in `byte_stream.hh`
/* Replace all the dummy definitions inside the methods in this file. */
ByteStream::ByteStream(const size_t capa)
{
  this->size=capa;
  this->popped=0;
  this->written=0;
  this->stop=false;
  _error=false;
}

size_t ByteStream::write(const string &data) {
  if (_error){
    return 0;
  }
  if (this->stop==true)
    return 0;
  int i=0;
  while(i<data.size()){
    if (this->q.size()==this->size){
      break;
    }
    this->q.push_back(data[i]);
    i++;
  }
  this->written+=i;
  return i;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
  if (_error){
    return "";
  }
  string s;
  int i=0;
  deque<char>::const_iterator it=this->q.begin();
  // if(this->q.size()<len){
  //   this->_eof=true;
  //   return "";
  // }
  while(it!=this->q.end()&&i<len){
    s+=*it;
    it++;
    i++;
  }
  return s;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
  if (_error){
    return;
  }
  int i=0;
  int u=this->q.size();
  if(u<len){
    this->set_error();
    return;
  }
  while(i<u&&i<len){
    this->q.pop_front();
    i++;
  }
  this->popped+=i;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
  if (_error){
    return "";
  }
  string s;
  int i=0;
  deque<char>::const_iterator it=this->q.begin();
  int u=this->q.size( );
  if(u<len){
    set_error();
    return "";
  }
  while(/*i<u&&*/i<len){
    s+=this->q.front();
    this->q.pop_front();
    i++; 
  }
  this->popped+=i;
  return s;
}
/*
  Assumption - The error flag will not affect the implementation of the following functions
*/
void ByteStream::end_input() {
  this->stop=true;
}

bool ByteStream::input_ended() const {
  return this->stop;
}

size_t ByteStream::buffer_size() const {
  return this->q.size();
}

bool ByteStream::buffer_empty() const {
  return (this->q.size()==0);
}

bool ByteStream::eof() const {
  return (stop && this->q.size()==0);
}

size_t ByteStream::bytes_written() const {
  return this->written;
}

size_t ByteStream::bytes_read() const {
  return this->popped;
}

size_t ByteStream::remaining_capacity() const {
  return (this->size-this->q.size()); 
}
