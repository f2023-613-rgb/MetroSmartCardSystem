# Metro Smart Card System

**Data Structures Semester Project**  
**Language:** C++17

---

## Project Overview

The Metro Smart Card System is a console-based C++17 application that simulates the main operations of a metro smart-card network with 30 stations arranged in a straight line.

The system supports:

- Card registration
- Fast lookup by 16-digit card number
- Card top-up
- Undo of the last N top-up transactions
- Block and unblock operations
- Blocked-card checking on every tap
- Tap-in and tap-out
- Fare calculation
- Passenger gate queues
- Journey history
- End-of-day transaction replay
- CSV loading and saving
- Interactive menu mode
- Command-file mode
- Operation timing in microseconds
- Step and comparison counting
- Performance testing
- Rule-of-Three testing

The implementation uses custom data structures instead of standard-library containers such as:

```text
std::vector
std::list
std::queue
std::stack
std::priority_queue
std::map
std::set
std::unordered_map
std::unordered_set

DATA STRUCTURE USED
1. Card Hash Table

The main card database is implemented as a custom hash table.

Purpose
->Register cards
->Search for a card by its unique 16-digit number
->Detect duplicate card numbers
->Find cards during top-up
->Find cards during tap-in
->Find cards during tap-out
->Find cards during blocking and unblocking

Implementation
->Dynamic array of bucket pointers
->Separate chaining using custom linked nodes
->Automatic resizing / rehashing
->Rehashing occurs when the projected load factor exceeds 0.75


COMPLEXITY

| Operation | Expected Average | Worst Case |
| --------- | ---------------: | ---------: |
| Insert    |             O(1) |       O(n) |
| Search    |             O(1) |       O(n) |
| Delete    |             O(1) |       O(n) |
The worst case occurs if many keys collide into the same chain.

2. Blocked Card Hash Table
A separate custom hash table stores only the card numbers that are currently blocked.

Purpose
->Check blocked-card membership on every tap
->Check blocked-card membership on every tap
->Block a card
->Unblock a card
->Maintain the current number of blocked cards

COMPLEXITY

| Operation            | Expected Average |
| -------------------- | ---------------: |
| Block / Insert       |             O(1) |
| Check blocked status |             O(1) |
| Unblock / Delete     |             O(1) |
The normal Card object also stores a blocked flag so card state can be displayed and saved. The separate blocked-card hash table is used for fast membership checking.

3. Gate Queue

Passengers waiting at a gate are stored in a custom linked queue.

Behavior

FIFO:

First In, First Out

A passenger is:

Added at the rear
Removed from the front

**COMPLEXITY**
| Operation | Complexity |
| --------- | ---------: |
| Enqueue   |       O(1) |
| Dequeue   |       O(1) |
The queue stores both front and rear pointers.

4. Top-Up Undo Stack

Operator top-up transactions are stored in a custom linked stack.
Behavior

LIFO:
Last In, First Out

This allows the last N top-up transactions to be undone in exact reverse order.

For example:
Top-up 100
Top-up 50
Top-up 25

Executing:
UNDO|3

undoes:
25
50
100

**COMPLEXITY**
Each individual undo uses:

->Stack pop: O(1)
->Expected card hash lookup: O(1)
->Balance update: O(1)
->Transaction-log append: O(1)

Therefore, one undo is expected O(1).
Undoing N transactions naturally takes O(N) total because N transactions must be processed.

5. Transaction Log

The daily transaction history is implemented as a custom singly linked list.

It stores operations such as:
REGISTER
TOP_UP
UNDO_TOP_UP
BLOCK_CARD
UNBLOCK_CARD
TAP_IN
TAP_OUT

Design
The list keeps:
head
tail

The tail pointer allows a new transaction to be appended without traversing the entire list.

**COMPLEXITY**

| Operation               | Complexity |
| ----------------------- | ---------: |
| Append transaction      |       O(1) |
| Replay all transactions |       O(n) |
Replay makes exactly one pass through the transaction list and preserves original transaction order.

6. Journey History

Each card contains a custom doubly linked list containing its recent completed journeys.
This implements Advanced B.

Features
->Stores at most the last 20 journeys
->Move backward one journey
->Move forward one journey
->Insert a missing journey after the current journey
->Delete the current journey
->No array shifting is required

Each node contains:
previous pointer
journey data
next pointer

**COMPLEXITY** when the current node is already known
| Operation            | Complexity |
| -------------------- | ---------: |
| Move previous        |       O(1) |
| Move next            |       O(1) |
| Insert after current |       O(1) |
| Delete current       |       O(1) |
When more than 20 journeys are added, the oldest journey is automatically removed.

7. Station Array

The metro contains exactly 30 stations, so a fixed array is used:
Station stations[30];

The stations are initialized as:
Internal ID 0  -> ST01 -> Station 1
Internal ID 1  -> ST02 -> Station 2
...
Internal ID 29 -> ST30 -> Station 30

Command and menu station IDs use:
0 to 29

Direct station access by internal ID is:
O(1)

A fixed array is appropriate because the number of stations never changes.


**CORE OPERATION**
**1. Register Card**

A card contains:
->16-digit card number
->Holder name
->CNIC
->Balance
->Active state
->Blocked state
The program validates that the card number is exactly 16 digits.

Valid range:
$$
1000000000000000
to
9999999999999999
$$

Duplicate card numbers are rejected.

A card may be registered initially as:
ACTIVE
or:
BLOCKED

If a new card is registered as blocked, its number is also immediately inserted into the blocked-card hash table.

**2. Tap In**

Tap-in verifies:

1 Station ID is valid
2 Card exists
3 Card is active
4 Card is not blocked
5 Card has the minimum required balance
6 Card does not already have an open journey

If valid, the card directly stores:

openJourney = true
entryStation
entryTime

Because the open journey is stored directly in the card, it does not need to be searched for later.

**3. Tap Out
**
Tap-out:

1 Validates the exit station
2 Finds the card through hashing
3 Checks the blocked-card hash table
4 Reads the open journey directly from the card
5 Calculates the fare
6 Deducts the fare
7 Creates a completed Journey
8 Adds it to journey history
9 Closes the open journey
10 Records the transaction

Finding the open tap-in after finding the card is O(1).

**4. Blocked Card Checking**

Blocked cards are stored in a separate custom hash table.

Both:

TAPIN
TAPOUT

check this structure.

The blocked list can change during the day through:

BLOCK
UNBLOCK

**5. Gate Queue
**

Passengers are processed strictly in the order they enter the queue.

enqueue -> rear
dequeue -> front

Both operations are O(1).

**6. Undo Last N Top-Ups**

Top-ups are placed onto a custom stack.

The system supports undoing:

1
2
3
...
N

recent top-ups in exact reverse order.

The implementation first checks the top stack item before removing it, so a failed undo does not unnecessarily destroy the undo record.

**ADVANCE REQUIERMENTS
**
Advanced A — End-of-Day Transaction Replay

Implemented using the custom singly linked transaction log.

Properties:

->O(1) append
->Original transaction order preserved
->One-pass replay
->Replay time measured in microseconds
->Replay traversal steps reported

Advanced B — Journey History

Implemented using the custom doubly linked JourneyHistory.

Supported operations:

->Move previous
->Move next
->Insert after current
->Delete current
->Store maximum 20 journeys

Forward and backward movement are both O(1).

**PROJECT STRUCTURE**
MetroSmartCardSystem/
|
|-- data/
|   |-- cards.csv
|   |-- journeys.csv
|   |-- commands.txt
|   `-- cards_bad.csv
|
|-- include/
|   |-- BlockedCardHashTable.h
|   |-- Card.h
|   |-- CardHashTable.h
|   |-- CommandProcessor.h
|   |-- GateQueue.h
|   |-- Journey.h
|   |-- JourneyHistory.h
|   |-- MetroSystem.h
|   |-- OperationCounter.h
|   |-- Station.h
|   |-- TopUpStack.h
|   `-- TransactionLog.h
|
|-- src/
|   |-- BlockedCardHashTable.cpp
|   |-- Card.cpp
|   |-- CardHashTable.cpp
|   |-- CommandProcessor.cpp
|   |-- GateQueue.cpp
|   |-- Journey.cpp
|   |-- JourneyHistory.cpp
|   |-- main.cpp
|   |-- MetroSystem.cpp
|   |-- OperationCounter.cpp
|   |-- Station.cpp
|   |-- TopUpStack.cpp
|   `-- TransactionLog.cpp
|
|-- tests/
|   |-- memory_test.cpp
|   |-- performance_test.cpp
|   |-- performance_results.txt
|   |-- structure_tests.cpp
|   |-- system_test.cpp
|   `-- test_log.txt
|
|-- .gitignore
|-- Makefile
`-- README.md

