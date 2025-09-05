// This program just demonstrates access to the AXI QSPI interface accross PCIe.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>

#include "fpga.h"

int ram_test(uint32_t* offset, uint32_t size);

int main(int argc,char** argv)
{
    char devstr[] = "/dev/xdma0_bypass";

    // get pointer to the FPGA logic.
    void* base_addr;
    int fd = open(devstr, O_RDWR|O_SYNC);
    if(fd < 0) {
        fprintf(stderr,"Can't open %s, you must be root!\n", devstr);
    } else {
        base_addr = mmap(0,FPGA_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        if(base_addr == NULL) fprintf(stderr,"Can't mmap\n");
    }
    uint32_t *reg_ptr  = base_addr + FPGA_REG_OFFSET;
    printf("FPGA_ID = 0x%08x, FPGA_VERSION = 0x%08x\n", reg_ptr[FPGA_ID], reg_ptr[FPGA_VERSION]);

    // access some AXI QSPI registers
    uint32_t * qspi_ptr = base_addr + QSPI_OFFSET;

    printf("QSPI_SRR = 0x%08x\n", qspi_ptr[QSPI_SRR/4]);
    printf("QSPI_CR = 0x%08x\n", qspi_ptr[QSPI_CR/4]);
    printf("QSPI_SR = 0x%08x\n", qspi_ptr[QSPI_SR/4]);
    printf("QSPI_DRR = 0x%08x\n", qspi_ptr[QSPI_DRR/4]);
    printf("QSPI_SSR = 0x%08x\n", qspi_ptr[QSPI_SSR/4]);
    printf("QSPI_TX_OCCUPANCY = 0x%08x\n", qspi_ptr[QSPI_TX_OCCUPANCY/4]);
    printf("QSPI_RX_OCCUPANCY = 0x%08x\n", qspi_ptr[QSPI_RX_OCCUPANCY/4]);
    printf("QSPI_DGIER = 0x%08x\n", qspi_ptr[QSPI_DGIER/4]);
    printf("QSPI_IPISR = 0x%08x\n", qspi_ptr[QSPI_IPISR/4]);
    printf("QSPI_IPEIR = 0x%08x\n", qspi_ptr[QSPI_IPEIR/4]);


    munmap(base_addr,FPGA_SIZE);

    return 0;
}

/*
#define     QSPI_SRR                0x40    // 40h SRR Write N/A Software reset register
#define     QSPI_SPICR              0x60    // 60h SPICR R/W 0x180 SPI control register
#define     QSPI_SPISR              0x64    // 64h SPISR Read 0x0a5 SPI status register
#define     QSPI_DTR                0x68    // 68h SPI DTR Write 0x0 SPI data transmit register. A single register or a FIFO
#define     QSPI_DRR                0x6C    // 6Ch SPI DRR Read N/A(1) SPI data receive register. A single register or a FIFO
#define     QSPI_SPISSR             0x70    // 70h SPISSR R/W No slave is selected 0xFFFF SPI Slave select register
#define     QSPI_TX_OCCUPANCY       0x74    // 74h SPI Transmit FIFO Occupancy Register(2) Read 0x0 Transmit FIFO occupancy register
#define     QSPI_RX_OCCUPANCY       0x78    // 78h SPI Receive FIFO Occupancy Register(2) Read 0x0 Receive FIFO occupancy register
#define     QSPI_DGIER              0x1C    // 1Ch DGIER R/W 0x0 Device global interrupt enable register
#define     QSPI_IPISR              0x20    // 20h IPISR R/TOW(3) 0x0 IP interrupt status register
#define     QSPI_IPEIR              0x28    // 28h IPIER R/W 0x0 IP interrupt enable register
*/
