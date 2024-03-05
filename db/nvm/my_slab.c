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
    return 0;
}

struct *physical_block init_physical_block(int block_id) {
    struct *physical_block p_block = (struct *physical_block)malloc(sizeof(struct physical_block));
    p_block->id = block_id;
    p_block->offset = block_id * BLOCK_SIZE;
    init_block_freelist(p_block->free_map);
}

void add_slab_block_array(struct slab_new *s, uint64_t add_blocks_num) {
    uint64_t ori_blocks_num = s->num_total_blocks;
    for (int i = ori_blocks_num; i < ori_blocks_num + add_blocks_num; i++) {
        struct block_context *block_ctx = (struct block_context *)malloc(sizeof(struct block_context));
        block_ctx->allocated = false;
        block_ctx->content = init_physical_block(i);
        s->slab_block_array[i] = block_ctx;
    }
    s->num_total_blocks += add_blocks_num;
    s->empty_blocks += add_blocks_num;
}

struct block_context **init_slab_block_array(int num_slab_blocks) {
    struct block_context **slab_block_array = (struct block_context **)malloc(sizeof(struct block_context *) * num_slab_blocks);
    //only init the first slab block
    for (int i = 0; i < num_slab_blocks; i++) {
        slab_block_array[i] = NULL;
    }
    return slab_block_array;
}

// pre allocate
struct logic_block **init_logic_block_array(int num_logic_blocks) {
    struct logic_block **logic_block_array = (struct logic_block **)malloc(sizeof(struct logic_block *) * num_logic_blocks);
    //only init the first logic block
    for (int i = 0; i < num_logic_blocks; i++) {
        logic_block_array[i] = NULL;
    }
    return logic_block_array;
}


int my_get_slab_id_new(struct slab_context_new *ctx, size_t cur_item_size, uint64_t key, bool hotness) {
    return 0;
}

int get_logical_block_id(int slab_id, uint64_t key) {
    return 0;
}

int get_physical_block_id(int slab_id, int l_block_id) {
    return 0;
}

int get_free_idx(int slab_id, int l_block_id, int p_block_id, size_t item_size) {
    return 0;
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
    if(s->size_on_disk < ALLOCATE_SIZE_BASE) {
        // JIANAN: fallocate only supported on xfs, ext4, btrfs, tmpfs, gfs
        int allocateRes = fallocate(s->fd, 0, 0, ALLOCATE_SIZE_BASE);
        s->size_on_disk = ALLOCATE_SIZE_BASE;
        //fprintf(stderr, "fallocate returns %d, fd %d\n", allocateRes, s->fd);
    }

    s->ctx = ctx;
    s->hotness = false;

    struct logic_block **logic_block_array = init_logic_block_array(MAX_LOGIC_BLOCK_NUM);
    s->logic_block_array = logic_block_array;
    s->num_logic_blocks = 0;
    s->
    struct block_context **slab_block_array = init_slab_block_array(MAX_BLOCK_PER_SLAB);
    add_slab_block_array(s, s->size_on_disk / BLOCK_SIZE);

    return s;
}

struct slab_new* resize_slab_new(struct slab_new *s) {
    uint64_t ori_size = s->size_on_disk;
    s->size_on_disk += RESIZE_SIZE_BASE;
    if(fallocate(s->fd, 0, 0, s->size_on_disk)) {
        fprintf(stderr, "Cannot resize slab (origin size %lu) new size %lu\n", ori_size, s->size_on_disk);
    }
    add_slab_block_array(s, s->size_on_disk / BLOCK_SIZE - ori_size / BLOCK_SIZE);   
}

off_t sector_page_num_new(struct slab_new *s, size_t idx) {
    size_t sectors_per_page = PAGE_SIZE / SECTRO_SIZE;
    return idx / sectors_per_page;
}

off_t sector_in_page_offset_new(struct slab_new *s, size_t idx) {
    size_t sectors_per_page = PAGE_SIZE / SECTRO_SIZE;
    return (idx % sectors_per_page) * SECTOR_SIZE;
}

void insert_item_at_idx(struct slab_new *slab, char *item, size_t item_size, size_t idx, struct op_result *res, bool load_phase_) {

    // 1. Get the disk page data
    size_t page_num = sector_page_num_new(slab, idx);
    #ifdef USE_O_DIRECT
    char page[PAGE_SIZE] __attribute__((aligned(PAGE_SIZE)));
    #else
    char page[PAGE_SIZE];
    #endif
    //char page[PAGE_SIZE];
    off_t page_offset = page_num * PAGE_SIZE;
    int r = pread(slab->fd, page, PAGE_SIZE, page_offset);
    if(r != PAGE_SIZE)
        printf(stderr,"pread failed! Read %d instead of %lu (offset %lu)\n", r, PAGE_SIZE, page_offset);
    //fprintf(stderr, "%s\n", "get disk page");

    // get the byte offset within the page
    off_t pg_offset = sector_in_page_offset_new(slab, idx);
    //fprintf(stderr, "get page byte offset %lld\n", pg_offset);

    // update the disk page
    memcpy(&page[pg_offset], item, item_size);

    // write back to the file
    // TODO: buffer wrrites
    //fprintf(stderr, "pwrite starts, fd %d, PAGE_SIZE %d, offset %d\n", slab->fd, PAGE_SIZE, page_num*PAGE_SIZE);
    int res_pwrite = pwrite(slab->fd, page, PAGE_SIZE, page_num * PAGE_SIZE);
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