Building the Project

The project requires:

C++17

Compiler flags:

-std=c++17
-Wall
-Wextra
-pedantic

Linux
</> BASH
make

Windows / MinGW
</> BASH
mingw32-make

The resulting executable is:

Linux:

metro

Windows:

metro.exe

**RUNNING THE PROGRAM**
The program supports two required execution modes.

**1. Interactive Menu Mode**
Windows
</> BASH
.\metro

Linux
</> BASH
./metro

The final menu contains:
1. Register card
2. Find/show card
3. Top up card
4. Undo last N top-ups
5. Block card
6. Unblock card
7. Tap in
8. Tap out
9. Add passenger to gate queue
10. Serve next passenger
11. Show current journey history record
12. Move journey previous
13. Move journey next
14. Delete current journey
15. Insert journey after current
16. Replay today's transactions
17. Save data
0. Exit
When menu option 0 is selected, card and journey data are saved before the program exits.

**2. Command-File Mode **
The required script syntax is supported.

Windows
</> BASH
.\metro --script data\commands.txt

Linux
</> BASH
./metro --script data/commands.txt

The program reads one command per line.

Lines beginning with:

#

are treated as comments.

Invalid command-line usage displays:
Usage:
  metro
  metro --script <command-file>

**Command File Syntax**
Commands use:

