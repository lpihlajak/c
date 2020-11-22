/* 
Implement (using c standard library only) a codec that serializes and deserializes instances of this struct:
struct {
       struct {
           char sync;
           unsigned int crc;
       } header;
       char payload[20];
}
The serialized form of the object should be like this:
<sync><crc><payload>
For example, the struct instance
{ 'S', 123, "geek of the week" }
in serialized form looks like this:
<S><123><geek of the week>
*/
 
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define HEADER_SIZE 5
#define PAYLOAD_SIZE 20
#define SYNC_CHAR 'S'

struct Message {

    struct {
        char sync;
        uint32_t crc;
    } header;

    char payload[PAYLOAD_SIZE];

};

struct Message msg;

void serialize(struct Message *msg, char* buf);
bool deserialize(struct Message *msg, char* buf);
bool validateMsg(struct Message *msg);

void main(void)
{
    char buf[128];

    msg.header.sync = SYNC_CHAR;
    msg.header.crc = 123; // CRC Calculation not specified to be implemented
    memcpy(&msg.payload, "Geek of the Week", 16);

    printf("Serializing data..");
    serialize(&msg, buf);

    printf("\r\nDeserializing data..");
    if(deserialize(&msg, buf) == true)
    {
        printf("\r\nCodec OK");
    }
    else
    {
        printf("\r\nCodec FAIL");
    }

}

void serialize(struct Message *msg, char* buf)
{
    memcpy(buf, &msg->header, HEADER_SIZE);
    memcpy(buf+HEADER_SIZE, msg->payload, PAYLOAD_SIZE);
    return;
}

bool deserialize(struct Message *msg, char* buf)
{
    uint8_t i;
    bool found = false;

    for(i = 0; i < PAYLOAD_SIZE-1; i++)
    {
        if(buf[i] == SYNC_CHAR)
        {
            found = true;
            break;
        }
    }

    if(found == true)
    {
        memcpy(&msg->header, buf+i, HEADER_SIZE);
        memcpy(msg->payload, (buf+i)+HEADER_SIZE, PAYLOAD_SIZE);
        return validateMsg(msg);
    }
    else
    {
        return false;
    }

}

bool validateMsg(struct Message *msg)
{
   // Implement CRC Calc here
   return true; 
}

