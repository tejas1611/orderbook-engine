#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>

#include "Side.hpp"
#include "Party.hpp"
using std::string;

class OrderBook;

class Order {
public:
    Order(long id, Party* party, int quantity, long ts, string side) {
        this->id = id;
        this->party = party;
        this->quantity = quantity;
        this->timestamp = ts;
        this->side = (side == "BUY") ? Side::buy : Side::sell;
    }

    void make_transaction(float matchedPrice, int matchedQuantity, long matchedTimestamp) {
        this->quantity -= matchedQuantity;
        this->party->record_transaction(matchedQuantity, matchedPrice, matchedTimestamp, this->side);
    }

    long getId() const { return this->id; }
    Party* getParty() { return this->party; }
    int getQuantity() const { return this->quantity; }
    long getTimestamp() const { return this->timestamp; }
    Side getSide() const { return this->side; }

protected:
    long id;
    long timestamp;
    int quantity;
    Side side;
    Party* party;
};

#endif