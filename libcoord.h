/*
 * Library for creating modules.
 * Defines: 
 *      1. Module's info; 
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
        // Module communications
        int send_msg(int write_end, char * buf, int len); /* Send message to module  */
        int get_msg(int read_end, char * buf, int len);   /* Get message from module */
#endif
