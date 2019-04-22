/* ***************************************************************************
 * GSM Routines:
 * 
 * Notes: 
 *      1) Loosely based on serialport-gsm by Zab Salahid
 *      2) Does not bother to use SIM memory (internal memory only)
 *      3) Deletes all messages upon reading a message
 *          3a) the handshake is that a server would never send two
 *              commands in a row, it would wait, at least some relatively
 *              long period of time (say 30 sec) before resending.
 * 
 * 
 * API:
 * gsmInit() // initializes modem - note to set baudrate etc, see uart init
 *           // ATZ, AT+QURCCFG, AT+CMGF, AT+CLIP
 *           // optional: AT_CGSN(IMEI), AT+CNUM (own number), ATI(mfg info)
 * 
 * gsmInfo() // returns Carrier and Signal Strength
 *           // This API is called only once by the controller SW at startup
 *           // AT+CSQ, AT+QPSN
 * 
 * gsmSendSMS (char* message, char* recipient) // recipient 1-xxx-xxx-xxxx)
 *           // invoked by controller software when required or in response to
 *           // to a Gateway command.
 *           // AT+CMGS
 * 
 * gsmReadSMS()  // automatically deletes all messages after reading
 *               // This API is invoked when a URC (unsolicited Response Code)
 *               // is received indicating a new message. 
 *               // AT+CMGRR, AT+CMGD
 * 
 * gsmRcvData() // This is a 'Private' routine which is called by UART receive 
 *              // It is responsible to parse four types of gsm modem Tx (UART 
 *              // Rx data). (1) the command, (2) the response to a sent command
 *              // (3) 'OK' | 'Error' | time-out. An  OK will terminate the
 *              // timer and return the command response information. An Error
 *              // or timeout; both of which will log an error. (4) A URC
 *              // message such as a received message.
 * 
 * GSM commands implemented:
 * AT, ATZ,  ATI, AT+QURCCFG (Quectel proprietary), AT_CMGF, AT+CLIP
 * AT+CMGS, AT+CMGR
 * AT+CMGD=,4(delete all), AT+CSQ
 */


_Bool executeCommand (char* command, char* message, char* recipient) {
    // executeCommand should be considered 'private' and used by the API
    // routines to send specific AT commands to the modem. It should not
    // be used directly. 
    //      1) Sends AT command
    //      2) sets a 10 second timer
    //      3) returns with success or failure
    // 
    // Returns success or fail (true/false)
    // how to log an error on fail?
    
}