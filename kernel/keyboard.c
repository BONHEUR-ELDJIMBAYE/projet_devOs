#include <n7OS/keyboard.h>
#include <n7OS/cpu.h>

extern void handler_IT_keyboard(void);
char keyboard_buffer[KEYB_BUFFER_SIZE];

int queue = 0;
int debut = 0;
int shift_state = 0; // Variable globale pour suivre l'état du Shift

void init_keyboard() {
    //demasquer le clavier
    outb(inb(0x21) & 0xFD, 0x21);

    //initialiser l'entrée 33 de la table des interruptions pour le clavier
    init_irq_entry(0x21, (uint32_t) handler_IT_keyboard);

}

void handler_keyboard() {
    outb(0x20, 0x20);

    uint16_t code = inb(KEYB_ENCODER);
    
    // Gérer le relâchement du Shift spécifiquement
    if (code == SHIFT_RELEASED) {
        shift_state = 0;
        return;
    }
    
    // Gérer les autres touches relâchées
    if (IS_KEY_RELEASED(code)) {
        return;
    }
    
    // Gérer la pression du Shift
    if (code == SHIFT_PRESSED) {
        shift_state = 1;
        return; // Ne pas ajouter Shift au buffer
    }
    
    // Obtenir le caractère selon l'état du Shift
    char c = shift_state ? scancode_map_shift[code] : scancode_map[code];
    
    if (c != 0) {
        keyboard_buffer[queue] = c;
        queue = (queue + 1) % KEYB_BUFFER_SIZE;
    }
}

char kgetch() {
    /* on attend qu'un caractère soit disponible */
    while (debut == queue) {}

    char c = keyboard_buffer[debut];
    debut = (debut + 1) % KEYB_BUFFER_SIZE;
    return c;

}