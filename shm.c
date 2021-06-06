#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

struct {
    struct spinlock lock;
    struct shm_page {
        uint id;
        char *frame;
        int refcnt;
    } shm_pages[64];
} shm_table;

void shminit() {
    int i;
    initlock(&(shm_table.lock), "SHM lock");
    acquire(&(shm_table.lock));
    for (i = 0; i< 64; i++) {
        shm_table.shm_pages[i].id =0;
        shm_table.shm_pages[i].frame =0;
        shm_table.shm_pages[i].refcnt =0;
    }
    release(&(shm_table.lock));
}

int shm_open(int id, char **pointer) {
    int i ;
    int sz = myproc()->sz ;
    int first_unused_page = 0 ;
    int found_id = 0 ;
    int found_unused_page = 0 ;
    int matching_id_index = 0 ;
    acquire(&(shm_table.lock));
    for (i = 0; i< 64; i++) {
        if(shm_table.shm_pages[i].id == id){
            matching_id_index = i ;
            found_id = 1 ;
            break ;
        }
        else if(found_unused_page == 0){
            if(shm_table.shm_pages[i].id == 0){
                first_unused_page = i ;
                found_unused_page = 1 ;
            }
        }
    }
    if(found_id == 1){ // already exists
        mappages(myproc()->pgdir, (void*) PGROUNDUP(sz), PGSIZE, V2P(shm_table.shm_pages[matching_id_index].frame), PTE_W|PTE_U) ;
        shm_table.shm_pages[matching_id_index].refcnt += 1 ;
        *pointer = (char *) PGROUNDUP(sz) ;
    }
    else{ // kalloc new page
        shm_table.shm_pages[first_unused_page].id = id ;
        shm_table.shm_pages[first_unused_page].frame = kalloc() ;
        memset(shm_table.shm_pages[first_unused_page].frame, 0, PGSIZE);
        shm_table.shm_pages[first_unused_page].refcnt = 1 ;
        mappages(myproc()->pgdir, (void*) sz, PGSIZE, V2P(shm_table.shm_pages[first_unused_page].frame), PTE_W|PTE_U) ;
        *pointer = (char *) sz ;
    }
    myproc()->sz += PGSIZE ;
    release(&(shm_table.lock));
    return 0; //added to remove compiler warning -- you should decide what to return
}


int shm_close(int id) {
    int i ;
    acquire(&(shm_table.lock));
    for (i = 0; i < 64; i++) {
        if(shm_table.shm_pages[i].id == id){
            if(shm_table.shm_pages[i].refcnt <= 1){
                shm_table.shm_pages[i].id =0;
                shm_table.shm_pages[i].frame =0;
                shm_table.shm_pages[i].refcnt =0;
            }
            else
                shm_table.shm_pages[i].refcnt -= 1 ;
            break ;
        }
    }
    release(&(shm_table.lock));
    return 0; //added to remove compiler warning -- you should decide what to return
}