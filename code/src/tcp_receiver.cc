#include "tcp_receiver.hh"

#include <algorithm>


using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader head = seg.header();
    // ...
    int eof=0;
    
    
    if (head.syn){
        _synReceived = true;
        _isn = head.seqno;
    }
    if (head.fin){
        _finReceived = true;
        eof=1;
    }
    Buffer d=(seg.payload());
    const string data = d.copy();
    
    
    // --- Hint ------
        // convert the seqno into absolute seqno
    uint64_t checkpoint = _reassembler.ack_index();
    uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
    uint64_t stream_idx = abs_seqno - _synReceived;
    // --- Hint ------  
    
    
    if ((!abs_seqno)&&_synReceived==1){
        return;
    }
    
    _reassembler.push_substring(data,stream_idx,eof);
    // ... 
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    
    if (_synReceived==false){
        return std::nullopt;
    }
    
    
    // // cout<<_reassembler.ack_index()+1<<"\n";
    // cerr<<_reassembler.ack_index()+1<<"\n";
    auto y=wrap(_reassembler.ack_index()+2,_isn);
    
    if (_synReceived==true && _finReceived==true){
        return {y};
    }
    else if (_synReceived==true && _finReceived==false){
        auto var=wrap(_reassembler.ack_index()+1,_isn);
        return {var};
    }
    // bruh ??
    return {};
}

size_t TCPReceiver::window_size() const { 
    return (_reassembler.stream_out().remaining_capacity());
}
