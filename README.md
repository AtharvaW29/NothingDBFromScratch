# NothingDB

NothingDB is a relational database management system being built from scratch in modern C++.

The project focuses on understanding core database internals including:

- Page-based storage
- Disk management
- Buffer pool management
- B+ Tree indexing
- Query execution
- SQL parsing
- Transactions and WAL
- Concurrency control

## Current Progress

### Implemented
- Fixed-size page abstraction
- Persistent disk storage
- Page allocation system
- Binary page read/write operations

## Tech Stack

- C++20
- CMake
- Visual Studio 2026
- MSVC v143 Toolset

## Project Structure

src/
├── storage/
├── buffer/
├── index/
├── parser/
├── executor/
├── transaction/
└── recovery/

include/
tests/
data/
logs/

## Build Commands
mkdir build
cd build
cmake ..
cmake --build .

## Run
./NothingDB