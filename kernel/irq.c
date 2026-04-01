#include <inttypes.h>
#include <n7OS/irq.h>


void init_irq_entry(int irq_num, uint32_t addr) {

    idt_entry_t *entry = (idt_entry_t *) &idt[irq_num];//idt est un tableau de 256 entrées, chaque entrée est de type idt_entry_t
    entry->offset_inf = addr & 0xFFFF; //on prend les 16 bits de poids faible de l'adresse
    entry->sel_segment = KERNEL_CS; // KERNEL_CS  le sélecteur de segment pour le code du noyau
    entry->zero = 0; // champ réservé
    entry->type_attr = 0x8E; //présent, DPL=0, S=0, GateType=1110 (interrupt gate (32 bits) numéro 14)
    entry->offset_sup = (addr >> 16) & 0xFFFF;//on prend les 16 bits de poids fort de l'adresse


}
