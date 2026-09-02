# Metro Smart Card System

Data Structures Semester Project  
Language: C++17

## Project Overview

This project implements a console-based Metro Smart Card System.

The system supports card registration, fast card lookup, top-up operations, undo of the last N top-ups, blocked-card checking, passenger gate queues, tap-in/tap-out, fare calculation, journey history, transaction replay, file loading/saving, interactive menu mode, and command-file mode.

The implementation uses custom data structures instead of standard-library containers such as `std::vector`, `std::list`, `std::queue`, `std::stack`, `std::map`, or `std::unordered_map`.

---

## Data Structures Used

### 1. Card Hash Table

Used for registered cards.

Purpose:
- Fast lookup by 16-digit card number
- Card registration
- Tap-in lookup
- Tap-out lookup
- Duplicate-card detection

Implementation:
- Custom hash table
- Separate chaining using linked nodes
- Automatic resizing when load factor exceeds 0.75

Expected average complexity:
- Insert: O(1)
- Search: O(1)
- Delete: O(1)

Worst-case complexity:
- O(n) if many keys collide

---

### 2. Blocked Card Hash Table

Stores blocked card numbers.

Purpose:
- Check blocked-card membership on every tap
- Block cards
- Unblock cards

Expected average complexity:
- Insert: O(1)
- Search: O(1)
- Delete: O(1)

---

### 3. Gate Queue

Custom linked queue used for passengers waiting at a gate.

Behavior:
- FIFO: First In, First Out
- Enqueue at rear
- Dequeue from front

Complexity:
- Enqueue: O(1)
- Dequeue: O(1)

---

### 4. Top-Up Undo Stack

Custom linked stack used to store operator top-up transactions.

Behavior:
- LIFO: Last In, First Out
- Supports undo of the last N top-up transactions in exact reverse order

Complexity per undo:
- Push: O(1)
- Pop: O(1)

---

### 5. Transaction Log

Custom singly linked list used for the daily transaction log.

Purpose:
- Append transactions in original order
- Replay all transactions at end of day

Complexity:
- Append: O(1), because a tail pointer is stored
- Replay: O(n), one pass through the transactions

---

### 6. Journey History

Custom doubly linked list used for Advanced B.

Purpose:
- Store the last 20 journeys of a card
- Move backward
- Move forward
- Insert a journey after the current journey
- Delete the current journey

Complexity when current node is already known:
- Move previous: O(1)
- Move next: O(1)
- Insert: O(1)
- Delete: O(1)

The history automatically removes the oldest journey when more than 20 journeys are stored.

---

### 7. Station Array

The metro system has 30 fixed stations.

Station information is stored in a fixed array:

