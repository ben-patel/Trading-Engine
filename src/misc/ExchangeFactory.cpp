#include <iostream>
#include <fstream>
#include <string>
#include "exchangeFactory.hpp"
using namespace TradingEngine;

/* Nasdaq: Only covers stocks with market cap > $2B USD */
/* India Taiwan: Only covers mega cap stocks */
Exchange::Exchange* Util::ExchangeFactory::getExchange(std::string exchangeFile) {
    Exchange::Exchange& exchange = Exchange::Exchange::getInstance();
    std::ifstream file("../exchanges/" + exchangeFile);

    if (!file.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
        return nullptr;
    }

    std::string symbol;
    while (std::getline(file, symbol)) {
        exchange.addInstrument(symbol.c_str());
    }

    file.close();
    return &exchange;
}
