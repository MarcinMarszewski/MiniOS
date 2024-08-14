#include "harddrive.h"
#include "port.h"

void ata_wait_bsy(unsigned short io) {
    while (port_byte_in(io + ATA_REG_STATUS) & ATA_SR_BSY);
}

void ata_wait_drq(unsigned short io) {
    while (!(port_byte_in(io + ATA_REG_STATUS) & ATA_SR_DRQ));
}

void ata_identify(unsigned short io) {
    port_byte_out(io + ATA_REG_HDDEVSEL, 0xA0);
    port_byte_out(io + ATA_REG_SECCOUNT0, 0);
    port_byte_out(io + ATA_REG_LBA0, 0);
    port_byte_out(io + ATA_REG_LBA1, 0);
    port_byte_out(io + ATA_REG_LBA2, 0);
    port_byte_out(io + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

    ata_wait_bsy(io);

    unsigned short status = port_byte_in(io + ATA_REG_STATUS);
    if (status == 0) {
        // No device
        return;
    }

    ata_wait_drq(io);

    unsigned short buffer[256];
    for (int i = 0; i < 256; i++) {
        buffer[i] = port_word_in(io + ATA_REG_DATA);
    }

    // Process the IDENTIFY data in buffer
}

void ata_read_sector(unsigned short io, unsigned int lba, unsigned char *buffer) {
    ata_wait_bsy(io);
    port_byte_out(io + ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0xF));
    port_byte_out(io + ATA_REG_SECCOUNT0, 1);
    port_byte_out(io + ATA_REG_LBA0, (unsigned char) lba);
    port_byte_out(io + ATA_REG_LBA1, (unsigned char)(lba >> 8));
    port_byte_out(io + ATA_REG_LBA2, (unsigned char)(lba >> 16));
    port_byte_out(io + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    ata_wait_bsy(io);
    ata_wait_drq(io);

    for (int i = 0; i < 256; i++) {
        ((unsigned short *)buffer)[i] = port_word_in(io + ATA_REG_DATA);
    }
}

void ata_write_sector(unsigned short io, unsigned int lba, unsigned char *buffer) {
    ata_wait_bsy(io);
    port_byte_out(io + ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0xF));
    port_byte_out(io + ATA_REG_SECCOUNT0, 1);
    port_byte_out(io + ATA_REG_LBA0, (unsigned char) lba);
    port_byte_out(io + ATA_REG_LBA1, (unsigned char)(lba >> 8));
    port_byte_out(io + ATA_REG_LBA2, (unsigned char)(lba >> 16));
    port_byte_out(io + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

    ata_wait_bsy(io);

    for (int i = 0; i < 256; i++) {
        port_word_out(io + ATA_REG_DATA, ((unsigned short *)buffer)[i]);
    }

    ata_wait_bsy(io);
}