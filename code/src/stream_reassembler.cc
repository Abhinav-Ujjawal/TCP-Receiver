#include "stream_reassembler.hh"

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    :_output(capacity)
{
    lowest_index=0;
    // m=vector<char>(capacity,'\0');
    this->capacity=capacity;
    d=std::deque<char>(capacity);
    vis=std::deque<int>(capacity,0);
    // b={0,0};
    b=false;
    rec=false;
    prevsize=0;
}


//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data,const size_t index,const bool eof){
    // int capacity=_output.buffer_size()+_output.remaining_capacity(); // equal to _output->size();
    // if (data.empty()/*|| index>=lowest_index+capacity || index+data.size()<=_output.bytes_read()*/){
    //     return;
    // }
    // if (eof) _output.end_input();
    // if (buffer.find(index)!=buffer.end()){
    //     if (buffer.find(index)->second.size()<g.size()){
    //         buffer[index]=g;
    //         int ind=index-_output.bytes_read();
    //         if ()
    //     }
    // }
    
    // string g=data;
    // if (lowest_index<index){
        
    // }
    
    int count=prevsize-_output.buffer_size();
    while(count--){
        d.push_back('\0');
        vis.push_back(0);
    }
    

    int p=0;
    for (auto &i:data){
        // cerr<<"The string is"<<i;
        if (index+p>=lowest_index){
            
            if (index<d.size()+lowest_index-p){
                d[index-lowest_index+p]=i;
                vis[index-lowest_index+p]=1;
                // d[index-lowest_index]=i;
                // vis[index-lowest_index]=1;

                // if(b.first==true && (b.second<=index+p)){
                //         b.first=0;
                // }
                if (b==true)
                    b=false;
            }
            
            else{
                // b={1,index+p};
                b=true;
            }
        }
        
        p++;
    }
    while(vis.size()){
        if (!vis[0])break;
        _output.write(string(1,d[0]));
        vis.pop_front();
        d.pop_front();
        lowest_index++;
    }
    
    
    
    prevsize=_output.buffer_size();
    if(!rec){
        if (eof&&!b){
            rec=true;
        }
    }
    
    
    if(rec && unassembled_bytes()==0){
        if(!b){
            _output.end_input();
        }
    }
    
    
    
    // done
    
    
    
    // if (lowest_index==index){
    //     int t=_output.write(g);
    //     if (t!=g.size()){
    //         return;
    //         // done
    //     }
    //     lowest_index+=t;
    //     string added="";
    //     int ind=lowest_index-_output.bytes_read();
    //     if (ind>capacity){
    //         return;
    //     }
    //     while(/*_output.remaining_capacity()!=0 && */m[lowest_index]!='\0'){
    //         added+=m[lowest_index];
    //         lowest_index++;
    //     }
    //     lowest_index+=_output.write(added);
    // }
    // else{
    //     buffer
    // }
    
    // if (eof) _output.end_input();
    
    // for (auto it=buffer.begin();it!=buffer.end();){
    //     if (_output.remaining_capacity()!=0 && it->first<=lowest_index){
    //         string fin="";
    //         for (int i=0;i<it->second.size();i++){
    //             if (i+it->first<lowest_index)
    //                 continue;
    //             fin+=it->second[i];
    //         }
            
    //         int t=_output.write(fin);
    //         lowest_index+=t;
    //         if (t!=fin.size()){
    //             // not enough space in _output
    //             // now merge properly
    //             if (buffer.find(lowest_index)!=buffer.end()){
    //                 if (buffer.find(lowest_index)->second.size()<size(fin.substr(t)))
    //                     buffer[lowest_index]=fin.substr(t);
    //             }
    //         }
    //         it=buffer.erase(it);
    //     }
    //     else{
    //         break;
    //     }
    // }
    // // while (buffer.find(lowest_index)!=buffer.end()){
    // //     int t=_output.write(buffer[lowest_index]);
    // //     buffer.erase(lowest_index);
    // //     lowest_index+=t;
    // // }
    // if (eof) _output.end_input();
    
}

size_t StreamReassembler::unassembled_bytes() const {
    // if (buffer.empty()) return 0;
    // // Total len of strings in buffer
    // int total_length=buffer.begin()->second.length();
    // auto it = buffer.begin();
    // auto p=it;
    // it++;
    int total_length=0;
    for(auto i: vis) if(i) total_length++;
    // for (;it!=buffer.end();it++){
    //     total_length+=it->second.length();
    //     // first add full length, now remove repeated part.
    //     if (p->first+p->second.size()>it->first){
    //         total_length-=p->first+p->second.size()-it->first;
    //     }
    //     p=it;
    // }
    return total_length;
}

bool StreamReassembler::empty() const {
    auto g=unassembled_bytes();
    return (g==0);
}

size_t StreamReassembler::ack_index() const { 
    return lowest_index;
}