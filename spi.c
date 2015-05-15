// gcc -o spi spi.c -l bcm2835
// sudo ./spi

#include <bcm2835.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc !=2 ) {
	printf("error: usage: %s <T1/T2/T3/T4/P1/H1/C2/C4>\n", argv[0]);
    }

// Use for testing
//       bcm2835_set_debug(1);
      if (!bcm2835_init())
        return 1;
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
   
//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);
bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256); 
   bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    
    uint8_t send_data1, send_data2;
    
    send_data2 =  argv[1][1];
    switch (argv[1][0]) {
    	case 'T': send_data1 = 0x54;
		  break;
	case 'P': send_data1 = 0x50;
		  break;
	case 'H': send_data1 = 0x48;
		  break;
	case 'C': send_data1 = 0x43;
		  break;
	default:  send_data1 = 0x5C; 
  		  send_data2 = 0x72; 
   		  break; 
    }

uint8_t send_data, read_data;

read_data = bcm2835_spi_transfer(send_data1);
read_data = bcm2835_spi_transfer(send_data2);
read_data = bcm2835_spi_transfer(0);

while(bcm2835_gpio_lev(RPI_GPIO_P1_13)==HIGH);
bcm2835_delay(1);

//send_data =0x0A;
printf("Read back from sensor: %s:\n", argv[1]);
do {
    bcm2835_delay(3);
     read_data = bcm2835_spi_transfer(0);
//     read_data = bcm2835_spi_transfer(send_data);    
    if(read_data==0x0D) printf("\n");
//    else if(read_data!=0x0A) 
      printf("%c",read_data);
} while ( read_data!=0x0D);

	bcm2835_spi_end();
	bcm2835_close();
    return 0;
}
