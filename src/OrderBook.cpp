#include <algorithm>
#include <iostream>
#include "../include/OrderBook.hpp"

using std::cout;
using std::endl;


OrderBook::OrderBook() {}

void OrderBook::print() {
    cout << "BUY BOOK: " << endl;
    for(auto& e: buyOrders) 
        cout << e->getId() << ":" << e->getQuantity() << "@" << e->getPrice() << "-" << e->getTimestamp() << "  |  ";
    cout << "\nSELL BOOK: " << endl;
    for(auto& e: sellOrders) 
        cout << e->getId() << ":" << e->getQuantity() << "@" << e->getPrice() << "-" << e->getTimestamp() << "  |  ";
    cout << "\n\n";
}

void OrderBook::insertOrder(shared_ptr<LimitOrder> order) {
    if(order->getQuantity() <= 0) return;

    long order_id = order->getId();
    this->allOrders.insert({order_id, order});

    switch (order->getSide()) {
    case Side::buy:
        this->buyOrders.insert(order);
    break;

    case Side::sell:
        this->sellOrders.insert(order);
    break;
    }
}

void OrderBook::execute(shared_ptr<LimitOrder> order) {
    // Match the new order against existing orders, then insert in orderbook 
    while (order->getQuantity() > 0) {
        shared_ptr<LimitOrder> matched_order = nullptr;
        if(order->getSide() == Side::buy) {
            auto top_ask = this->sellOrders.begin();
            if (top_ask == this->sellOrders.end()) break;
            matched_order = *top_ask;
            if(order->getPrice() < matched_order->getPrice()) break;
            sellOrders.erase(top_ask);
        }
        if(order->getSide() == Side::sell) {
            auto top_bid = this->buyOrders.begin();
            if (top_bid == this->buyOrders.end()) break;
            matched_order = *top_bid;
            if(order->getPrice() > matched_order->getPrice()) break;
            buyOrders.erase(top_bid);
        }

        float matched_price = matched_order->getPrice();
        int matched_qty = std::min(order->getQuantity(), matched_order->getQuantity());
        long matched_timestamp = order->getTimestamp();

        order->make_transaction(matched_price, matched_qty, matched_timestamp);
        matched_order->make_transaction(matched_price, matched_qty, matched_timestamp);
        this->insertOrder(matched_order);

        // std::cout << "Made transaction: " << ((float)matched_price * matched_qty) << std::endl;
    }

    this->insertOrder(order);
}

void OrderBook::execute(shared_ptr<AuctionOrder> order) {
    this->insertOrder(order);
}

double OrderBook::executeAuction() {
    double matching_price = this->calculateMatchingPrice();
    this->executeOrdersAtPrice(matching_price);

    return matching_price;
}

void OrderBook::executeOrdersAtPrice(double price) {
    auto curr_buy = this->buyOrders.begin();
    auto curr_sell = this->sellOrders.begin();
    
    while (curr_buy != this->buyOrders.end() && curr_sell != this->sellOrders.end() && 
        (*curr_buy)->getPrice() >= price && (*curr_sell)->getPrice() <= price) {
            int buy_qty = (*curr_buy)->getQuantity();
            int sell_qty = (*curr_sell)->getQuantity();
            
            int matched_qty = std::min(buy_qty, sell_qty);
            (*curr_buy)->make_transaction(price, matched_qty, 0);
            (*curr_sell)->make_transaction(price, matched_qty, 0);

            if (buy_qty == matched_qty) curr_buy++;                
            if (sell_qty == matched_qty) curr_sell++;
    }
}

double OrderBook::calculateMatchingPrice() {
    vector<std::pair<double, long>> cumulative_buys;         // Cumulative quantities at each price level of buy orders
    long cumulative_buy_qty = 0;
    
    for(auto& order: buyOrders) {
        cumulative_buy_qty += order->getQuantity();
        double curr_price_level = order->getPrice();
        if (cumulative_buys.size() > 0 && cumulative_buys.back().first == curr_price_level)
            cumulative_buys.pop_back();
        cumulative_buys.push_back( {curr_price_level, cumulative_buy_qty} );
    }

    // Two pointers to calculate best matching price
    auto curr_buy = cumulative_buys.rbegin();
    auto curr_sell = this->sellOrders.begin();
    long cumulative_sell_qty = 0;
    double matching_price = 0, max_transaction_amt = 0; 

    while (curr_buy != cumulative_buys.rend()) {
        double transaction_price =  0.0;
        while (curr_sell != this->sellOrders.end() && curr_buy->first >= (*curr_sell)->getPrice()) {
            cumulative_sell_qty += (*curr_sell)->getQuantity();
            transaction_price = (*curr_sell)->getPrice();
            curr_sell++;
        }

        transaction_price = std::max(transaction_price, curr_buy->first);           // We found sell order that matches curr_buy,
                                                                                    // and select the max price
        long transaction_qty = std::min(cumulative_sell_qty, curr_buy->second);
        double transaction_amt = transaction_qty * transaction_price;
        
        if(transaction_amt >= max_transaction_amt) {            // Found a higher transaction amount or equal transaction amount with higher price
            matching_price = transaction_price;
            max_transaction_amt = transaction_amt;
        }
        
        curr_buy++;
    }

    return max_transaction_amt == 0 ? 0 : matching_price;       // If no orders matched, return 0
}