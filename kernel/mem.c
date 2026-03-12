#include <n7OS/mem.h>
#include <stdio.h>



/**
 * initialisation du tableau bitmap
 * c'est un tableau de unint32_t, une indexe renvoit alors 
 * un nombre sur 32 bits chaque bit montre l'etat d'une page (1 pour occupé et 0 dispo)
 * il nous faut alors un tableau de taille NB_PAGES/32 pour gérer l'etat de toutes les pages
 */

 static uint32_t page_bitmap[NB_PAGES/32];


/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {

    /**
     * trouver le numero de la page
     * ex: si l'adresse est 61440 alors c'est la page 61440/4096(taille d'une page) = 15
     */
    int numero_page = addr / PAGE_SIZE; 
    /**
     * connaissant le numero de la page on peut pouvoir trouver l'indexe dans le tableau bitmap
     * et à partir de l'indexe le bit qui donne l'information
     * sur la page à l'adresse addr
     *  ex: cette page 15 peut-être retrouvée à la position 0 (il ya 32 bit dans cette position)
     * (15 / 32 = 0)
     * pour le bit c'est clairement le 15 bit de cette position 0 (15 % 32 = 15)
     */
    int index = numero_page / 32; 
    int bit = numero_page % 32;
    page_bitmap[index] |= (1 << bit);  


}

/**
 * @brief Désalloue la page
 * 
 * Libère la page allouée.
 * 
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    /**
     * pareil comme setPage mais c'est l'inverse
     */
    int numero_page = addr / PAGE_SIZE; 
    int index = numero_page / 32; 
    int bit = numero_page % 32;
    page_bitmap[index] &= ~(1 << bit);  
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * 
 * @return uint32_t Adresse de la page sélectionnée, ou 0 si aucune page n'est libre.
 */
uint32_t findfreePage() {
    for (int i = 0; i < NB_PAGES/32; i++) {
        if (page_bitmap[i] != 0xFFFFFFFF) { // si tous les bits à la position i ne sont pas à 1, il y a une page de libre
            for (int j = 0; j < 32; j++) { // si non on a de la disponibilité et il faut la trouver 
                if ((page_bitmap[i] & (1 << j)) == 0) {
                    uint32_t page_num = i * 32 + j;
                    uint32_t addr = page_num * PAGE_SIZE;
                    setPage(addr); // on alloue la page
                    return addr; // retourne l'adresse
                }
            }
        }
    }
    return 0; // Retourne 0 si aucune page n'est libre
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 * 
 */
void init_mem() {
    for (int i = 0; i < NB_PAGES/32; i++) {
        page_bitmap[i]= 0x00000000;
    }
    

}

/**
 * @brief Affiche l'état de la mémoire physique
 * 
 */
void print_mem() {
    int used_pages_count = 0;
    int total_pages = NB_PAGES;

    // Compter les pages utilisées en parcourant le bitmap
    for (int i = 0; i < total_pages; i++) {
        int index = i / 32;
        int bit = i % 32;
        if ((page_bitmap[index] & (1 << bit))) {
            used_pages_count++;
        }
    }

    printf("Memory usage: %d / %d pages used.\n", used_pages_count, total_pages);
}