#include "vm_stats.h"
#include <types.h>
#include <lib.h>

int tlb_faults = 0;
int tlb_faults_free = 0;
int tlb_faults_replace = 0;
int tlb_invalidations = 0;
int tlb_reloads = 0;
int pf_zeroed = 0;
int pf_disk = 0;
int pf_elf = 0;
int pf_swap = 0;

void vm_shutdown(void){
	kprintf("Number of TLB faults: %d\n", tlb_faults);
	kprintf("Number of TLB faults with Free: %d\n", tlb_faults_free);
	kprintf("Number of TLB faults with Replace: %d\n", tlb_faults_replace);
	kprintf("Number of TLB invalidations: %d\n", tlb_invalidations);
	kprintf("Number of TLB reloads: %d\n", tlb_reloads);
}

void increment_tlb_faults(void){
	tlb_faults++;
}
void increment_tlb_faults_free(void){
	tlb_faults_free++;
}
void increment_tlb_faults_replace(void){
	tlb_faults_replace++;
}
void increment_tlb_invalidations(void){
	tlb_invalidations++;
}
void increment_tlb_reloads(void){
	tlb_reloads++;
}
void increment_pf_zeroed(void){
	pf_zeroed++;
}
void increment_pf_disk(void){
	pf_disk++;
}
void increment_pf_elf(void){
	pf_elf++;
}
void increment_pf_swap(void){
	pf_swap++;
}

