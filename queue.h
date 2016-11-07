/**
 Program:	queue.h
 Description:	Declarations for the Queue class.
 Author:	Brownell, Adam
 Date:		March 2016
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
/**
  Distinguish our queue from std::queue, enclose it in
 our very own namespace.
 */
namespace Pic10B {
    
    template <typename T>
    class queue {
    public:
        // Only one of 'the big 4' is needed here.
        inline queue() : theQueue() {}
        
        inline bool is_empty() const { return theQueue.empty(); }
        inline void enter( T value ) { theQueue.push(value); return; }
        inline T leave() { T r=theQueue.front(); theQueue.pop(); return r; }
        inline T peek() const { return theQueue.front(); }
        
    private:
        std::queue<T> theQueue;
    };
    
} 

#endif
