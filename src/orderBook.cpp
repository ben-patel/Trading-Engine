#include <iostream>
#include "orderBook.h"
#include "order.h"

// toggle debug, mostly for memory ATM.
//#define DEBUG_MODE

/* Allocate memory on heap when 'new' function is called*/
void* operator new(size_t size) {
    #ifdef DEBUG_MODE
    std::cout << "    DEBUG: ALLOCATING " << size << " BYTES OF MEMORY" << std::endl;
    #endif
    return malloc(size);
}   

/* Free memory when 'delete' is called */
void operator delete (void *memory) {
    #ifdef DEBUG_MODE
    std::cout << "    DEBUG: FREEING MEMORY" << std::endl;
    #endif
    free(memory);
}

namespace TradingEngine::Data::LimitOrderBook {
    BookEntry::BookEntry(TradingEngine::Order::Order order): order{ order }, next { nullptr } {}
}