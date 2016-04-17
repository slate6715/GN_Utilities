#include "stdafx.h"
#include "OutStream.h"

namespace util {
    
    OutStream::OutStream() {
        
    }
    
    OutStream::~OutStream() {
        
    }
    
   OutStream::OutStream(int max_lvl) { 
        _max_lvl = max_lvl;
        _msg_lvl.push(1);       // Starting default level
   }
    
   OutStream::OutStream(const OutStream& orig):
                      _msg_lvl(orig._msg_lvl), 
                      _max_lvl(orig._max_lvl)
   {
    
   }

   void OutStream::push_msgpri(int lvl) {
       _msg_lvl.push(lvl);
   }
   
   void OutStream::pop_msgpri() {
       if (_msg_lvl.size() <= 1)
           throw util::RuntimeException("Attempted to pop last msg priority value");
       
       _msg_lvl.pop();
   }

   bool OutStream::setLogFile(const char *filename) {
       logfile.open(filename);
       if (!logfile.is_open())
           return false;
       return true;
   }
   

} // namespace util