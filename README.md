**Presto Labs Assessment for Software Engineering**

## Program Execution
### Compilation:  
(Execute from root directory)  
On Linux, use the Makefile by: `make`  
Or `g++ -std=c++17 include/* src/* -o matching_engine`

### Run program using: 
`./matching_engine <filename> <option>`  
*filename*: path of csv file relative to `matching_engine` generated in Compile step  
*option*: 1 - normal priority; 2 - auction; 3 - normal priority with analysis

**Examples:**  
`./matching_engine orders_first1000.csv 1`  
`./matching_engine pth/to/orders.csv 2`

### Tests:
I have included 3 basic tests for Problem 1 and Problem 2. To run tests:  
```bash
cd test 
make
./test
```

## Assumptions:
General:
- Party names are case-sensitive
- As mentioned in examples, a buy order and a sell order from the same party may match with each other (code can be modified to prevent this)
- Fields in csv file are correctly formatted (I have not performed error checking for input fields)

Problem 1:
- Output order not specified, so I output net position of each party in random order

Problem 2:
- Assuming all orders are in the same auction period (code can be modified to accomodate multiple auction periods)
- If no orders can match at end of auction, I output matching price as 0

Problem 3:
- Negative position means short; Positive position means long
- All people start with a historical position of 0 at timestamp 0
- Starting position of 0 also count towards determining the max. and min. positions, but not for average position
- Output format not specified, so I have output all the required analysis information for each party


### Technical Considerations:
- Main (`matching_engine`) owns the Orderbook and all the Party objects, and deletes them upon termination
- Orderbook owns all the Order objects, and deletes them when order fulfilled or out of scope
- Orderbook manages execution of orders