|

as the field separator.

**REGISTER**

Format:

REGISTER|cardNumber|holderName|cnic|balance|status

Valid statuses:

ACTIVE
BLOCKED

Example:

REGISTER|1234560000000201|Ali Khan|3520212345671|500|ACTIVE

Blocked registration example:

REGISTER|1234560000000202|Ahmed Khan|3520212345672|500|BLOCKED

**FIND**

Format:

FIND|cardNumber

Example:

FIND|1234560000000201

**TOPUP**

Format:

TOPUP|cardNumber|amount|time

Example:

TOPUP|1234560000000201|200|09:30

**UNDO**

Format:

UNDO|N

Example:

UNDO|3

This attempts to undo the last three available top-up transactions in exact reverse order.

If N is omitted:

UNDO

the program defaults to one undo.

If fewer than N transactions exist, only the available transactions are undone.

Example:

UNDO 2/5 OK

means two transactions were available when five were requested.

**BLOCK**

Format:

BLOCK|cardNumber|time

Example:

BLOCK|1234560000000201|10:00

**UNBLOCK**

Format:

UNBLOCK|cardNumber|time

Example:

UNBLOCK|1234560000000201|10:05

**TAPIN**

Format:

TAPIN|cardNumber|stationId|time

Example:

TAPIN|1234560000000201|3|10:10

Valid station IDs:

0 to 29

Tap-in checks:

->Card exists
->Card is active
->Card is not blocked
->Minimum balance is available
->No journey is already open
->Station ID is valid

**TAPOUT**

Format:

TAPOUT|cardNumber|stationId|time

Example:

TAPOUT|1234560000000201|8|10:40

Tap-out:

->Finds the card using hashing
->Checks blocked-card membership
->Reads the open tap-in directly from the card
->Calculates the fare
->Deducts the fare
->Creates a completed journey
->Adds the journey to history
->Closes the open journey

