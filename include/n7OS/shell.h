/** @file shell.h
 * interface du shell
 */

#ifndef SHELL_H
#define SHELL_H

#define MAX_COMMAND_LENGTH 255


/** lit une ligne de commande 
 * @param buffer buffer pour stocker la commande
 */
void get_input_line(char* buffer);

/** exécute une commande
 *  @param buffer buffer contenant la commande
 */
void execute_command(char* buffer);

/** la boucle principale du shell
 *  affiche le prompt /_ 
 *  lit une commande
 *  execute la commande
 *  recommence
 */
void shell_run();


#endif /* SHELL_H */