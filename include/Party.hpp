#ifndef PARTY_H
#define PARTY_H

#include "Side.hpp"
#include <string>
#include <vector>

class Party {
public:
    Party(std::string name);
    void record_transaction(int quantity, double price, long ts, Side side);
    void print();
    void analyze();

    int getNetPosition() const {
        return this->net_position;
    }

private:
    std::string name;
    std::vector<std::pair<long, int>> historical_positions;     // Timestamp, net position pairs
    int net_position;
    long volume;
    double PnL;
};

#endif