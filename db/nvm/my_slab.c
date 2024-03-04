#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "slab_new.h"
#include "freelist_new.h"

#ifdef USE_O_DIRECT
int use_o_direct = 1;
#else
int use_o_direct = 0;
#endif

uint64_t encode_key_range(unsigned char* buf){

}


struct logic_block **init_logic_block_array(int num_logic_blocks) {
    struct logic_block **logic_block_array = (struct logic_block **)malloc(sizeof(struct logic_block *) * num_logic_blocks);
    int num_physical_blocks = 1;
    //only init the first logic block
    for (int i = 0; i < num_logic_blocks; i++) {
        if (i == 0) {
            logic_block_array[i] = (struct logic_block *)malloc(sizeof(struct logic_block));
            logic_block_array[i]->lb_id = 0;
            logic_block_array[i]->num_physical_blocks = num_physical_blocks;
            logic_block_array[i]->physical_block_array = (struct physical_block **)malloc(sizeof(struct physical_block *) * num_physical_blocks);
            struct physical_block **physical_block_array = logic_block_array[i]->physical_block_array;
            //only init the first physical block
            for (int j = 0; j < num_physical_blocks; j++) {
                if (j == 0) {
                    physical_block_array[j] = (struct physical_block *)malloc(sizeof(struct physical_block));
                    physical_block_array[j]->pb_id = 0;
                    physical_block_array[j]->offset = 0;
                    init_freelist(physical_block_array[j]->free_map);                  
                } else {
                    physical_block_array[j] = NULL;
                }
            }
        } else {
            logic_block_array[i] = NULL;
        }
    }
    return logic_block_array;
}


int my_get_slab_id_new(struct slab_context_new *ctx, size_t cur_item_size, uint64_t key, bool hotness) {

}

int get_logical_block_id(int slab_id, uint64_t key) {

}

int get_physical_block_id(int slab_id, int l_block_id) {

}

int get_free_idx(int slab_id, int l_block_id, int p_block_id, size_t item_size) {

}

void my_add_item_sync(struct slab_context_new *ctx, char *item, size_t item_size, struct op_result *res, bool load_phase_, uint64_t key) {
    bool hotness = CheckHotess(key);
    int slab_id = my_get_slab_id_new(ctx, key);
    int l_block_id = get_logical_block_id(slab_id, key);
    int p_block_id = get_physical_block_id(slab_id, l_block_id);
    int free_idx = get_free_idx(slab_id, l_block_id, p_block_id, item_size);
    size_t idx;

    if (free_idx > -1) {
      //fprintf(stderr, "FREELIST: get from freelist: idx = %d\n", free_idx);
      idx = (size_t) free_idx;
    } else {
      // if no free slot, add to the end of slab file
      // resize the slab if reaching its full capacity
      if (slab->last_item >= slab->nb_max_items) {
         resize_slab_new(slab);
      }
      idx = slab->last_item;
      //fprintf(stderr, "FREELIST: freelist is full, append to the end of file: idx = %d\n", idx);
      slab->last_item++;
    }

    res->slab_idx = (uint64_t) idx;
    insert_item_at_idx(slab, item, item_size, idx, res, load_phase_);
    //fprintf(stderr, "FREELIST: after insert_item_at_idx\n");
    return;
}

struct slab_new* create_slab_new(struct slab_context_new *ctx, int slab_worker_id, size_t avg_size) {
    struct stat sb;
    char path[512];
    struct slab_new *s = calloc(1, sizeof(*s));

    size_t disk = 0;

    sprintf(path, PATH, disk, slab_worker_id, 0LU, avg_size);
    // JIANAN: TODO
    // If file is opened with O_DIRECT flag, for synchronous IO, flush + fsync?
    // JIANAN: use kernel page cache
    if (use_o_direct) {
    //s->fd = open(path,  O_RDWR | O_CREAT | O_DIRECT | O_SYNC, 0777);
        s->fd = open(path,  O_RDWR | O_CREAT | O_DIRECT, 0777);
    } else {
        s->fd = open(path,  O_RDWR | O_CREAT, 0777);
    }
    if(s->fd == -1)
        fprintf(stderr, "Cannot allocate slab %s, error code: %d, error msg: %s\n", path, errno, strerror(errno));

    fstat(s->fd, &sb);
    s->size_on_disk = sb.st_size;
    if(s->size_on_disk < 2 * PAGE_SIZE) {
        // JIANAN: fallocate only supported on xfs, ext4, btrfs, tmpfs, gfs
        int allocateRes = fallocate(s->fd, 0, 0, 2 * PAGE_SIZE);
        s->size_on_disk = 2 * PAGE_SIZE;
        //fprintf(stderr, "fallocate returns %d, fd %d\n", allocateRes, s->fd);
    }

    size_t nb_items_per_page = PAGE_SIZE / avg_size;
    s->nb_max_items = s->size_on_disk / PAGE_SIZE * avg_size;
    s->nb_items = 0;
    s->item_size = avg_size;
    s->nb_free_items = 0;
    s->last_item = 0;
    s->ctx = ctx;
    s->hotness = false;
    //init logic block array
    fprintf(stderr, "before init_logic_block_array\n");
    struct logic_block **logic_block_array = init_logic_block_array(1);
    s->logic_block_array = logic_block_array;
    s->num_logic_blocks = 1;
    fprintf(stderr, "after init_logic_block_array\n");

    return s;
}

void insert_item_at_idx(struct slab_new *slab, char *item, size_t item_size, size_t idx, struct op_result *res, bool load_phase_) {

   // 1. Get the disk page data
   size_t page_num = item_page_num_new(slab, idx);
   #ifdef USE_O_DIRECT
   char page[PAGE_SIZE] __attribute__((aligned(PAGE_SIZE)));
   #else
   char page[PAGE_SIZE];
   #endif
   //char page[PAGE_SIZE];
   off_t page_offset = page_num*PAGE_SIZE;
   int r = pread(slab->fd, page, PAGE_SIZE, page_offset);
   if(r != PAGE_SIZE)
      printf(stderr,"pread failed! Read %d instead of %lu (offset %lu)\n", r, PAGE_SIZE, page_offset);
   //fprintf(stderr, "%s\n", "get disk page");

   // get the byte offset within the page
   off_t pg_offset = item_in_page_offset_new(slab, idx);
   //fprintf(stderr, "get page byte offset %lld\n", pg_offset);

   // update the disk page
   memcpy(&page[pg_offset], item, item_size);

   // write back to the file
   // TODO: buffer wrrites
   //fprintf(stderr, "pwrite starts, fd %d, PAGE_SIZE %d, offset %d\n", slab->fd, PAGE_SIZE, page_num*PAGE_SIZE);
   int res_pwrite = pwrite(slab->fd, page, PAGE_SIZE, page_num*PAGE_SIZE);
   if (res_pwrite == -1) {
      fprintf(stderr, "pwrite fails, error code: %d, error msg: %s\n", errno, strerror(errno));
      res->success = -1;
      return;
   }
   //fprintf(stderr, "%s\n", "pwrite");

   // flush and sync
   // JIANAN: TODO: double check this, do we need fflush() or O_SYNC flag when opening this file?
   if (!load_phase_){
     int res_sync = fdatasync(slab->fd);
     if (res_sync == -1) {
         fprintf(stderr, "fdatasync fails, error code: %d, error msg: %s\n", errno, strerror(errno));
         res->success = -1;
         return;
       }
     //fprintf(stderr, "%s\n", "fdatasync done");
   }

   res->success = 0;
   //fprintf(stderr, "insert_item_at_index returns\n");
   return;
}
