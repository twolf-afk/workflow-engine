#include "opcua.h"

#include <open62541/client.h>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>

#include <stdio.h>

void opcua::placeholderFunction()
{

    /* Create a client and connect */
    UA_Client* client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    UA_StatusCode status = UA_Client_connect(client, "opc.tcp://DESKTOP-0AULV4D:4840/");
    if (status != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
    }

    /* Read the value attribute of the node. UA_Client_readValueAttribute is a
     * wrapper for the raw read service available as UA_Client_Service_read. */
    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);
    UA_String myString;
    UA_String_init(&myString);
    char string[] = "tobi";
    myString.length = strlen(string);
    myString.data = (UA_Byte*)string;

    UA_Variant_setScalar(&value, &myString, &UA_TYPES[UA_TYPES_STRING]);

    char identifier[] = "the answer";
    status = UA_Client_writeValueAttribute(client, UA_NODEID_STRING(1, identifier), &value);

    if (status == UA_STATUSCODE_GOOD)
    {
        printf("success\n");
    }

    /* Clean up */
    // UA_Variant_clear(&value);
    UA_Client_delete(client); /* Disconnects the client internally */

}
