#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

#include "../include/OrderBook.hpp"
#include "../include/LimitOrder.hpp"
#include "../include/AuctionOrder.hpp"
#include "../include/Party.hpp"

using std::string;
using std::cout;
using std::endl;

Party* getParty(string& name, std::unordered_map<string, Party>& parties) {
    if(parties.count(name) > 0) return &(parties.at(name));

    Party newParty{name};
    parties.insert({name, newParty});
    return &(parties.at(name));
}

void processAuctionOrder(std::vector<string>& row, OrderBook& orderBook, std::unordered_map<string, Party>& parties) {
    // cout << "Processing Auction Order " << row[0] << endl;
    Party* orderParty = getParty(row[1], parties);

    std::shared_ptr<AuctionOrder> ord(new AuctionOrder(
        std::stol(row[0]),      // id
        orderParty,             // party
        std::stod(row[2]),      // price
        std::stoi(row[3]),      // quantity
        std::stol(row[4]),      // timestamp
        row[5]                  // side
    ));

    orderBook.execute(std::move(ord));
}

void processLimitOrder(std::vector<string>& row, OrderBook& orderBook, std::unordered_map<string, Party>& parties) {
    // cout << "Processing Limit Order " << row[0] << endl;
    Party* orderParty = getParty(row[1], parties);
    
    std::shared_ptr<LimitOrder> ord(new LimitOrder (
        std::stol(row[0]),      // id
        orderParty,             // party
        std::stod(row[2]),      // price
        std::stoi(row[3]),      // quantity
        std::stol(row[4]),      // timestamp
        row[5]                  // side
    ));

    orderBook.execute(std::move(ord));
}

int main(int argc, char **argv) 
{
    if(argc != 3) {
        cout << "Not enough arguments" << endl;
        cout << "Run program using: ./matching_engine <filename> <option>" << endl;
        cout << "\t<option>: 1 - normal priority; 2 - auction; 3 - normal priority with analysis" << endl;
        return -1;
    }
    if(strcmp(argv[2], "1") != 0 && strcmp(argv[2], "2") != 0 && strcmp(argv[2], "3") != 0) {
        cout << "Wrong format of <option>" << endl;
        cout << "\t<option>: 1 - normal priority; 2 - auction; 3 - normal priority with analysis" << endl;
        return -1;
    }

    OrderBook orderBook;
    std::unordered_map<string, Party> parties;

    std::fstream file; 
    file.open(argv[1], std::ios::in); 

    if(file.is_open()) {
        std::vector<string> row; 
        string line, word, temp;
        while(std::getline(file, line)) {
            row.clear();
            std::stringstream str(line);
            while(std::getline(str, word, ','))
                row.push_back(word);
            
            if(strcmp(argv[2], "1") == 0 || strcmp(argv[2], "3") == 0) 
                processLimitOrder(row, orderBook, parties);
            if(strcmp(argv[2], "2") == 0) 
                processAuctionOrder(row, orderBook, parties);
            // orderBook.print();
        }
    }
    else {
        cout<<"Could not open the file\n";
    }

    file.close();

    // orderBook.print();

    if(strcmp(argv[2], "2") == 0) {
        double matching_price = orderBook.executeAuction();
        cout << "Matching Price = " << matching_price << endl;
    }
    
    for(auto it = parties.begin(); it != parties.end(); it++) {
        if(strcmp(argv[2], "1") == 0 || strcmp(argv[2], "2") == 0) (it->second).print();
        if(strcmp(argv[2], "3") == 0) (it->second).analyze();
    }

    return 0;
} 