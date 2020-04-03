/*
 * Library for creating modules.
 * Defines: 
 *      1. Modules' information; 
 *      2. Communication between modules;
 */
#ifndef LIBCOORD_H
    #define LIBCOORD_H
        /* structs */
        // Module info
        typedef struct Module {
            char * name;   /* module's name */
            int write_end; /* module's write-end */
            int read_end;  /* module's read-end */
        } Module;
        /* functions */
        // Module information
        Module * get_module(char * name);
        // Module communications
        int send_msg(Module * mod, char * buf, int len);  /* Send message to module  */
        int get_msg(Module * mod, char * buf, int len);   /* Get message from module */
#endif
