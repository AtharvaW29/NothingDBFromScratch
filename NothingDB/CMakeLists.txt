cmake_minimum_required(VERSION 4.2)
# Add source
add_executable (NothingDB   
				"include/common/config.h" 
				"include/storage/page.h" 
				"src/storage/page.cpp" 
				"src/NothingDB.h" 
				"src/NothingDB.cpp" 
				"include/storage/disk_manager.h" 
				"src/storage/disk_manager.cpp" 
				"include/buffer/lru_replacer.h" 
				"src/buffer/lru_replacer.cpp" 
				"include/buffer/bufferpool_manager.h" 
				"src/buffer/bufferpool_manager.cpp" 
				"include/storage/table_page.h" 
				"src/storage/table_page.cpp" 
				"include/common/types.h" 
				"include/catalog/column.h" 
				"include/catalog/schema.h" 
				"include/common/value.h" 
				"include/storage/tuple.h" 
				"src/storage/tuple.cpp" 
				"include/storage/table_heap.h" 
				"src/storage/table_heap.cpp" "include/storage/rid.h" "include/index/bplus_tree_node.h" "include/index/bplus_tree.h" "src/index/bplus_tree.cpp" "src/index/bplus_tree_node.cpp" )

target_include_directories(NothingDB PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NothingDB PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
