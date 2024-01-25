#ifndef AUCTIONORDER_H
#define AUCTIONORDER_H

#include "LimitOrder.hpp"

using std::string;

class AuctionOrder: public LimitOrder {
public:
    AuctionOrder(long id, Party* party, double price, int quantity, long ts, string side):
        LimitOrder(id, party, price, quantity, ts, side) {}

    virtual bool operator<(const LimitOrder& other) const override {
        const AuctionOrder* otherAuction = dynamic_cast<const AuctionOrder*>(&other);
        
        if (this->price != otherAuction->price) {
            return this->price < otherAuction->price;
        }
        else if (this->quantity != otherAuction->quantity) {
            return this->quantity > otherAuction->quantity;
        }
        return this->timestamp < otherAuction->timestamp;
    }

    virtual bool operator>(const LimitOrder& other) const override {
        const AuctionOrder* otherAuction = dynamic_cast<const AuctionOrder*>(&other);

        if (this->price != otherAuction->price) {
            return this->price > otherAuction->price;
        }
        else if (this->quantity != otherAuction->quantity) {
            return this->quantity > otherAuction->quantity;
        }
        return this->timestamp < otherAuction->timestamp;
    }
};

#endif