```cpp
Station stations[30];

PROJECT STRUCTURE

MetroSmartCardSystem/
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
|-- data/
|   |-- cards.csv
|   |-- journeys.csv
|   |-- commands.txt
|   `-- cards_bad.csv
|
|-- tests/
|   `-- structure_tests.cpp
|   `-- memory_test.cpp
|   `-- performance_test.cpp
|   `-- performance_result.txt
|   `-- system_test.cpp
|   `-- test_log.txt
|
|-- Makefile
|-- README.md
`-- .gitignore

BUILDING THE PROJECT

The project requires C++17.

Linux / systems where GNU Make is named make
</> Bash
make

Windows / MinGW
</> Bash
mingw32-make

The build uses:

-std=c++17
-Wall
-Wextra
-pedantic

The final executable is named:
metro

or on Windows:
metro.exe


RUNNING THE PROGRAM
Menu Mode

Windows:
</> Bash
.\metro

Linux:
</> Bash
./metro

This launches the interactive menu.

COMMAND-FILE MODE

Windows:
</> Bash
.\metro data\commands.txt

Linux:
</> Bash
./metro data/commands.txt

The program reads one command per line. Lines beginning with # are comments.

COMMAND FILE SYNTAX

The command separator is:

|

REGISTER
Format:
REGISTER|cardNumber|holderName|cnic|balance
Example:
REGISTER|1234560000000201|Ali Khan|3520212345671|500

FIND
Format:
FIND|cardNumber
Example:
FIND|1234560000000201

TOPUP
Format:
TOPUP|cardNumber|amount|time
Example:
TOPUP|1234560000000201|200|09:30

UNDO
Format:
UNDO|N
Example:
UNDO|3
This undoes up to the last 3 available top-up transactions in reverse order.
If N is omitted:

UNDO
the program defaults to one undo.

BLOCK
Format:
BLOCK|cardNumber|time
Example:
BLOCK|1234560000000201|10:00

UNBLOCK
Format:
UNBLOCK|cardNumber|time
Example:
UNBLOCK|1234560000000201|10:05

TAPIN
Format:
TAPIN|cardNumber|stationId|time
Example:
TAPIN|1234560000000201|3|10:10

Valid station IDs are currently:
0 to 29

Tap-in checks:
->card exists
->card is active
->card is not blocked
->minimum balance is available
->no journey is already open
->station ID is valid

TAPOUT
Format:
TAPOUT|cardNumber|stationId|time
Example:
TAPOUT|1234560000000201|8|10:40

Tap-out:
->finds the card using hashing
->checks blocked-card membership
->obtains the open journey directly from the card
->calculates fare
->deducts fare
->creates a completed journey
->stores it in journey history
->closes the open journey

ENQUEUE
Format:
ENQUEUE|cardNumber
Example:
ENQUEUE|1234560000000201
Adds the passenger to the gate queue.

SERVE
Format:
SERVE
Removes and serves the passenger at the front of the gate queue.

REPLAY
Format:
REPLAY
Replays all transactions recorded during the current execution in original order.

SAVE
Format:
SAVE
Saves card and journey data.

CARD CSV DORMAT

The file:
data/cards.csv

uses this header:
cardNumber,holderName,cnic,balance,blocked,active,openJourney,entryStation,entryTime

Example:
1234560000000101,Ali Khan,3520212345671,500.00,0,1,0,-1,

Fields:

blocked:
0 = not blocked
1 = blocked

active:
0 = inactive
1 = active

openJourney:
0 = no open journey
1 = open journey

entryStation:
-1 = no entry station

For this project format, text fields should not contain commas.

JOURNEY CSV FORMAT

The file:
data/journeys.csv

uses:
cardNumber,entryStation,exitStation,entryTime,exitTime,fare

Example:
1234560000000101,1,4,08:00,08:20,30.00

FARE RULE

The project specification states that fare depends on the number of stations travelled but does not provide an exact tariff.

For development, the system uses:
fare = absolute difference between station IDs * 10

Example:
Entry station: 3
Exit station: 8

Distance = |8 - 3| = 5

Fare = 5 * 10 = 50

The fare logic is isolated in:
MetroSystem::calculateFare()

so it can easily be changed if another tariff is provided.

MINIUM BALANCE

The development minimum balance required for tap-in is:
50

This value is stored as a constant inside MetroSystem.

PERFORMANCE MEASUREMENT

Operations report:
Time in microseconds
Number of steps
Number of comparisons

Timing uses:
std::chrono

User typing time is not included in the measured operation time.
The hash-table implementation has also been tested with:
5,000 cards
50,000 cards
while keeping the load factor controlled using automatic resizing.

ERROR HANDLING

The program handles cases including:
->missing files
->malformed CSV rows
->duplicate card numbers
->invalid menu input
->missing cards
->blocked cards
->invalid station IDs
->low balance
->duplicate tap-in
->tap-out without an open journey
->empty gate queue
->empty undo stack
->deleting or navigating an empty journey history

MEMORY MANAGEMENT

Pointer-based structures dynamically allocate their own nodes.

Owning classes implement the Rule of Three:
->destructor
->copy constructor
->copy assignment operator
This prevents shallow-copy problems, double deletion, and dangling ownership between copied structures.

ADVANCE REQUIREMENTS
Advanced A
End-of-day transaction replay.

Implemented using a singly linked transaction log with:
->head pointer
->tail pointer
->O(1) append
->one-pass replay in original order

Advanced B
Journey history for one card.

Implemented using a doubly linked list supporting:
->constant-time forward movement
->constant-time backward movement
->constant-time known-position insertion
->constant-time deletion of current journey
->maximum 20 journeys

TESTING

Functional test

Windows:
</> Bash
mingw32-make test

Passed: 14
Failed: 0
ALL FUNCTIONAL TESTS PASSED

performance_test
</> Bash
mingw32-make performance
.\performance_test

Actual measured results are stored in:
tests/performance_results.txt

Rule of Three / Memory Behavior Test
Compile:
</> Bash
g++ -std=c++17 -Wall -Wextra -pedantic tests/memory_test.cpp src/Card.cpp src/Journey.cpp src/JourneyHistory.cpp src/OperationCounter.cpp src/GateQueue.cpp src/TopUpStack.cpp src/TransactionLog.cpp src/CardHashTable.cpp src/BlockedCardHashTable.cpp -Iinclude -o memory_test

</> Bash
.\memory_test

GIT
 
The project is developed using meaningful Git commits throughout implementation.
The repository contains separate .h and .cpp files and avoids committing generated .o files and executable files.

NOTES
This repository uses custom implementations for the required data structures.
Standard-library containers such as std::vector, std::list, std::stack, std::queue, std::map, and std::unordered_map are not used for the core data structures.
