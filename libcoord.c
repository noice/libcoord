// ----------------------------------- Headers ----------------------------------
#include "libcoord.h"

#include <sys/types.h>  /* Special types */
#include <sys/socket.h> /* For connection */
#include <netdb.h>      /* Network address and service translation */
#include <string.h>     /* Memset */
#include <stdlib.h>     /* Standard funcs */
#include <stdio.h>      /* Formatted output */
#include <unistd.h>     /* Syscalls */

// ---------------------------------- Prototypes --------------------------------
// Additional funcs
int connect_to_coordinator();

int init_modules_storage();
// ---------------------------------- Main funcs --------------------------------
/*
 * Initialize new module
 * Notify Coordinator about self-existence
 * Initialize modules' data storage
 */
int
init_module(char *name, char *addr, char *port) {
    if (connect_to_coordinator(name, addr, port)) {
       fprintf(stderr, "Error while connecting to Coordinator\n"); 
       return -1;
    }

    init_modules_storage();

    return 0;
}

/*
 * Get module info by provided @name
 * Check modules' data storage
 * If exist, returns pointer to Module 
 * Otherwise returns NULL
 */
Module* 
get_module_by_name(char *name) {
    return NULL;
}

/*
 * Connect to module provided @mod
 * Send request for connection to Coordinator
 * If connection can be established, update requested module's information in data storage and returns 0 (success) 
 * If no, returns -1 (failure)
 */
int
connect_to_module(Module *mod) {
    return 0;
}

/*
 * Send msg to module by provided @mod
 * Message represented by @buf (actual message here) and @len (length of message)
 */
int
send_msg(Module *mod, char *buf, msg_len len) {
    return 0;
}

/*
 * Handle incoming messages
 * First use default defined handler 
 * Then use user-provded @handler
 */
int
handle(int (*handler)(Message*)) {
    return 0;
}

/*
 * Get any incoming message
 * Check if any read-ends contains message and can be readed
 * If yes, return pointer to first founded Message
 * Otherwise returns NULL
 */
Message*
get_msg() {
    return NULL;
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
       fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
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
    char msg[264]; /* message to Coordinator*/
    char ans[27];  /* response */
    
    sprintf(msg, "IEXIST (%s)\n", name);
    if (send(sfd, msg, strlen(msg), 0) == -1) {
        perror("Error while sending \"IEXIST\" to Coordinator");
        close(sfd);
        return -1;
    }
    
    // Read answer
    read(sfd, ans, 27); 

    // TODO: Perhaps in future this will be in separate function
    // If LOL, then everything is alright
    if (strncmp(ans, "LOL", 3) == 0) {
        char write_end[10]; /* store write-end to Coordinator here */
        char read_end[10];  /* store read-end to Coordinator here */
        
        int i, j = 0;

        // Copy write-end
        for (i = ans[4]; i != ' '; i++) {
            write_end[j++] = ans[i];                   
        }

        j = 0; 
        i += 1;

        // Copy read-end
        for (; i != ' '; i++) {
            read_end[j++] = ans[i];
        }
        
        // TODO: Update communication info in storage
        close(sfd);

        return 0;
    }
    // It means module with name already exist
    // Report about it and return error
    else if (strncmp(ans, "IKNOW", 5) == 0) {
        fprintf(stderr, "Module with that name already exist\n"); 
        close(sfd);

        return -1;
    }
    // Other messages also is error
    // Return -1
    else {
        close(sfd);

        return -1;
    }

}

/*
 * Initialize modules' storage
 */
int
init_modules_storage() {
    return 0;
}
