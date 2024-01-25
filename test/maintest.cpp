#define BOOST_TEST_MODULE OrderBookTest
#include <boost/test/included/unit_test.hpp>

#include "../include/OrderBook.hpp"
#include "../include/Party.hpp"

BOOST_AUTO_TEST_CASE(PrintsZeroWhenNoOrdersMatched) {
    OrderBook ob;

    BOOST_CHECK_EQUAL(ob.executeAuction(), 0);

    Party testParty = Party("testParty");

    shared_ptr<AuctionOrder> ord1(new AuctionOrder(1, &testParty, 75, 100, 10001, "BUY"));
    ob.execute(ord1);
    shared_ptr<AuctionOrder> ord2(new AuctionOrder(2, &testParty, 85, 100, 10008, "BUY"));
    ob.execute(ord2);
    shared_ptr<AuctionOrder> ord3(new AuctionOrder(3, &testParty, 95, 100, 10017, "BUY"));
    ob.execute(ord3);

    BOOST_CHECK_EQUAL(ob.executeAuction(), 0);
}

BOOST_AUTO_TEST_CASE(PrintsMatchingPriceWhenOrdersMatched) {
    OrderBook ob;

    Party testParty("testParty");

    shared_ptr<AuctionOrder> ord1(new AuctionOrder(1, &testParty, 99, 100, 10001, "BUY"));
    ob.insertOrder(ord1);
    shared_ptr<AuctionOrder> ord2(new AuctionOrder(2, &testParty, 101, 200, 10005, "SELL"));
    ob.insertOrder(ord2);
    shared_ptr<AuctionOrder> ord3(new AuctionOrder(3, &testParty, 102, 100, 10019, "BUY"));
    ob.insertOrder(ord3);
    shared_ptr<AuctionOrder> ord4(new AuctionOrder(4, &testParty, 100, 50, 10032, "SELL"));
    ob.insertOrder(ord4);

    BOOST_CHECK_EQUAL(ob.executeAuction(), 102);
}

BOOST_AUTO_TEST_CASE(LimitOrderExecutesCorrectly) {
    OrderBook ob;

    Party simon = Party("Simon");
    Party mal = Party("Mal");
    Party shep = Party("Shephard");
    Party inara = Party("Inara");

    shared_ptr<LimitOrder> ord1(new LimitOrder(1, &simon, 105, 200, 2, "SELL"));
    ob.execute(ord1);
    shared_ptr<LimitOrder> ord2(new LimitOrder(2, &mal, 102, 100, 4, "SELL"));
    ob.execute(ord2);
    shared_ptr<LimitOrder> ord4(new LimitOrder(3, &inara, 101, 100, 7, "SELL"));
    ob.execute(ord4);
    shared_ptr<LimitOrder> ord3(new LimitOrder(3, &shep, 98, 100, 6, "BUY"));
    ob.execute(ord3);
    shared_ptr<LimitOrder> ord5(new LimitOrder(3, &mal, 102, 100, 10, "BUY"));
    ob.execute(ord5);

    BOOST_CHECK_EQUAL(mal.getNetPosition(), 100);
    BOOST_CHECK_EQUAL(inara.getNetPosition(), -100);
    BOOST_CHECK_EQUAL(shep.getNetPosition(), 0);
    BOOST_CHECK_EQUAL(simon.getNetPosition(), 0);
}