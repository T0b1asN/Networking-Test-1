# Network Protocol

## Server & Client

### Sending Message

`MSG [Message]`

### Sending RSA Key

`RSA [N] [e]` Send components of public key. __This Message should not be encrypted!

### Sending General Error

`ERROR [Message]` Send an error-message for a general error__

## Server

### Sending Message (No Sound)

`MSGNS [Message]` Send a message to the Client with no Sound

### Send Name Response

`NAME_RESP [Result]` Send response to name (_Response=1_ if name already exists on server, otherwise _Response=0_)

### Sending Shutdown

`SHUTDOWN [Message(optional)]` Notify clients, that the server is about to shut down

### Sending Disconnect Message

`MSGDC [Message]`

### Error

`WRONG_CMD` Expecting different command

`RSA_MISSING` Message should be Key but is not, try resending the key

`NAME_MISSING` Message should be name but is not, try resending the name

## Client

### Send Name

`NAME [Name]` Send the name to the server
