#include <n7OS/paging.h>
#include <n7OS/mem.h>
#include <stddef.h> 


DirectoryTable  repertoire_page;

void initialise_paging() {
    init_mem(); // Initialiser la mémoire avant de configurer la pagination
    // Configuration de la pagination (création de du répertoire de page, des tables de page, etc.)
    repertoire_page = (DirectoryTable)kmalloc_a(sizeof(page_directory_entry_t) * 1024); // Allouer de la mémoire pour le répertoire de page
    setPage((uint32_t)repertoire_page); // Marquer le répertoire de page comme utilisé dans la bitmap de pages

    // Initialiser le répertoire de page 
    for (int i = 0; i < 1024; i++) {
        PageTable page_table = (PageTable)kmalloc_a(sizeof(page_table_entry_t) * 1024); 
        // for (int j = 0; j < 1024; j++){
        //     page_table[j].value = 0; 

        // }
        repertoire_page[i].directory_entry.PAGE_ADRESS = (uint32_t)page_table >> 12; 
        repertoire_page[i].directory_entry.P = 1; 
        repertoire_page[i].directory_entry.W = 1; 
        repertoire_page[i].directory_entry.U = 0; 
    }

    //reservation de la table de page du noyau
    for(uint32_t i = 0; i < 1024; i++){
        alloc_page_entry(i*PAGE_SIZE, 1, 1);
    }

    //activer la pagination
    // uint32_t addr_cr3 = (uint32_t)repertoire_page;
    // __asm__ __volatile__ ( "mov %0, %%cr3" :: "r" (addr_cr3) );//ajouter le repertoire de page au registre cr3
    
    // uint32_t cr0;
    // __asm__ __volatile__ ( "mov %%cr0, %0" : "=r" (cr0) );//recuperer la valeur du registre cr0
    
    // //positionner le bit 31 du registre cr0 à 1 pour activer la pagination
    // cr0 = cr0 | 0x80000000;
    // __asm__ __volatile__ ( "mov %0, %%cr0" :: "r" (cr0) );//activer la pagination

    setup_base((uint32_t)repertoire_page);
}



PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel){

    AV add_virtuel;
    add_virtuel.value = address;
    
    //découpage de l'addresse virtuel
    uint32_t index_table = add_virtuel.addresse_virtuel.I_TABLE;
    uint32_t index_dir = add_virtuel.addresse_virtuel.I_DIR;

    
    
    //récuprération du repertoire de page
    DTE directory_page_entry = repertoire_page[index_dir];
    //récuprération de la table de page
    PageTable page_table = (PageTable)(directory_page_entry.directory_entry.PAGE_ADRESS <<12);
    //récupération de l'entrée de la table de page
    PTE page_table_entry = page_table[index_table];
    
    //maper la page physique à l'adresse virtuelle
    uint32_t page_addr = findfreePage();
    page_table_entry.page_entry.PAGE_ADRESS = page_addr >> 12;
    page_table_entry.page_entry.P = 1;
    page_table_entry.page_entry.W = is_writeable;
    page_table_entry.page_entry.U = !is_kernel;
    return page_table;
}
