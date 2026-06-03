#include "storage/table_page.h"

#include <cstring>
#include <vector>

namespace NothingDB{
TablePage::TablePage() : Page() {}

void TablePage::Init()
{
*GetFreeSpacePointer() = PAGE_SIZE;
*GetTupleCountPointer() = 0;
SetNextPageId(INVALID_PAGE_ID);
}

uint16_t* TablePage::GetFreeSpacePointer() 
{
return reinterpret_cast<uint16_t*>(GetData());
}

uint16_t* TablePage::GetTupleCountPointer()
{
return reinterpret_cast<uint16_t*>(GetData() + sizeof(uint16_t));
}

TupleInfo* TablePage::GetTupleInfo(uint16_t slot_num)
{
size_t offset = sizeof(uint16_t) * 2 + sizeof(int) + slot_num * sizeof(TupleInfo);
return reinterpret_cast<TupleInfo*>(GetData() + offset);
}

const TupleInfo* TablePage::GetTupleInfo(uint16_t slot_num) const
{
size_t offset = sizeof(uint16_t) * 2 + sizeof(int) + slot_num * sizeof(TupleInfo);
return reinterpret_cast<const TupleInfo*>(GetData() + offset);
}

uint16_t TablePage::GetTupleCount() const
{
return *reinterpret_cast<const uint16_t*>(GetData() + sizeof(uint16_t));
}

uint16_t TablePage::GetSlotDirectoryEnd() const
{
return sizeof(uint16_t) * 2 + sizeof(int) + GetTupleCount() * sizeof(TupleInfo);
}

uint16_t TablePage::CalculateTotalFreeSpace() const
{
uint16_t free_space_pointer = *reinterpret_cast<const uint16_t*>(GetData());
uint16_t slot_directory_end = GetSlotDirectoryEnd();
uint16_t free_space = free_space_pointer - slot_directory_end;

uint16_t tuple_count = GetTupleCount();
for (uint16_t i = 0; i < tuple_count; ++i) {
const TupleInfo* tuple_info = GetTupleInfo(i);
if (tuple_info->is_deleted) {
free_space += tuple_info->size;
}
}

return free_space;
}

void TablePage::CompactPage()
{
uint16_t tuple_count = GetTupleCount();
char* page_data = GetData();

std::vector<std::pair<uint16_t, TupleInfo>> active_tuples;
for (uint16_t i = 0; i < tuple_count; ++i) {
TupleInfo* tuple_info = GetTupleInfo(i);
if (!tuple_info->is_deleted) {
active_tuples.push_back({i, *tuple_info});
}
}

uint16_t new_offset = PAGE_SIZE;
std::vector<std::pair<uint16_t, uint16_t>> offset_mapping;

for (auto& [slot_num, tuple_info] : active_tuples) {
new_offset -= tuple_info.size;
offset_mapping.push_back({slot_num, new_offset});
}

// Create a temporary buffer to hold all data during compaction
char temp_buffer[PAGE_SIZE];
std::memcpy(temp_buffer, page_data, PAGE_SIZE);

for (size_t i = 0; i < active_tuples.size(); ++i) {
auto [slot_num, old_tuple_info] = active_tuples[i];
uint16_t new_pos = offset_mapping[i].second;

std::memcpy(
page_data + new_pos,
temp_buffer + old_tuple_info.offset,
old_tuple_info.size
);
}

for (size_t i = 0; i < active_tuples.size(); ++i) {
auto [slot_num, old_tuple_info] = active_tuples[i];
uint16_t new_pos = offset_mapping[i].second;

TupleInfo* tuple_info = GetTupleInfo(slot_num);
tuple_info->offset = new_pos;
}


*GetFreeSpacePointer() = new_offset;
}

bool TablePage::InsertTuple(const Tuple& tuple, RID& rid)
{
const auto& tuple_data = tuple.GetData();

uint16_t size = static_cast<uint16_t>(tuple_data.size());
uint16_t free_space_pointer = *GetFreeSpacePointer();
uint16_t tuple_count = *GetTupleCountPointer();
uint16_t slot_directory_end = sizeof(uint16_t) * 2 + sizeof(int) + (tuple_count + 1) * sizeof(TupleInfo);

if (free_space_pointer < slot_directory_end + size) {
return false; // Not enough space
}

free_space_pointer -= size;
std::memcpy(GetData() + free_space_pointer, tuple_data.data(), size);

TupleInfo* tuple_info = GetTupleInfo(tuple_count);

tuple_info->offset = free_space_pointer;
tuple_info->size = size;
tuple_info->is_deleted = false;
*GetFreeSpacePointer() = free_space_pointer;
(*GetTupleCountPointer())++;
rid = RID(GetPageId(), tuple_count);

return true;
}

bool TablePage::GetTuple(uint16_t slot_num, Tuple& tuple)
{
if (slot_num >= GetTupleCount()) {
return false; // Invalid slot number
}

const TupleInfo* tuple_info = GetTupleInfo(slot_num);

if (tuple_info->is_deleted) {
return false;
}

std::vector<char> tuple_data(tuple_info->size);
std::memcpy(
tuple_data.data(),
GetData() + tuple_info->offset,
tuple_info->size
);

tuple.SetData(tuple_data);

return true;
}

int TablePage::GetNextPageId() const {

return *reinterpret_cast<const int*>(GetData() + sizeof(uint16_t) * 2);
}

void TablePage::SetNextPageId(int page_id) {
*reinterpret_cast<int*>(GetData() + sizeof(uint16_t) * 2) = page_id;
}

bool TablePage::DeleteTuple(uint16_t slot_num) {
if (slot_num >= GetTupleCount()) {
return false; // Invalid slot number
}
TupleInfo* tuple_info = GetTupleInfo(slot_num);
if (tuple_info->is_deleted) {
return false; // Already deleted
}
tuple_info->is_deleted = true;
return true;
}

bool TablePage::UpdateTuple(uint16_t slot_num, const Tuple& new_tuple) {
if (slot_num >= GetTupleCount()) {
return false;
}
TupleInfo* tuple_info = GetTupleInfo(slot_num);
if (tuple_info->is_deleted) {
return false;
}
const auto& new_tuple_data = new_tuple.GetData();
uint16_t new_size = static_cast<uint16_t>(new_tuple_data.size());


if (new_size <= tuple_info->size) {
std::memcpy(GetData() + tuple_info->offset, new_tuple_data.data(), new_size);
tuple_info->size = new_size;
return true;
}

uint16_t total_free_space = CalculateTotalFreeSpace();
if (new_size > total_free_space) {
return false; // Not enough space even with compaction
}

CompactPage();

uint16_t free_space_pointer = *GetFreeSpacePointer();
free_space_pointer -= new_size;
std::memcpy(GetData() + free_space_pointer, new_tuple_data.data(), new_size);


tuple_info->offset = free_space_pointer;
tuple_info->size = new_size;
*GetFreeSpacePointer() = free_space_pointer;

return true;
}
}
