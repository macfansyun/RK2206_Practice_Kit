/*
 * Copyright (c) 2024 iSoftStone Education Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
// Mifare RC522 RFID Card reader 13.56 MHz

#include "iot_spi.h"
#include "rc522.h"

static IoT_SPI_InitTypeDef g_spi_cfg =
    {
        .Mode              = SPI_MODE_MASTER,
        .Direction         = SPI_DIRECTION_2LINES,
        .DataSize          = SPI_DATASIZE_8BIT,
        .CLKPolarity       = SPI_POLARITY_LOW,
        .CLKPhase          = SPI_PHASE_1EDGE,
        .BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32,
        .FirstBit          = SPI_FIRSTBIT_MSB,
};

#define SPI_IO_SELECT ESPI0_M1
mfrc522_dev_t g_rc552_dev =
    {
        .spi_dev = {.io_select = SPI_IO_SELECT, .cfg = &g_spi_cfg},
};

static void mfrc522_spi_init(enum EnumSpiId id)
{
    spi_dev_t *spi_dev = &g_rc552_dev.spi_dev;

    spi_dev->io_select = id;
    IoTSpiInit(id, spi_dev->cfg);
}

static void mfrc522_spi_deinit(enum EnumSpiId id)
{
    spi_dev_t *spi_dev = &g_rc552_dev.spi_dev;

    spi_dev->io_select = id;
    IoTSpiDeinit(id);
}

void spi1_write_reg(uint8_t address, uint8_t value)
{
    spi_dev_t *spi_dev = &g_rc552_dev.spi_dev;
    unsigned char writeCommand[2];

    writeCommand[0] = address;
    writeCommand[1] = value;

    return IoTSpiWrite(spi_dev->io_select, writeCommand, sizeof(writeCommand));
}

uint8_t spi1_read_reg(uint8_t address)
{
    spi_dev_t *spi_dev = &g_rc552_dev.spi_dev;
    unsigned char writeCommand[1];
    unsigned char readValue[1];

    writeCommand[0] = address;

    IoTSpiWriteRead(spi_dev->io_select, writeCommand, sizeof(writeCommand), readValue, sizeof(readValue));
    return readValue[0];
}

void mfrc522_write_register(uint8_t addr, uint8_t val)
{
    addr = (addr << 1) & 0x7E; // Address format: 0XXXXXX0
    spi1_write_reg(addr, val);
}

uint8_t mfrc522_read_register(uint8_t addr)
{
    uint8_t val;

    addr = ((addr << 1) & 0x7E) | 0x80;
    val  = spi1_read_reg(addr);
    return val;
}

uint8_t mfrc522_check(uint8_t *id)
{
    uint8_t status;
    status = mfrc522_request(PICC_REQIDL, id); // Find cards, return card type
    if (status == MI_OK) {
        status = mfrc522_anticoll(id); // Card detected. Anti-collision, return card serial number 4 bytes
    }
    mfrc522_halt(); // Command card into hibernation
    return status;
}

uint8_t mfrc522_compare(uint8_t *CardID, uint8_t *CompareID)
{
    uint8_t i;
    for (i = 0; i < 5; i++) {
        if (CardID[i] != CompareID[i]) {
            return MI_ERR;
        }
    }
    return MI_OK;
}

void mfrc522_set_bitmask(uint8_t reg, uint8_t mask)
{
    mfrc522_write_register(reg, mfrc522_read_register(reg) | mask);
}

void mfrc522_clear_bitmask(uint8_t reg, uint8_t mask)
{
    mfrc522_write_register(reg, mfrc522_read_register(reg) & (~mask));
}

uint8_t mfrc522_request(uint8_t reqMode, uint8_t *TagType)
{
    uint8_t status;
    uint16_t backBits; // The received data bits

    mfrc522_write_register(MFRC522_REG_BIT_FRAMING, 0x07); // TxLastBists = BitFramingReg[2..0]
    TagType[0] = reqMode;
    status     = mfrc522_to_card(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);
    if ((status != MI_OK) || (backBits != 0x10)) {
        status = MI_ERR;
    }
    return status;
}

uint8_t mfrc522_to_card(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint16_t *backLen)
{
    uint8_t status  = MI_ERR;
    uint8_t irqEn   = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint16_t i;

    switch (command) {
        case PCD_AUTHENT: {
            irqEn   = 0x12;
            waitIRq = 0x10;
            break;
        }
        case PCD_TRANSCEIVE: {
            irqEn   = 0x77;
            waitIRq = 0x30;
            break;
        }
        default:
            break;
    }

    mfrc522_write_register(MFRC522_REG_COMM_IE_N, irqEn | 0x80);
    mfrc522_clear_bitmask(MFRC522_REG_COMM_IRQ, 0x80);
    mfrc522_set_bitmask(MFRC522_REG_FIFO_LEVEL, 0x80);
    mfrc522_write_register(MFRC522_REG_COMMAND, PCD_IDLE);

    // Writing data to the FIFO
    for (i = 0; i < sendLen; i++) {
        mfrc522_write_register(MFRC522_REG_FIFO_DATA, sendData[i]);
    }

    // Execute the command
    mfrc522_write_register(MFRC522_REG_COMMAND, command);
    if (command == PCD_TRANSCEIVE) {
        mfrc522_set_bitmask(MFRC522_REG_BIT_FRAMING, 0x80); // StartSend=1,transmission of data starts
    }

    // Waiting to receive data to complete
    i = 2000; // i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms
    do {
        // CommIrqReg[7..0]
        // Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = mfrc522_read_register(MFRC522_REG_COMM_IRQ);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

    mfrc522_clear_bitmask(MFRC522_REG_BIT_FRAMING, 0x80); // StartSend=0

    if (i != 0) {
        if (!(mfrc522_read_register(MFRC522_REG_ERROR) & 0x1B)) {
            status = MI_OK;
            if (n & irqEn & 0x01) {
                status = MI_NOTAGERR;
            }
            if (command == PCD_TRANSCEIVE) {
                n        = mfrc522_read_register(MFRC522_REG_FIFO_LEVEL);
                lastBits = mfrc522_read_register(MFRC522_REG_CONTROL) & 0x07;
                if (lastBits) {
                    *backLen = (n - 1) * 8 + lastBits;
                } else {
                    *backLen = n * 8;
                }
                if (n == 0) {
                    n = 1;
                }
                if (n > MFRC522_MAX_LEN) {
                    n = MFRC522_MAX_LEN;
                }
                for (i = 0; i < n; i++) {
                    backData[i] = mfrc522_read_register(MFRC522_REG_FIFO_DATA); // Reading the received data in FIFO
                }
            }
        } else {
            status = MI_ERR;
        }
    }
    return status;
}

uint8_t mfrc522_anticoll(uint8_t *serNum)
{
    uint8_t status;
    uint8_t i;
    uint8_t serNumCheck = 0;
    uint16_t unLen;

    mfrc522_write_register(MFRC522_REG_BIT_FRAMING, 0x00); // TxLastBists = BitFramingReg[2..0]
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status    = mfrc522_to_card(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);
    if (status == MI_OK) {
        // Check card serial number
        for (i = 0; i < 4; i++) {
            serNumCheck ^= serNum[i];
        }
        if (serNumCheck != serNum[i]) {
            status = MI_ERR;
        }
    }
    return status;
}

void mfrc522_CalculateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData)
{
    uint8_t i, n;

    mfrc522_clear_bitmask(MFRC522_REG_DIV_IRQ, 0x04);  // CRCIrq = 0
    mfrc522_set_bitmask(MFRC522_REG_FIFO_LEVEL, 0x80); // Clear the FIFO pointer
    // Write_MFRC522(CommandReg, PCD_IDLE);

    // Writing data to the FIFO
    for (i = 0; i < len; i++) {
        mfrc522_write_register(MFRC522_REG_FIFO_DATA, *(pIndata + i));
    }
    mfrc522_write_register(MFRC522_REG_COMMAND, PCD_CALCCRC);

    // Wait CRC calculation is complete
    i = 0xFF;
    do {
        n = mfrc522_read_register(MFRC522_REG_DIV_IRQ);
        i--;
    } while ((i != 0) && !(n & 0x04)); // CRCIrq = 1

    // Read CRC calculation result
    pOutData[0] = mfrc522_read_register(MFRC522_REG_CRC_RESULT_L);
    pOutData[1] = mfrc522_read_register(MFRC522_REG_CRC_RESULT_M);
}

uint8_t mfrc522_select_tag(uint8_t *serNum)
{
    uint8_t i;
    uint8_t status;
    uint8_t size;
    uint16_t recvBits;
    uint8_t buffer[9];

    buffer[0] = PICC_SElECTTAG;
    buffer[1] = 0x70;
    for (i = 0; i < 5; i++) {
        buffer[i + 2] = *(serNum + i);
    }
    mfrc522_CalculateCRC(buffer, 7, &buffer[7]);
    status = mfrc522_to_card(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
    if ((status == MI_OK) && (recvBits == 0x18)) {
        size = buffer[0];
    } else {
        size = 0;
    }
    return size;
}

uint8_t mfrc522_auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *serNum)
{
    uint8_t status;
    uint16_t recvBits;
    uint8_t i;
    uint8_t buff[12];

    // Verify the command block address + sector + password + card serial number
    buff[0] = authMode;
    buff[1] = BlockAddr;
    for (i = 0; i < 6; i++) {
        buff[i + 2] = *(Sectorkey + i);
    }
    for (i = 0; i < 4; i++) {
        buff[i + 8] = *(serNum + i);
    }
    status = mfrc522_to_card(PCD_AUTHENT, buff, 12, buff, &recvBits);
    if ((status != MI_OK) || (!(mfrc522_read_register(MFRC522_REG_STATUS2) & 0x08))) {
        status = MI_ERR;
    }
    return status;
}

uint8_t mfrc522_read(uint8_t blockAddr, uint8_t *recvData)
{
    uint8_t status;
    uint16_t unLen;

    recvData[0] = PICC_READ;
    recvData[1] = blockAddr;
    mfrc522_CalculateCRC(recvData, 2, &recvData[2]);
    status = mfrc522_to_card(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);
    if ((status != MI_OK) || (unLen != 0x90)) {
        status = MI_ERR;
    }
    return status;
}

uint8_t mfrc522_write(uint8_t blockAddr, uint8_t *writeData)
{
    uint8_t status;
    uint16_t recvBits;
    uint8_t i;
    uint8_t buff[18];

    buff[0] = PICC_WRITE;
    buff[1] = blockAddr;
    mfrc522_CalculateCRC(buff, 2, &buff[2]);
    status = mfrc522_to_card(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);
    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) {
        status = MI_ERR;
    }
    if (status == MI_OK) {
        // Data to the FIFO write 16Byte
        for (i = 0; i < 16; i++) {
            buff[i] = *(writeData + i);
        }
        mfrc522_CalculateCRC(buff, 16, &buff[16]);
        status = mfrc522_to_card(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
        if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) {
            status = MI_ERR;
        }
    }
    return status;
}

void mfrc522_init(void)
{
    mfrc522_spi_init(SPI_IO_SELECT);
    LOS_Msleep(500);
    mfrc522_reset();
    mfrc522_write_register(MFRC522_REG_T_MODE, 0x8D);
    mfrc522_write_register(MFRC522_REG_T_PRESCALER, 0x3E);
    mfrc522_write_register(MFRC522_REG_T_RELOAD_L, 30);
    mfrc522_write_register(MFRC522_REG_T_RELOAD_H, 0);
    mfrc522_write_register(MFRC522_REG_RF_CFG, 0x7F);       // 48dB gain
    mfrc522_write_register(MFRC522_REG_RX_THRESHOLD, 0x84); ////////////////////////////<<<<<<<<<<<<<<<<<<<<
    mfrc522_write_register(MFRC522_REG_TX_AUTO, 0x40);
    mfrc522_write_register(MFRC522_REG_MODE, 0x3D);
    mfrc522_antenna_on(); // Open the antenna
}

void mfrc522_deinit(void)
{
    mfrc522_spi_deinit(SPI_IO_SELECT);
}

void mfrc522_reset(void)
{
    mfrc522_write_register(MFRC522_REG_COMMAND, PCD_RESETPHASE);
}

void mfrc522_antenna_on(void)
{
    uint8_t temp;

    temp = mfrc522_read_register(MFRC522_REG_TX_CONTROL);
    if (!(temp & 0x03)) {
        mfrc522_set_bitmask(MFRC522_REG_TX_CONTROL, 0x03);
    }
}

void mfrc522_antenna_off(void)
{
    mfrc522_clear_bitmask(MFRC522_REG_TX_CONTROL, 0x03);
}

void mfrc522_halt(void)
{
    uint16_t unLen;
    uint8_t buff[4];

    buff[0] = PICC_HALT;
    buff[1] = 0;
    mfrc522_CalculateCRC(buff, 2, &buff[2]);
    mfrc522_to_card(PCD_TRANSCEIVE, buff, 4, buff, &unLen);
}
