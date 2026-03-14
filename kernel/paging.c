#include <n7OS/paging.h>
#include <stddef.h> 


DirectoryTable  repertoire_page;

void initialise_paging() {
    init_mem(); // Initialiser la mémoire avant de configurer la pagination
    // Configuration de la pagination (création de du répertoire de page, des tables de page, etc.)
    repertoire_page = (DirectoryTable)kmalloc_a(sizeof(page_directory_entry_t) * 1024); // Allouer de la mémoire pour le répertoire de page

    // Initialiser le répertoire de page et les tables de page selon les besoins du système
    for (int i = 0; i < 1024; i++) {
        repertoire_page[i].directory_entry.PAGE_ADRESS = 0; // Allouer de la mémoire pour la table de page
        repertoire_page[i].directory_entry.P = 0; // Marquer la table de page comme absente
        repertoire_page[i].directory_entry.W = 0; // Marquer la table de page comme inaccessible en écriture
        repertoire_page[i].directory_entry.U = 0; // Marquer la table de page comme utilisable par le noyau
    }

    //reservation de la table de page du noyau
    for(uint32_t i = 0; i < 1024; i++){
        alloc_page_entry(i, 1, 1);
    }

    //activer la pagination
    uint32_t addr_cr3 = (uint32_t)repertoire_page;
    __asm__ __volatile__ ( "mov %0, %%cr3" :: "r" (addr_cr3) );//ajouter le repertoire de page au registre cr3
    
    uint32_t cr0;
    __asm__ __volatile__ ( "mov %%cr0, %0" : "=r" (cr0) );//recuperer la valeur du registre cr0
    
    //positionner le bit 31 du registre cr0 à 1 pour activer la pagination
    cr0 = cr0 | 0x80000000;
    __asm__ __volatile__ ( "mov %0, %%cr0" :: "r" (cr0) );//activer la pagination
}



PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel){

    AV add_virtuel;
    add_virtuel.value = address;
    
    //découpage de l'addresse virtuel
    uint32_t index_table = add_virtuel.addresse_virtuel.I_TABLE;
    uint32_t index_dir = add_virtuel.addresse_virtuel.I_DIR;
    uint32_t index_page = add_virtuel.addresse_virtuel.I_PAGE;

    if(repertoire_page[index_dir].directory_entry.P == 0){//si la table de page n'est pas allouée
        PageTable page_table = (PageTable)kmalloc_a(sizeof(page_table_entry_t) * 1024);
        uint32_t page_table_addr = (uint32_t)page_table;    
        repertoire_page[index_dir].directory_entry.PAGE_ADRESS = page_table_addr >> 12;
        repertoire_page[index_dir].directory_entry.P = 1;
        repertoire_page[index_dir].directory_entry.W = 1;
        repertoire_page[index_dir].directory_entry.U = 1;
    }
    
    //récuprération de du repertoire de page
    DTE directory_page_entry = repertoire_page[index_dir];
    //récuprération de la table de page
    PageTable page_table = (PageTable)(directory_page_entry.directory_entry.PAGE_ADRESS <<12);
    //récupération de l'entrée de la table de page
    PTE page_table_entry = page_table[index_page];
    
    //maper la page physique à l'adresse virtuelle
    uint32_t page_addr = findfreePage();
    page_table_entry.page_entry.PAGE_ADRESS = page_addr >> 12;
    page_table_entry.page_entry.P = 1;
    page_table_entry.page_entry.W = is_writeable;
    page_table_entry.page_entry.U = !is_kernel;
    return page_table;
}