**ENQUEUE**

Format:

ENQUEUE|cardNumber

Example:

ENQUEUE|1234560000000201

Adds a passenger to the rear of the gate queue.

**SERVE**

Format:

SERVE

Removes and serves the passenger at the front of the gate queue.

**REPLAY**

Format:

REPLAY

Replays all transactions recorded during the current execution in their original order.

Replay also reports:

->Time
->Steps
->Comparisons

**SAVE**

Format:

SAVE

Saves card and journey information back to the CSV files.

**CARD CSV FORMAT**
File:

data/cards.csv

Header:

cardNumber,holderName,cnic,balance,blocked,active,openJourney,entryStation,entryTime

Example:

1234560000000101,Ali Khan,3520212345671,500.00,0,1,0,-1,
Boolean Fields

Blocked:

0 = not blocked
1 = blocked

Active:

0 = inactive
1 = active

Open journey:

0 = no open journey
1 = open journey

If there is no open journey:

entryStation = -1

For the project CSV format, text fields must not contain commas.

Malformed rows are skipped safely.

**JOURNEY CSV FORMAT
**
File:

data/journeys.csv

Header:

cardNumber,entryStation,exitStation,entryTime,exitTime,fare

Example:

1234560000000101,1,4,08:00,08:20,30.00

The card number identifies the owner of the journey.

When journeys are loaded, the JourneyHistory structure automatically enforces its 20-journey maximum.

**FARE RULE**
The project specification states that fare depends on the number of stations travelled but does not provide a specific tariff.

For development, the system uses:

fare = |exitStation - entryStation| * 10

Example:

Entry station = 3
Exit station  = 8

Distance = |8 - 3|
         = 5

Fare = 5 * 10
     = 50

The fare logic is isolated inside:

```c++
MetroSystem::calculateFare()

```
so the tariff can easily be changed later without redesigning the data structures.

**MINIUM BALANCE**
The development minimum balance required for tap-in is:

50

It is stored as a constant inside MetroSystem.

PERFORMANCE MEASUREMENT

Major operations report:

Time in microseconds
Number of steps
Number of comparisons

Timing is performed using:

```c++
std::chrono

```
Only the actual operation is timed. User typing time is not included.

**PERFORMANCE TEST**
The main card hash table was tested at:

5,000 cards
20,000 cards
50,000 cards

Each test repeatedly searched for an existing card 10,000 times.

Actual test results showed that the measured lookup comparison count remained approximately constant as the number of stored cards increased.

The recorded development results include:

5,000 cards:
Average comparisons = 1.0000
Average steps       = 2.0000

20,000 cards:
Average comparisons = 1.0000
Average steps       = 2.0000

50,000 cards:
Average comparisons = 1.0000
Average steps       = 2.0000

Full measured output is stored in:

tests/performance_results.txt
Run Performance Test

Windows
</> BASH
mingw32-make performance
.\performance_test

Linux
</> BASH
make performance
./performance_test

**FUNCTIONAL TESTING**
A complete functional test is provided in:
tests/system_test.cpp

Run on Windows:
</>BASH
mingw32-make test

Expected result:

Passed: 14
Failed: 0

ALL FUNCTIONAL TESTS PASSED

The test covers:

->Valid card registration
->VDuplicate registration
->Invalid card-number rejection
->Card lookup
->Top-up
->Undo
->Tap-in
->Duplicate tap-in rejection
->Blocked tap-out rejection
->Unblock and successful tap-out
->Correct fare
->Journey-history insertion after tap-out
->FIFO gate queue
->Empty undo stack
->Empty gate queue

A saved development test log is stored in:

tests/test_log.txt

**RULE OF THREE** / Memory Behavior Test

Pointer-owning structures implement the Rule of Three:

->Destructor
->Copy constructor
->Copy assignment operator

A dedicated test is provided in:

tests/memory_test.cpp

It creates copies and assignments of:

