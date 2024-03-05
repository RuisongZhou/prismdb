#ifndef SLAB_NEW_H
#define SLAB_NEW_H 1

#include "indexes/btree.h"
#include "indexes/memory-item.h"
#include "db/hottracker/hottracker.h"

//#define USE_O_DIRECT

#ifdef USE_O_DIRECT
  #define PAGE_SIZE (512LU)
#else
  #define PAGE_SIZE (4LU*1024LU)
#endif

#define SECTOR_SIZE 1
#define BLOCK_SIZE (8LU*1024LU)
#define MAX_LOGIC_BLOCK_NUM 100
#define MAX_BLOCKS_PER_SLAB 100
#define ALLOCATE_SIZE_BASE (4UL*8LU*1024LU)
#define RESIZE_SIZE_BASE (4UL*8LU*1024LU)

struct kv_pair {
   size_t key_size;
   size_t val_size;
   char *buf;
};

struct op_result {
   int slab_id;
   uint64_t slab_idx;
   int success;
};

struct item_metadata {
   size_t rdt;
   size_t key_size;
   size_t value_size;
   // key
   // value
};
struct physical_block {
    uint64_t id;
    uint64_t offset; // Offset in the slab file
    struct my_freelist *free_map;
};

struct block_context {
   bool alloaccated; // 1 if the block is allocated, 0 if it is free
   struct *physical_block content;
}

struct logic_block {
    uint64_t id;
    struct physical_block **physical_block_array;
    uint64_t num_physical_blocks;
    uint64_t max_writable_size;
};

struct slab_context_new {
   size_t worker_id __attribute__((aligned(64)));        // ID
   struct slab_new **slabs;                                  // Files managed by this worker
   uint64_t rdt;                                         // Latest timestamp
   int nb_slabs;
};

//struct slab_new;

/* Header of a slab -- shouldn't contain any pointer as it is persisted on disk. */
struct slab_new {
   struct slab_context_new *ctx;

   size_t item_size;
   size_t nb_items;   // Number of non freed items
   size_t last_item;  // Total number of items, including freed
   size_t nb_max_items;
   size_t nb_free_items;
   
   int fd;
   size_t size_on_disk;
   bool hotness;
   struct logic_block **logic_block_array;//根据logic block的id来索引
   uint64_t num_logic_blocks;

   struct block_context **slab_block_array;
   uint64_t num_total_blocks;
   uint64_t num_free_blocks;

};

uint64_t encode_key_range(unsigned char* buf);
int my_get_slab_id_new(struct slab_context_new *ctx, size_t item_size, uint64_t key, bool hotness);
void my_add_item_sync(struct slab_context_new *ctx, char *item, size_t item_size, struct op_result *res, bool load_phase_, uint64_t key);
int get_logical_block_id(int slab_id, uint64_t key);
int get_physical_block_id(int slab_id, int l_block_id);
int get_free_idx(int slab_id, int l_block_id, int p_block_id, int item_size);

struct logic_block **init_logic_block_array(int num_logic_blocks);
struct block_context **init_slab_block_array(int num_slab_blocks);
struct slab_new* create_slab_new(struct slab_context_new *ctx, int slab_worker_id, size_t avg_size);