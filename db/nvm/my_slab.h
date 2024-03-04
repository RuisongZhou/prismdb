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

#define SECTOR_SIZE 
#define BLOCK_SIZE (8LU*1024LU)

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
    uint64_t pb_id;
    uint64_t offset; // Offset in the slab file
    struct my_bitmap *free_map;
};

struct logic_block {
    uint64_t lb_id;
    struct physical_block **physical_block_array;
    size_t num_physical_blocks;
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

   int fd;
   size_t size_on_disk;

   size_t nb_free_items;
   
   bool hotness;
   struct logic_block **logic_block_array;
   size_t num_logic_blocks;

};

uint64_t encode_key_range(unsigned char* buf);
int my_get_slab_id_new(struct slab_context_new *ctx, size_t item_size, uint64_t key, bool hotness);
void my_add_item_sync(struct slab_context_new *ctx, char *item, size_t item_size, struct op_result *res, bool load_phase_, uint64_t key);
int get_logical_block_id(int slab_id, uint64_t key);
int get_physical_block_id(int slab_id, int l_block_id);
int get_free_idx(int slab_id, int l_block_id, int p_block_id, int item_size);
struct logic_block **init_logic_block_array(int num_logic_blocks);