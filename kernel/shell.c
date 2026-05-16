#include <n7OS/shell.h>
#include <n7OS/keyboard.h>
#include <n7OS/processus.h>
#include <string.h>


char cmd_buffer[MAX_COMMAND_LENGTH];
int index;

void get_input_line(char* buffer) {
    index = 0;
    while (1) {
        char c = kgetch();
        if (c == '\n' || c == KEY_RETURN) {
            buffer[index] = '\0';
            printf("\n");
            break;
        }
        if (c == KEY_BACKSPACE) {
            if (index > 0) {
                index--;
                buffer[index] = '\0';
                printf("\b \b");
            }
            continue;
        }
        buffer[index] = c;
        index++;
        printf("%c", c);
    }
}

void execute_command(char* cmd){
    if (strcmp(cmd, "ps") == 0){
        ps();
    }else if (strcmp(cmd, "clear") == 0){
        clear_screen();
    }else if (strcmp(cmd, "help") == 0){
        printf("Commandes disponibles :\n");
        printf("ps : afficher la liste des processus\n");
        printf("kill <pid> : tuer un processus\n");
        printf("block <pid> : bloquer un processus\n");
        printf("unblock <pid> : debloquer un processus\n");
        printf("interrupt <pid> : interrompre un processus\n");
        printf("start <pid> : demarrer un processus\n");
        printf("clear : effacer l'ecran\n");
        printf("help : afficher l'aide\n");
        printf("shutdown : eteindre le systeme\n");
    }else if (strcmp(cmd, "shutdown") == 0){
        shutdown(1);
    }else if (strncmp(cmd, "kill ", 5) == 0) {
        int pid = atoi(cmd + 5);
        terminate_process(pid);
    } else if (strncmp(cmd, "block ", 6) == 0) {
        int pid = atoi(cmd + 6);
        block_process(pid);
    } else if (strncmp(cmd, "unblock ", 8) == 0) {
        int pid = atoi(cmd + 8);
        unblock_process(pid);
    }else if (strncmp(cmd, "interrupt ", 10) == 0) {
        int pid = atoi(cmd + 10);
        interrupt_process(pid);
    }else if (strncmp(cmd, "start ", 6) == 0) {
        int pid = atoi(cmd + 6);
        start_process(pid);
    }
    else{
        printf("Commande inconnue\n");
    }
    
}

void shell_run(){
    while(true){
        printf("bonheur-eldjimbaye@devOS> ");
        get_input_line(cmd_buffer);
        execute_command(cmd_buffer);
    }
}
    
