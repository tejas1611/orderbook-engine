#include <iostream>
#include "../include/Party.hpp"

using std::cout;
using std::endl;

Party::Party(std::string name) {
    this->name = name;
    this->net_position = 0;
    this->volume = 0;
    this->PnL = 0.0;
    this->historical_positions.push_back({0, 0});
}

void Party::record_transaction(int quantity, double price, long ts, Side side) {
    if(side == Side::buy) {
        this->net_position += quantity;
        this->PnL -= quantity*price;
    }
    else {
        net_position -= quantity;
        this->PnL += quantity*price;
    }
    
    this->volume += quantity;

    // Another transaction occurred at same timestamp, thus only record latest net position at that timestamp
    if(this->historical_positions.size() > 0 && this->historical_positions.back().first == ts)
        this->historical_positions.pop_back();

    this->historical_positions.push_back({ts, net_position});
}

void Party::print() {
    char X = (this->net_position >= 0) ? 'L' : 'S';
    cout << this->name << " -- " << X << ": " << abs(this->net_position) << endl;
}

void Party::analyze() {
    cout << this->name << " ----------------" << endl;
    char X = (this->net_position >= 0) ? 'L' : 'S';
    cout << "Net Position = " << X << ": " << abs(this->net_position) << endl;
    
    int max_pos = 0, min_pos = 0;
    long sum_pos = 0;
    cout << "Historical Positions: (position @ timestamp)" << endl;
    for(auto& p: this->historical_positions) {
        cout << p.second << " @ " << p.first << " | ";
        max_pos = std::max(max_pos, p.second);
        min_pos = std::min(min_pos, p.second);
        sum_pos += p.second;
    }
    float avg_pos = (float)sum_pos / (this->historical_positions.size()-1);
    
    cout << endl << "PnL = " << this->PnL << " ; Volume = " << this->volume << endl;
    cout << "Max Position = " << max_pos << " ; Min Position = " << min_pos << " ; Avg Position = " << avg_pos << endl;
    
    cout << endl;
}