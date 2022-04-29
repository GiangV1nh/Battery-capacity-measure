
// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

#include <p18F4550.h>
#include <xlcd_portd.h>
#include <delays.h>
#include <math.h>
#include <stdio.h>

typedef enum {
    false, true
} bool;
#define BUTTON PORTBbits.RB3
#define BUTTONPIN TRISBbits.TRISB3
#define DISCHARGELED PORTEbits.RE0
#define DISCHARGELEDPIN TRISEbits.TRISE0
#define ENCODERAPIN TRISBbits.TRISB5
#define ENCODERBPIN TRISBbits.TRISB4
#define ENCODERINTIF INTCONbits.RBIF
#define PWMVALLOADDR 0x00
#define PWMVALHIADDR 0x01
#define CURRENTVALADDR 0x02
#define VOLVALLOADDR 0x03
#define VOLVALHIADDR 0x04
#define R_START 0x0  
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6
//0b00000100
#define DIR_CW 0x10
#define DIR_CCW 0x20
// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = INTOSC_HS // Oscillator Selection bits (Internal oscillator, HS oscillator used by USB (INTHS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)
unsigned int SetPWM = 0;
unsigned char SetCurrent = 0;
unsigned int SetVoltage = 0;
unsigned char State = 0;
unsigned long Second = 0;
const unsigned char ttable[7][4] = {
    // R_START
    {R_START, R_CW_BEGIN, R_CCW_BEGIN, R_START},
    // R_CW_FINAL
    {R_CW_NEXT, R_START, R_CW_FINAL, R_START | DIR_CW},
    // R_CW_BEGIN
    {R_CW_NEXT, R_CW_BEGIN, R_START, R_START},
    // R_CW_NEXT
    {R_CW_NEXT, R_CW_BEGIN, R_CW_FINAL, R_START},
    // R_CCW_BEGIN
    {R_CCW_NEXT, R_START, R_CCW_BEGIN, R_START},
    // R_CCW_FINAL
    {R_CCW_NEXT, R_CCW_FINAL, R_START, R_START | DIR_CCW},
    // R_CCW_NEXT
    {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};
unsigned char count = 0;
unsigned char PinState = 0;
unsigned char Dir = 0;
unsigned char S = 0;
void LCDInit(void);
void DelayFor18TCY(void); //Delay 18TCY
void DelayPORXLCD(void); //Delay 15ms
void DelayXLCD(void);
void ftostr(float, unsigned char*, unsigned char);
void inttostr(int, char, unsigned char*, unsigned char);
int intToStr(int, char[], int);
void DisplayStarEnDis(bool);
void SetCurrentDisplay(unsigned char);
void SetVoltageDisplay(unsigned int);
void DisplayCapacity(unsigned int);
void BatteryVoltageDisplay(unsigned long);
void ChangePWM(unsigned int);
void CCPInit(void);
void InterruptInit(void);
void EncoderInterruptInit(void);
void Timer0Init(void);
void Timer1Init(void);
void ADCInit(void);
unsigned long BatteryVoltageMeasure(void);
void IsCurrent(void);
void Timer1EnDis(bool);
void EncoderInterruptISR(void);
void Timer1InterruptISR(void);
void EncoderEnDis(bool);
void PWMEnDis(bool);
bool DebonceButton(void);
void EEPROMAccess(void);
void EEPROMWrite(unsigned char, unsigned char);
unsigned char EEPROMRead(unsigned char);

void main() {
    bool Pushed = false;
    bool Hold = false;
    bool PrevHold = false;
    unsigned char case2 = 0;
    unsigned char prevState = 0;
    unsigned char PrevSetCurrent = 0;
    unsigned char PrevSetCurrent1 = 0;
    unsigned char PrevSetVoltage = 0;
    unsigned int PrevSetPWM = 0;
    unsigned int SetPWMEEPROM = 0;
    unsigned char SetCurrentEEPROM = 0;
    unsigned int SetVoltageEEPROM = 0;
    unsigned long BatVoltage = 0;
    unsigned int Capacity = 0;
    unsigned int PrevCapacity = 0;
    char strBatVoltage[3];
    OSCCON = 0x72; //Chon tan so dao dong noi la 8Mhz, nguon xung clock tu dao dong noi
    BUTTONPIN = 1;
    DISCHARGELEDPIN = 0;
    DISCHARGELED = 1;
    INTCON2bits.RBPU = 0; //Dung dien tro keo len cho PORTB
    EncoderInterruptInit();
    InterruptInit();
    Timer0Init();
    Timer1Init();
    CCPInit();
    ADCInit();
    EEPROMAccess();
    SetPWMEEPROM = EEPROMRead(PWMVALHIADDR);
    SetPWMEEPROM = (SetPWMEEPROM << 8) | EEPROMRead(PWMVALLOADDR);
    SetCurrentEEPROM = EEPROMRead(CURRENTVALADDR);
    SetVoltageEEPROM = EEPROMRead(VOLVALHIADDR);
    SetVoltageEEPROM = (SetVoltageEEPROM << 8) | EEPROMRead(VOLVALLOADDR);
    if (SetPWMEEPROM > 800 || SetCurrentEEPROM > 200 || SetVoltageEEPROM > 420) {
        SetPWM = 0;
        SetCurrent = 0;
        SetVoltage = 0;
   } else {
        SetPWM = SetPWMEEPROM;
        SetCurrent = SetCurrentEEPROM;
        SetVoltage = SetVoltageEEPROM;
    }
    LCDInit();
    DisplayStarEnDis(true);
    SetCurrentDisplay(SetCurrent);
    SetVoltageDisplay(SetVoltage);
    while (1) {
        BatVoltage = BatteryVoltageMeasure();
        sprintf(strBatVoltage, "%lu%lu%lu", BatVoltage / 100000, (BatVoltage % 100000) / 10000, (BatVoltage % 10000) / 1000);
        if (DebonceButton() == true) {
            if (Pushed == false) {
                Pushed = true;
                TMR0H = 0x0B;
                TMR0L = 0xDC;
                INTCONbits.TMR0IF = 0;
                T0CONbits.TMR0ON = 1;
            }
            if (INTCONbits.TMR0IF == 1 && Hold == false) {
                Hold = true;
                PrevHold = true;
                State = 2;
                INTCONbits.TMR0IF = 0;
                T0CONbits.TMR0ON = 0;
            }
        } else {
            if (Pushed == true) {
                if (Hold == false) {
                    Pushed = false;
                    INTCONbits.TMR0IF = 0;
                    T0CONbits.TMR0ON = 0;
                    if (PrevHold == true) {
                        State = 0;
                        PrevHold = 0;
                    } else {
                        State++;
                        State = State % 2;
                    }
                } else {
                    Pushed = false;
                    Hold = false;
                }
            }
        }
        switch (State) {
            case 0:
                PORTCbits.RC2 = 0;
                PWMEnDis(false);
                Timer1EnDis(false);
                DISCHARGELED = 1;
                EncoderEnDis(true);
                ChangePWM(SetPWM);
                case2 = 0;
                break;
            case 1:
                PWMEnDis(false);
                Timer1EnDis(false);
                DISCHARGELED = 1;
                EncoderEnDis(true);
                case2 = 0;
                break;
            case 2:
                if (atoi(strBatVoltage) > 0 && atoi(strBatVoltage) > SetVoltage && SetCurrent != 0) {
                    if (case2 == 0) {
                        DisplayStarEnDis(false);
                        EncoderEnDis(false);
                        DISCHARGELED = 0;
                        Timer1EnDis(true);
                        PWMEnDis(true);
                        if (PrevSetCurrent1 != SetCurrent) {
                            PrevSetCurrent1 = SetCurrent;
                            PrevCapacity = Capacity;
                            Second = 0;
                        }
                        case2 = 1;
                    }
                    Capacity = (((SetCurrent * Second) / 3600.0) * 10) + PrevCapacity;
                } else {
                    State = 0;
                    PrevHold = false;
                }
                break;
        }
        if (State != prevState) {
            prevState = State;
            DisplayStarEnDis(true);
        }
        if (PrevSetCurrent != SetCurrent || PrevSetPWM != SetPWM) {
            PrevSetCurrent = SetCurrent;
            PrevSetPWM = SetPWM;
            EEPROMWrite(PWMVALLOADDR, SetPWM);
            EEPROMWrite(PWMVALHIADDR, SetPWM >> 8);
            EEPROMWrite(CURRENTVALADDR, SetCurrent);
            SetCurrentDisplay(SetCurrent);
        }
        if (PrevSetVoltage != SetVoltage) {
            PrevSetVoltage = SetVoltage;
            EEPROMWrite(VOLVALLOADDR, SetVoltage);
            EEPROMWrite(VOLVALHIADDR, SetVoltage >> 8);
            SetVoltageDisplay(SetVoltage);
        }
        BatteryVoltageDisplay(BatVoltage);
        DisplayCapacity(Capacity);
    }
}

/*Phuong phap chong nhay nut nhan. Xem tai: http://www.ganssle.com/debouncing.htm*/
bool DebonceButton() {
    unsigned int State = 0;
    State = (State << 1) | BUTTON | 0xF000;
    if (State == 0xF000)
        return true;
    return false;
}
/*---------------------------------------------------------------------*/
#pragma code Timer1Interrupt = 0x08

void Timer1Interrupt() {
    _asm
            goto Timer1InterruptISR
            _endasm
}
#pragma code
#pragma interrupt Timer1InterruptISR

void Timer1InterruptISR() {
    if (PIR1bits.TMR1IF == 1) {
        Second++;
        PIR1bits.TMR1IF = 0;
        TMR1H = 0x80;
        TMR1L = 0x00;
    }
}
#pragma code EncoderInterrupt = 0x18

void EncoderInterrupt() {
    _asm
            goto EncoderInterruptISR
            _endasm
}
#pragma code
#pragma interruptlow EncoderInterruptISR

void EncoderInterruptISR() {
    if (ENCODERINTIF == 1) {
        ENCODERINTIF = 0;
        /*Phuong phap chong nhay encoder. Xem tai: http://www.buxtronix.net/2011/10/rotary-encoders-done-properly.html*/
        PinState = ((PORTBbits.RB4) << 1) | PORTBbits.RB5;
        S = ttable[S & 0xf][PinState];
        Dir = S & 0x30;
        /*-----------------------------------------------------------------------------------*/
        if (Dir == DIR_CW) {
            switch (State) {
                case 0:
                    if (SetPWM < 800 && SetCurrent < 200) {
                        SetPWM = SetPWM + 8;
                        SetCurrent = SetCurrent + 2;
                    }
                    break;
                case 1:
                    if (SetVoltage < 420) {
                        SetVoltage = SetVoltage + 2;
                    }
                    break;
            }
        }
        if (Dir == DIR_CCW) {
            switch (State) {
                case 0:
                    if (SetPWM > 0 && SetCurrent > 0) {
                        SetPWM = SetPWM - 8;
                        SetCurrent = SetCurrent - 2;
                    }
                    break;
                case 1:
                    if (SetVoltage > 0) {
                        SetVoltage = SetVoltage - 2;
                    }
                    break;
            }
        }
    }
}

void InterruptInit() {
    RCONbits.IPEN = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

void ChangePWM(unsigned int PWMValue) {
    CCP1CONbits.DC1B = PWMValue & 0x03;
    CCPR1L = PWMValue >> 2;
}

void DelayFor18TCY() {
    char i;
    for (i = 0; i < 17; i++) {
        Delay1TCY();
    }
}

void DelayPORXLCD() {
    Delay1KTCYx(40);
}

void DelayXLCD() {
    Delay1KTCYx(30);
}

void EncoderInterruptInit() {
    ENCODERAPIN = 1;
    ENCODERBPIN = 1;
    ENCODERINTIF = 0; // Encoder dung ngat thay doi PORTB, bit co la RBIF
    INTCON2bits.RBIP = 0; // Ngat encoder co do uu tien thap
}

void LCDInit() {
    OpenXLCD(FOUR_BIT & LINES_5X7);
    while (BusyXLCD());
    WriteCmdXLCD(0x08); //Display/Cursor ON/OFF control: Display off, cursor off, blink off
    while (BusyXLCD());
    WriteCmdXLCD(0x01); //Clear Display
    while (BusyXLCD());
    WriteCmdXLCD(0x06); //Entry mode set: Shift cursor to right, no display shift
    while (BusyXLCD());
    WriteCmdXLCD(0x0C); //Display/Cursor ON/OFF control: Display on, cursor on, blink off
    while (BusyXLCD());
}

void CCPInit() {
    PR2 = 0xFF; // Chu ky PWM
    CCPR1L = 0x00;
    CCP1CON = 0x00; //Ngo ra PWM chi duy nhat chan RC2, khoi CCP tat
    TRISCbits.TRISC2 = 0; //RC2 la ngo ra
    T2CON = 0x00; //Timer 2 khong dung prescaler, tat Timer2
}

void Timer0Init() {//Timer0 dung de dem thoi gian nhan giu nut
    T0CON = 0b00000101; //Timer 0 hoat dong 16bit, dung nguon xung tu VDK, prescaler 1:64
    //INTCONbits.TMR0IF = 0;
    TMR0H = 0;
    TMR0L = 0;
}

void Timer1Init() {// Timer1 dung de dem thoi gian khi xa pin, cu 1 giay tao ngat
    T1CON = 0b10000110; //Xung clock VDK khong tu dao dong Timer1, prescaler 1:1, dao dong Timer1 tat, xung clock tu chan RC0
    IPR1bits.TMR1IP = 1; // Ngt Timer1 uu tien cao
    PIR1bits.TMR1IF = 0;
    TMR1H = 0x80; //Gia tri nap vao TMR1H:TMR1L de tran trong t giay la: TMR1H:TMR1L=(65536 - (t/(1/ftimer1)))
    TMR1L = 0x00; //t = 1s => TMR1H:TMR1L = 32768 = 0x8000
}

void EncoderEnDis(bool EnDis) {
    switch (EnDis) {
        case false:
            INTCONbits.RBIE = 0;
            break;
        case true:
            INTCONbits.RBIE = 1;
            break;
    }
}

void ADCInit() {
    ADCON1 = 0b00001110; // Vref+ = 5v, Vref- = 0v, AN0 la ngo vao tuong tu
    ADCON2 = 0b10010001; // Acqition time = 4Tad, AD clock Fosc/8
    ADCON0bits.ADON = 1; // Bat khoi ADC
}

unsigned long BatteryVoltageMeasure() {
    unsigned long BatteryVoltage = 0;
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO == 1);
    BatteryVoltage = ADRES * (unsigned long) 538;
    return BatteryVoltage;
}

void PWMEnDis(bool EnDis) {
    switch (EnDis) {
        case false://Khi khong dung PWM thì tat khoi CCP và Timer2
            CCP1CONbits.CCP1M = 0; //Tat khoi CCP
            T2CONbits.TMR2ON = 0; //Tat Timer2
            break;
        case true://Bat PWM
            CCP1CONbits.CCP1M = 0b1100; //Che do PWM, chan RC2 tich cuc muc cao
            T2CONbits.TMR2ON = 1;
            break;
    }
}

void Timer1EnDis(bool EnDis) {
    switch (EnDis) {
        case false://Khi tat Timer1, tat luon ca ngat va nguon dao dong cua Timer1 
            T1CONbits.TMR1ON = 0;
            PIE1bits.TMR1IE = 0;
            T1CONbits.T1OSCEN = 0;
            break;
        case true:// Bat Timer1, nap lai gia tri cho TMR1H:TMR1L
            PIR1bits.TMR1IF = 0;
            TMR1H = 0x80;
            TMR1L = 0x00;
            T1CONbits.T1OSCEN = 1;
            T1CONbits.TMR1ON = 1;
            PIE1bits.TMR1IE = 1;
            break;
    }
}

void DisplayStarEnDis(bool EnDis) {
    switch (EnDis) {
        case false:
            SetDDRamAddr(0x80);
            while (BusyXLCD());
            putcXLCD(' ');
            while (BusyXLCD());
            SetDDRamAddr(0xC0);
            while (BusyXLCD());
            putcXLCD(' ');
            break;
        case true:
            switch (State) {
                case 0:
                    SetDDRamAddr(0xC0);
                    while (BusyXLCD());
                    putcXLCD(' ');
                    while (BusyXLCD());
                    SetDDRamAddr(0x80);
                    while (BusyXLCD());
                    putcXLCD('*');
                    break;
                case 1:
                    SetDDRamAddr(0x80);
                    while (BusyXLCD());
                    putcXLCD(' ');
                    while (BusyXLCD());
                    SetDDRamAddr(0xC0);
                    while (BusyXLCD());
                    putcXLCD('*');
                    break;
            }
            break;
    }
}

void EEPROMAccess() {
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
}

void EEPROMWrite(unsigned char Address, unsigned char WriteValue) {
    EEADR = Address;
    EEDATA = WriteValue;
    EECON1bits.WREN = 1;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    while (EECON1bits.WR == 1);
    EECON1bits.WR = 0;
}

unsigned char EEPROMRead(unsigned char Address) {
    EEADR = Address;
    EECON1bits.RD = 1;
    while (EECON1bits.RD == 1);
    return (EEDATA);
}

void SetCurrentDisplay(unsigned char Current) {
    unsigned char strCurrent[7];
    sprintf(strCurrent, "%d.%d%dA", (int) Current / 100, (int) (Current % 100) / 10, (int) (Current % 100) % 10);
    SetDDRamAddr(0x81);
    while (BusyXLCD());
    putsXLCD(strCurrent);
}

void SetVoltageDisplay(unsigned int Voltage) {
    unsigned char strVoltage[7];
    sprintf(strVoltage, "%d.%d%dV", (int) Voltage / 100, (int) (Voltage % 100) / 10, (int) (Voltage % 100) % 10);
    SetDDRamAddr(0xC1);
    while (BusyXLCD());
    putsXLCD(strVoltage);
}

void BatteryVoltageDisplay(unsigned long BattVoltage) {
    unsigned char strBattVoltage[7];
    sprintf(strBattVoltage, "%lu.%lu%luV", BattVoltage / 100000, (BattVoltage % 100000) / 10000, (BattVoltage % 10000) / 1000);
    SetDDRamAddr(0x89);
    while (BusyXLCD());
    putsXLCD(strBattVoltage);
}

void DisplayCapacity(unsigned int Capacity) {
    unsigned char strCapacity[9];
    sprintf(strCapacity, "%d%d%d%d%dmAh", Capacity / 10000, (Capacity % 10000) / 1000, (Capacity % 1000) / 100, (Capacity % 100) / 10, Capacity % 10);
    SetDDRamAddr(0xC8);
    while (BusyXLCD());
    putsXLCD(strCapacity);
}


