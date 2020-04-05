// ----------------------------------- Headers ----------------------------------
#include "libcoord.h"

// ---------------------------------- Prototypes --------------------------------
// Additional funcs

// ---------------------------------- Main funcs --------------------------------
/*
 * Initialize new module
 * Notify Coordinator about self-existence
 * Initialize modules' data storage
 */
int
init_module() {
}

/*
 * Get module info by provided @name
 * Check modules' data storage
 * If exist, returns pointer to Module 
 * Otherwise returns NULL
 */
Module* 
get_module_by_name(char *name) {
}

/*
 * Get module info by provided @id
 * Check modules' data storage
 * If exist, returns pointer to Module
 * Otherwise returns NULL
 */
Module* 
get_module_by_id(mod_id id) {
}

/*
 * Connect to module provided @mod
 * Send request for connection to Coordinator
 * If connection can be established, update requested module's information in data storage and returns 0 (success) 
 * If no, returns -1 (failure)
 */
int
connect_to_module(Module *mod) {
}

/*
 * Send msg to module by provided @mod
 * Message represented by @buf (actual message here) and @len (length of message)
 */
int
send_msg(Module *mod, char *buf, msg_len len) {
}

/*
 * Handle incoming messages
 * First use default defined handler 
 * Then use user-provded @handler
 */
int
handle(int (*handler)(Message*)) {
}

/*
 * Get any incoming message
 * Check if any read-ends contains message and can be readed
 * If yes, return pointer to first founded Message
 * Otherwise returns NULL
 */
Message*
get_msg() {
}
// ------------------------------- Additional funcs -----------------------------
