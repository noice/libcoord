// ----------------------------------- Headers ----------------------------------
#include "libcoord.h"

#include <sys/types.h>  /* Special types */
#include <sys/socket.h> /* For connection */
#include <netdb.h>      /* Network address and service translation */
#include <string.h>     /* Memset */
#include <stdlib.h>     /* Standard funcs */

// ---------------------------------- Prototypes --------------------------------
// Additional funcs
int connect_to_coordinator();

int init_modules();
// ---------------------------------- Main funcs --------------------------------
/*
 * Initialize new module
 * Notify Coordinator about self-existence
 * Initialize modules' data storage
 */
int
init_module(char *name, char *addr, char *port) {
    connect_to_coordinator(char *name, char *addr, char *port);

    init_modules();
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
/*
 * Send self module's name to coordinator (by default address)
 * Wait for connection to coordinator
 * If connection established, return 0 (success)
 * If no, return -1 (failure)
 */
int
connect_to_coordinator(char *name, char *addr, char *port) {
    struct addrinfo hints;   /* connection information */
    struct addrinfo *result; /* possible addresses' info for connection */
    struct addrinfo *rp;     /* current possible address'es info */

    int status; /* status code of getaddrinfo */
    int sfd;    /* socket's fd */

    memset(&hints, 0, sizeof(struct addrinfo)); /* empty structure */

    hints.ai_family   = AF_UNSPEC;   /* IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP segment */ 
    hints.ai_protocol = 0;           /* Any protocol (TCP)*/

    // Get possible adresses for connection
    status = getaddrinfo(name, port, &hints, &result);
    if (status != 0) {
       fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
       return -1;
    }

    // Try each possible address until connection is succesful
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        // Open socket with current possible address'es info
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        // If failed open socket, then use next possible address'es
        if (sfd == -1) {
            continue;
        }

        // Try to connect
        // If success, break cycle
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
           break; 
        }
        // If not, close socket's fd

        close(sfd);
    }

    // If no address succeeded, print error message
    if (rp == NULL) {
       fprintf(stderr, "Could not connect\n");
       return -1;
    }

    freeaddrinfo(result);

    // Send to Coordinator "IEXIST (module's name)"
    char msg[263];
    char ans[26];
    
    fprintf(msg, "IEXIST (%s)", name);
    if (send(sfd, msg, strlen(msg)) == -1) {
        perror("Error while sending \"IEXIST\" to Coordinator");
        close(sfd);
        return -1;
    }
    
    // Read answer
    read(sfd, ans, 26); 

    // If LOL, then everything is alright
    if (strncmp(ans, "LOL", 3) != 0) { 
    // There was some error (maybe module with that name exist, idk)
        close(sfd);
        return -1;
    }
    // TODO: Get fds for communicating with Coordinator and update value in modules' data storage
    close(sfd);
}

/*
 * Initialize modules' storage
 */
int
init_modules_storage() {
}
