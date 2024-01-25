#ifndef LIMITORDER_H
#define LIMITORDER_H

#include "Order.hpp"

using std::string;

class LimitOrder: public Order {
public:
    LimitOrder(long id, Party* party, double price, int quantity, long ts, string side):
        Order(id, party, quantity, ts, side) {
        this->price = price;
    }

    double getPrice() const {
        return this->price;
    }

    virtual bool operator<(const LimitOrder& other) const {
        if (this->price != other.price) {
            return this->price < other.price;
        }
        else if (this->timestamp != other.timestamp) {
            return this->timestamp < other.timestamp;
        }
        return this->quantity > other.quantity;
    }

    virtual bool operator>(const LimitOrder& other) const {
        if (this->price != other.price) {
            return this->price > other.price;
        }
        else if (this->timestamp != other.timestamp) {
            return this->timestamp < other.timestamp;
        }
        return this->quantity > other.quantity;
    }

protected:
    double price;
};

#endif