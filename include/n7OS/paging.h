/**
 * @file paging.h
 * @brief Gestion de la pagination dans le noyau
 */
#ifndef _PAGING_H
#define _PAGING_H

#include <inttypes.h>

/**
 * @brief Description d'une ligne dans le repertoire de page
 * 
 */
typedef struct {
    uint32_t P             : 1; //table de page présente en mémoire
    uint32_t W             : 1; // table de page accessible en lecture/écriture (0 : lecture seule)
    uint32_t U             : 1; //page utilisateur; U = 0 : page en mode noyau
    uint32_t RESERVE       : 9; //reserve
    uint32_t PAGE_ADRESS  : 20; //adresse de la page en mémoire 
} page_directory_entry_t;

/**
 * @brief Une entrée dans le repertoire de page peut être manipulée en utilisant
 *        la structure page_directory_entry_t ou directement la valeur
 */
typedef union {
    page_directory_entry_t directory_entry;
    uint32_t value;
} DTE; // DTE = Directory Table Entry 


/**
 * @brief Un repertoire de page est un tableau de descripteurs de page
 * 
 */
typedef DTE * DirectoryTable;

/**
 * @brief Description d'une ligne dans la table de page
 * 
 */
typedef struct {
    uint32_t P             : 1; //table de page présente en mémoire
    uint32_t W             : 1; // table de page accessible en lecture/écriture (0 : lecture seule)
    uint32_t U             : 1; // page utilisateur
    uint32_t RSVD1         : 2; //bits réservés 1
    uint32_t A             : 1;
    uint32_t D             : 1;
    uint32_t RSVD2         : 2;//bits réservés 2
    uint32_t AVAIL         : 3; //reserve
    uint32_t PAGE_ADRESS  : 20; //adresse de la page en mémoire 
} page_table_entry_t;


/**
 * @brief Une entrée dans la table de page peut être manipulée en utilisant
 *        la structure page_table_entry_t ou directement la valeur
 */
typedef union {
    page_table_entry_t page_entry;
    uint32_t value;
} PTE; // PTE = Page Table Entry 


/**
 * @brief Une table de page (PageTable) est un tableau de descripteurs de page
 * 
 */
typedef PTE * PageTable;



/**
 * @brief l'adresse virtuelle
 * 
 */
typedef struct {
    uint32_t I_PAGE       : 12; // indice de page
    uint32_t I_TABLE      : 10; // indice de table
    uint32_t I_DIR        : 10; // indice de repertoire de page
   
} adresse_virtuel_t;

/**
 * @brief ue adresse virtuel peut-être utilisée en utilisant la structure de adresse_virtuel_t ou
 * directement la valeur
 */
typedef union{
    adresse_virtuel_t adresse_virtuel;
    uint32_t value;
} AV; //AV = adresse virtuelle


/**
 * @brief Cette fonction initialise le répertoire de page, alloue les pages de table du noyau
 *        et active la pagination
 * 
 */
void initialise_paging();

/**
 * @brief Cette fonction alloue une page de la mémoire physique à une adresse de la mémoire virtuelle
 * 
 * @param address       Adresse de la mémoire virtuelle à mapper
 * @param is_writeable  Si is_writeable == 1, la page est accessible en écriture
 * @param is_kernel     Si is_kernel == 1, la page ne peut être accédée que par le noyau
 * @return PageTable    La table de page modifiée
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);
#endif