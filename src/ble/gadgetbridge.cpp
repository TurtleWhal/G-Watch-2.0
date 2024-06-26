#include "config.h"
#include "gadgetbridge.h"
#include "ble/blectl.h"
#include "ble/callback.h"
#include "ble/utils/charbuffer.h"
#include "ble/utils/alloc.h"
#include "ble/utils/bluejsonrequest.h"
#include "Arduino.h"
#include "NimBLEDescriptor.h"

#include "ble.hpp"

QueueHandle_t gadgetbridge_msg_transmit_queue; /** @brief gadgetbridge transmit message queue */
QueueHandle_t gadgetbridge_msg_receive_queue;  /** @brief gadgetbridge receive message queue */

/**
 * local buffer and callback table
 */
static blectl_msg_t gadgetbridge_msg;            /** @brief gadgetbridge chunk message buffer */
static callback_t *gadgetbridge_callback = NULL; /** @brief gadgetbridge callback structure */
static CharBuffer gadgetbridge_RX_msg;           /** @brief RX msg buffer */
/**
 * local function declaration
 */
static void gadgetbridge_send_next_msg(char *msg);
static bool gadgetbridge_send_event_cb(EventBits_t event, void *arg);
static bool gadgetbridge_powermgm_loop_cb(EventBits_t event, void *arg);
static bool gadgetbridge_blectl_event_cb(EventBits_t event, void *arg);

/**
 * @brief Characteristic callback class für gadgetbridge
 */
NimBLECharacteristic *pGadgetbridgeTXCharacteristic = NULL; /** @brief TX Characteristic */
NimBLECharacteristic *pGadgetbridgeRXCharacteristic = NULL; /** @brief RX Characteristic */
class CharacteristicCallbacks : public NimBLECharacteristicCallbacks
{

    void onRead(NimBLECharacteristic *pCharacteristic)
    {
        std::string msg = pCharacteristic->getValue();
        log_d("BLE received: %s, %i\n", msg.c_str(), msg.length());
    };

    void onWrite(NimBLECharacteristic *pCharacteristic)
    {
        size_t msgLen = pCharacteristic->getValue().length();
        // const char *msg = pCharacteristic->getValue().c_str();
        String smsg = pCharacteristic->getValue().c_str();
        // const uint8_t *data = pCharacteristic->getValue().data();

        // for (int i = 0; i < msgLen; i++)
        // {
        //     // Serial.printf("%02X ", data[i]);
        //     // Serial.printf("%c", data[i]);
        //     Serial.printf("%c", msg[i]);
        // }
        // Serial.println(msg);
        // Serial.println(smsg);

        for (int i = 0; i < msgLen; i++)
        {
            switch (smsg.charAt(i))
            {
            case EndofText:
                gadgetbridge_RX_msg.clear();
                gadgetbridge_send_event_cb(GADGETBRIDGE_CONNECT, (void *)"connected");
                break;
            case DataLinkEscape:
                gadgetbridge_RX_msg.clear();
                break;
            case LineFeed:
            {
                size_t size = strlen(gadgetbridge_RX_msg.c_str()) + 1;
                /*
                 * Duplicate message
                 */
                char *buff = (char *)CALLOC_ASSERT(size, 1, "buff calloc failed");
                strlcpy(buff, gadgetbridge_RX_msg.c_str(), size);
                /*
                 * Send message
                 */
                // Serial.println();
                parseBLE(buff);
                // Serial.println(buff);
                // if (xQueueSendFromISR(gadgetbridge_msg_receive_queue, &buff, 0) != pdTRUE)
                //    log_e("fail to send a receive BLE msg (%d bytes)", size);
                gadgetbridge_RX_msg.clear();
                break;
            }
            default:
                gadgetbridge_RX_msg.append(smsg.charAt(i));
                break;
            }
        }
    };
};

static CharacteristicCallbacks GadgetbridgeCallbacks;

