#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include <n7OS/timer.h>
#define X0   1 //pour l'affichage de mon console, je comment à afficher à partir de la ligne 1 et colonne 1
#define Y0   1


uint16_t *scr_tab;
static int cursor_x = 0;
static int cursor_y = 0;



void clear_screen() { //pour effacer ce qui s'affiche par defaut sur l'ecran
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
            scr_tab[i] = (CHAR_COLOR << 8) | ' ';
        }
}
void init_console() {
    scr_tab= (uint16_t *) SCREEN_ADDR;
    cursor_x = X0;
    cursor_y = Y0;
    clear_screen();
    console_cursor(cursor_x, cursor_y);
}

void console_cursor(int x, int y) {
    uint16_t pos = y * VGA_WIDTH + x;

    outb(CMD_LOW, PORT_CMD);         // envoi de la commande 15
    outb(pos, PORT_DATA); // Envoie l'octet de poids faible

    outb(CMD_HIGH, PORT_CMD);         // envoie de la commande 14
    outb(pos >> 8, PORT_DATA); // Envoie l'octet de poids fort
}

void console_putchar(const char c) {
    if (c > 31 && c < 127){ //affiché les caractères imprimables
        
        int pos = cursor_y * VGA_WIDTH + cursor_x;
        scr_tab[pos] = (CHAR_COLOR << 8) | c;
        cursor_x++;
        if (cursor_x > (VGA_WIDTH-2)) { // revenir à la suivante quand on atteint la colonne max de l'écran
            cursor_x = X0;
            cursor_y++;
        }
    } else if (c == 8) {// pour deplacer le curceur d'une colonne en arrière

        if (cursor_x > X0) cursor_x--;
    } else if (c == 9){ // ajouter un espace de 8 caractère

        cursor_x += 8 - (cursor_x % 8);
    }else if (c == 10){ // déplace le curseur à la ligne suivante, colone 0
        cursor_x = X0;
        cursor_y++;
    }else if (c == 12){ // efface l'ecran et révient à la colonne 0 de la ligne 0
       
        for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
            scr_tab[i] = (CHAR_COLOR << 8) | ' ';
        }
        cursor_x = X0;
        cursor_y = Y0;
    }else if (c == 13){ // déplace le curseur à la conlonne 0 de la ligne courante
        cursor_x = X0;
    }
    
    console_cursor(cursor_x, cursor_y);
}

void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}