->Gate queue
->Top-up stack
->Transaction log
->Journey history
->Card hash table
->Blocked-card hash table

Compile on Windows:
</> BASH
g++ -std=c++17 -Wall -Wextra -pedantic tests/memory_test.cpp src/Card.cpp src/Journey.cpp src/JourneyHistory.cpp src/OperationCounter.cpp src/GateQueue.cpp src/TopUpStack.cpp src/TransactionLog.cpp src/CardHashTable.cpp src/BlockedCardHashTable.cpp -Iinclude -o memory_test

Run:
</> BASH
.\memory_test

The test verifies that copied objects can leave scope without crashes or double deletion.

AddressSanitizer Note
AddressSanitizer compilation was attempted during development.
The installed Windows MinGW environment did not contain the required libasan runtime and produced:

cannot find -lasan

Therefore, the project does not claim that AddressSanitizer successfully ran on the development Windows environment.
The Rule-of-Three test is included as the available local memory-behavior verification.

Error Handling

The program handles cases including:

->Missing input files
->Malformed CSV rows
->Duplicate card numbers
->Invalid non-16-digit card numbers
->Invalid registration status
->Invalid menu input
->Missing cards
->Blocked cards
->Invalid station IDs
->Low balance
->Duplicate tap-in
->Tap-out without an open journey
->Empty gate queue
->Empty undo stack
->Invalid undo count
->Deleting or navigating empty journey history
->Missing command file
->Unknown command
->Invalid command data
->Invalid command-line arguments

Wrong input is handled without intentionally leaking memory or crashing normal program execution.

**MEMORY MANAGEMENT
**

Custom linked structures dynamically allocate their own nodes.

The owning classes use:

Destructor
Copy constructor
Copy assignment operator

to perform deep copying and correct cleanup.

This avoids shallow copies that could otherwise cause:

->Double deletion
->Shared ownership of raw pointers
->Dangling pointers
->Invalid memory access

**File Loading and Saving**
At startup:

cards.csv
journeys.csv

are loaded.

Existing data is inserted directly into the structures rather than being treated as new daily business transactions.

For example, loading an old card does not generate a new REGISTER transaction for today.

At save or normal program exit:

cards.csv
journeys.csv

are updated.

**Development Data**
Because sample cards.csv, journeys.csv, and command files were not available during development, development versions were created based on the project specification.

These files provide repeatable examples for testing:

data/cards.csv
data/journeys.csv
data/commands.txt
data/cards_bad.csv

If an official instructor file format is later provided, the file parsing layer can be adjusted without redesigning the custom data structures.

**Git**

The project was developed using Git throughout implementation.

The repository contains more than the required 15 meaningful commits across different development stages.

Generated files such as:

*.o
*.exe

are excluded through .gitignore.

BUILD AND TEST SUMMARY
Typical Windows development workflow:
</>BASH
mingw32-make

Run menu mode:
</>BASH
.\metro

Run command-file mode:
</>BASH
.\metro --script data\commands.txt

Run functional tests:
</>BASH
mingw32-make test

Run performance tests:
</>BASH
mingw32-make performance
.\performance_test

Run Rule-of-Three test:
</>BASH
.\memory_test

**NOTES**
This project intentionally uses custom data-structure implementations for the semester requirements.
The design focuses on matching each problem speed rule to an appropriate structure rather than simply making the program function correctly.

Key mappings include:
Fast card lookup          -> Hash table
Blocked-card membership   -> Hash table
Passenger order           -> Queue
Top-up undo               -> Stack
Daily transaction replay  -> Singly linked list
Journey navigation/edit   -> Doubly linked list
Fixed 30 stations         -> Array
Open tap-in               -> Direct fields inside Card

This allows the required operations to satisfy their intended complexity constraints.


A few important improvements over your current README are now reflected correctly: your current file still documents the old script invocation `.\metro data\commands.txt`, the old five-field `REGISTER` command, and only mentions 5,000/50,000 cards in the performance section. :contentReference[oaicite:1]{index=1} :contentReference[oaicite:2]{index=2}