void gadgetbridge_setup(void)
{
    /**
     * init gadgetbridge chunk buffer
     */
    gadgetbridge_msg.active = false;
    gadgetbridge_msg.msg = NULL;
    gadgetbridge_msg.msglen = 0;
    gadgetbridge_msg.msgpos = 0;

    /**
     * allocate send and receive queue
     */
    gadgetbridge_msg_transmit_queue = xQueueCreate(5, sizeof(char *));
    ASSERT(gadgetbridge_msg_transmit_queue, "Failed to allocate msg queue");
    gadgetbridge_msg_receive_queue = xQueueCreate(5, sizeof(char *));
    ASSERT(gadgetbridge_msg_receive_queue, "Failed to allocate msg receive queue");
    /**
     * Create the BLE Service
     */
    NimBLEServer *pServer = blectl_get_ble_server();
    NimBLEAdvertising *pAdvertising = blectl_get_ble_advertising();
    NimBLEService *pGadgetbridgeService = pServer->createService(NimBLEUUID(GADGETBRIDGE_SERVICE_UUID));
    /**
     * Create Gadgetbridge TX/RX Characteristic
     */
    pGadgetbridgeTXCharacteristic = pGadgetbridgeService->createCharacteristic(NimBLEUUID(GADGETBRIDGE_CHARACTERISTIC_UUID_TX), NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ);
    pGadgetbridgeTXCharacteristic->addDescriptor(new NimBLE2904());
    pGadgetbridgeRXCharacteristic = pGadgetbridgeService->createCharacteristic(NimBLEUUID(GADGETBRIDGE_CHARACTERISTIC_UUID_RX), NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ);
    pGadgetbridgeRXCharacteristic->setCallbacks(&GadgetbridgeCallbacks);
    pGadgetbridgeService->start();
    /**
     * add service to advertising
     */
    pAdvertising->addServiceUUID(pGadgetbridgeService->getUUID());

    /**
     * init callback for powermgm and bluetooth
     */
    // powermgm_register_loop_cb( POWERMGM_SILENCE_WAKEUP | POWERMGM_STANDBY | POWERMGM_WAKEUP, gadgetbridge_powermgm_loop_cb, "powermgm blectl loop" );
    blectl_register_cb(BLECTL_DISCONNECT, gadgetbridge_blectl_event_cb, "blectl gadgetbridge event cb");
    return;
}

static bool gadgetbridge_blectl_event_cb(EventBits_t event, void *arg)
{
    switch (event)
    {
    case BLECTL_DISCONNECT:
        gadgetbridge_send_event_cb(GADGETBRIDGE_DISCONNECT, (void *)"disconnected");
        break;
    }
    return (true);
}

bool gadgetbridge_register_cb(EventBits_t event, CALLBACK_FUNC callback_func, const char *id)
{
    if (gadgetbridge_callback == NULL)
    {
        gadgetbridge_callback = callback_init("blectl");
        ASSERT(gadgetbridge_callback, "blectl callback alloc failed");
    }
    return (callback_register(gadgetbridge_callback, event, callback_func, id));
}

static bool gadgetbridge_send_event_cb(EventBits_t event, void *arg)
{
    return (callback_send(gadgetbridge_callback, event, arg));
}

bool gadgetbridge_send_loop_msg(const char *format, ...)
{
    bool retval = false;
    /**
     * build new string
     */
    va_list args;
    va_start(args, format);
    char *buffer = NULL;
    vasprintf(&buffer, format, args);
    va_end(args);
    /**
     * if we have a string, send it via via call back
     */
    if (buffer)
    {
        BluetoothJsonRequest request(buffer, strlen(buffer) * 4);
        /**
         * check if we have a valid json
         */
        if (request.isValid())
            gadgetbridge_send_event_cb(GADGETBRIDGE_JSON_MSG, (void *)&request);
        else
            gadgetbridge_send_event_cb(GADGETBRIDGE_MSG, (void *)buffer);

        request.clear();

        retval = true;
        free(buffer);
    }

    return retval;
}

bool gadgetbridge_send_msg(const char *format, ...)
{
    bool retval = false;

    /**
     * check if we connected
     */
    if (blectl_get_event(BLECTL_CONNECT | BLECTL_AUTHWAIT))
    {
        /**
         * build new string
         */
        va_list args;
        va_start(args, format);
        char *buffer = NULL;
        vasprintf(&buffer, format, args);
        va_end(args);
        /**
         * if we have a string, send it via msg_queue
         */
        if (buffer)
        {
            if (xQueueSend(gadgetbridge_msg_transmit_queue, &buffer, 0) != pdTRUE)
                log_e("fail to send msg");
            else
                retval = true;
        }
    }
    else
    {
        log_e("msg can't send while bluetooth is not connected");
    }

    return retval;
}

/**
 * @brief put a message into gadgetbridge chunk buffer
 *
 * @param msg   pointer to a msg
 */
