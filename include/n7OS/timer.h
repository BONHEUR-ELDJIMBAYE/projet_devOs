#ifndef TIMER_H
#define TIMER_H


#define F_OSC 0x1234BD
#define HORLOGE 1000
#define FREQUENCE F_OSC / HORLOGE

/**
 * @brief Initialise le timer
 *
 * Cette fonction initialise le timer pour qu'il génère une interruption à une fréquence de 1.19MHz.
 */
void init_timer();

/**
 * @brief Handler de l'interruption du timer
 *
 * Cette fonction est appelée à chaque fois que le timer génère une interruption.
 */
void handler_timer();

/**
 * @brief conversion du compteur au format hh:mm:ss
 */
void convert_counter_to_time(int *hours, int *minutes, int *seconds);

#endif