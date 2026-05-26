# NothingDB

NothingDB is a relational database management system being built from scratch in modern C++.

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
- Tuple Lifecycle Management
- Logical Deletion
- RID-Based Updates
- Stable Tuple Addressing
- Visibility Filtering

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