static void gadgetbridge_send_next_msg(char *msg)
{
    if (!gadgetbridge_msg.active && blectl_get_event(BLECTL_CONNECT))
    {

        size_t size = strlen((const char *)msg) + 1;

        if (gadgetbridge_msg.msg == NULL)
            gadgetbridge_msg.msg = (char *)CALLOC_ASSERT(size, 1, "blectl_msg.msg calloc failed");

        strncpy(gadgetbridge_msg.msg, msg, size);
        gadgetbridge_msg.active = true;
        gadgetbridge_msg.msglen = size;
        gadgetbridge_msg.msgpos = 0;
    }
    else
    {
        log_e("blectl is send another msg or not connected");
        gadgetbridge_send_event_cb(GADGETBRIDGE_MSG_SEND_ABORT, (char *)"msg send abort, blectl is send another msg or not connected");
        return;
    }
}

/**
 * @brief send a msg chunk to gadgetbridge over ble
 *
 * @param msg       pointer to a msg
 * @param len       chunk size (normaly 20bytes)
 */
void gadgetbridge_send_chunk(unsigned char *msg, int32_t len)
{
    /*
     * send msg chunk
     */
    pGadgetbridgeTXCharacteristic->setValue(msg, len);
    pGadgetbridgeTXCharacteristic->notify();
}

/**
 * @brief powermgm loop event callback funktion, handle gadgetbridge chunks and recieved msg
 *
 * @param event             event type
 * @param arg               event arg
 * @return true
 * @return false
 */
static bool gadgetbridge_powermgm_loop_cb(EventBits_t event, void *arg)
{
    static uint64_t nextmillis = 0;
    /**
     * check if we connected
     */
    if (!blectl_get_event(BLECTL_CONNECT))
        return (true);
    /**
     * work on send queue
     */
    /**
     * copy next msg from the queue
     */
    if (!gadgetbridge_msg.active)
    {
        char *msg;
        BaseType_t available = xQueueReceive(gadgetbridge_msg_transmit_queue, &msg, 0);
        if (available == pdTRUE && msg)
        {
            gadgetbridge_send_next_msg(msg);
            free(msg);
        }
    }

    /**
     * send blectl_msg chunk when we have a active msg in it
     */
    if (gadgetbridge_msg.active && nextmillis < millis())
    {
        bool finish = true;
        nextmillis = millis() + BLECTL_CHUNKDELAY;

        if (gadgetbridge_msg.msgpos < gadgetbridge_msg.msglen)
        {
            if ((gadgetbridge_msg.msglen - gadgetbridge_msg.msgpos) > BLECTL_CHUNKSIZE)
            {
                gadgetbridge_send_chunk((unsigned char *)&gadgetbridge_msg.msg[gadgetbridge_msg.msgpos], BLECTL_CHUNKSIZE);
                gadgetbridge_msg.msgpos += BLECTL_CHUNKSIZE;
                finish = false;
            }
            else if ((gadgetbridge_msg.msglen - gadgetbridge_msg.msgpos) > 0)
            {
                gadgetbridge_send_chunk((unsigned char *)&gadgetbridge_msg.msg[gadgetbridge_msg.msgpos], gadgetbridge_msg.msglen - gadgetbridge_msg.msgpos);
                gadgetbridge_send_event_cb(GADGETBRIDGE_MSG_SEND_SUCCESS, (char *)"msg send success");
            }
            else
            {
                gadgetbridge_send_event_cb(GADGETBRIDGE_MSG_SEND_ABORT, (char *)"msg send abort, malformed chunksize");
            }
        }

        if (finish)
        {
            gadgetbridge_msg.active = false;
            gadgetbridge_msg.msglen = 0;
            gadgetbridge_msg.msgpos = 0;
        }
    }
    /**
     * work on recieve queue
     */
    /**
     * get next msg from receive queue
     */
    char *gbmsg;
    BaseType_t available = xQueueReceive(gadgetbridge_msg_receive_queue, &gbmsg, 0);
    if (available == pdTRUE && gbmsg)
    {
        /**
         * check if we have a GB message
         */
        if (gbmsg[0] == 'G' && gbmsg[1] == 'B')
        {
            /**
             * copy gbmsg pointer to a new pointer to prevent destroying gbmsg pointer
             */
            char *GBmsg = gbmsg + 3;
            GBmsg[strlen(GBmsg) - 1] = '\0';

            BluetoothJsonRequest request(GBmsg, strlen(GBmsg) * 4);

            if (request.isValid())
                gadgetbridge_send_event_cb(GADGETBRIDGE_JSON_MSG, (void *)&request);
            else
                gadgetbridge_send_event_cb(GADGETBRIDGE_MSG, (void *)GBmsg);

            request.clear();
        }
        else
            gadgetbridge_send_event_cb(GADGETBRIDGE_MSG, (void *)gbmsg);

        free(gbmsg);
    }

    return (true);
}