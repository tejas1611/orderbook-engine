#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <unordered_map>
#include <set>
#include <vector>
#include <memory>
#include "LimitOrder.hpp"
#include "AuctionOrder.hpp"

using std::vector;
using std::unordered_map;
using std::set;
using std::shared_ptr;

struct MinBSTComparator {
    bool operator()(const shared_ptr<LimitOrder> a, const shared_ptr<LimitOrder> b) const {
        return (*a) < (*b);
    }
};

struct MaxBSTComparator {
    bool operator()(const shared_ptr<LimitOrder> a, const shared_ptr<LimitOrder> b) const {
        return (*a) > (*b);
    }
};

class OrderBook {
public:
    OrderBook();
    void insertOrder(shared_ptr<LimitOrder> order);
    void execute(shared_ptr<LimitOrder> order);
    void execute(shared_ptr<AuctionOrder> order);
    double executeAuction();
    void print();

private:
    double calculateMatchingPrice();
    void executeOrdersAtPrice(double price);

    unordered_map<long, shared_ptr<LimitOrder>> allOrders;               // id - Order pairs
    set<shared_ptr<LimitOrder>, MinBSTComparator> sellOrders;            // Order pointers (MinBST)
    set<shared_ptr<LimitOrder>, MaxBSTComparator> buyOrders;             // Order pointers (MaxBST)
};

#endif