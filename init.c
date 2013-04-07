#include <msp430g2553.h>

#define TXD BIT1	// TXD on P1.1
#define RXD BIT2	// RXD on P1.2

void Init_CLK( void )
{
    BCSCTL1 = CALBC1_16MHZ;     //设置COM口传输时钟
    DCOCTL = CALDCO_16MHZ;
}
void Init_Port(void)
{
	P1DIR |= BIT0+BIT6;	//LED1
	P1OUT=BIT6;
	P1IE |=  BIT3;	// P1.3 interrupt enabled
	P1IES |= BIT3;	// P1.3 Hi/lo edge
	P1REN |= BIT3;	// Enable Pull Up on SW2 (P1.3)
	P1OUT |= BIT3;
	P1IFG &= ~BIT3;	// P1.3 IFG cleared

    P1SEL |= TXD+RXD;
	P1SEL2 |= TXD+RXD;
	P1DIR |= TXD;
	P1DIR&=~RXD;
}
void Init_UART(void)
{
    UCA0CTL1 =  UCSWRST;	//置位UCSWRST位，使UART模块的寄存器处于初始状态
    UCA0CTL1 |= UCSSEL1+UCDORM;	//UCLK=SMCLK，只在地址字时触发中断
	UCA0CTL1 |= UCSSEL1;
	UCA0CTL0 |= UCMODE0;
	UCA0CTL0 &=~ UCMODE1;	//USCI模式选择空闲线多机模式
    UCA0BR0  = 0x82;
    UCA0BR1  = 0x06;  //UCBRx=1666,UCBRFx=0
    UCA0MCTL = UCBRS2+UCBRS1;	//UCBRSx=6,Baud Rate=9600
    UCA0CTL1 &= ~UCSWRST;
	IE2|=UCA0RXIE;
}
void Init_LaunchPAD(void)	//初始化MCU
{
    WDTCTL = WDTPW + WDTHOLD;                 // 关看门狗
    Init_CLK();                               // 初始化时钟
    Init_Port();                              // 初始化端口
    //Init_Timer();                             // 初始化相关定时器
    Init_UART();  //初始化UART
}
