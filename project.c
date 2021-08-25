// LCD module connections
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;


char uart_rd;

char buffer[10] = "";
char temp[17] = "";

char pos = 0;
char newValue = 0;
unsigned currADCValue;
unsigned oldADCValue;
unsigned adc_int;

float tmp;

float period;
int tick;

void interrupt() {
	currADCValue = ADRESH<<2;
	ADIF_bit = 0;
	ADCON0 |= 0x02; // Start A/D conversion
}


void main() {
	ANSELA = 0x1;
	TRISA  = 0x1;
	ANSELC = 0;
	ANSELD = 0;
	ANSELB = 0;
	ANSELE = 0;
	ADCON0 = 0b00000001;
	ADCON1 = 0b00000000;
	ADCON2 = 0b00010100;

   Lcd_Init();
   Lcd_Cmd(_LCD_CURSOR_OFF);
   Lcd_Cmd(_LCD_CLEAR);
   Lcd_Out(1,1,"ODE projekat");
   
   UART1_Init(9600);
   Delay_ms(1000);
   
   Lcd_Cmd(_LCD_CLEAR);
   Lcd_Out(1,1,"Ivan Pesic");
   Lcd_Out(2,1,"Aleksa Markovic");
   Delay_ms(500);
   
	//ADC
	oldADCValue = 10;
	currADCValue = 0;
	ADIF_bit = 0;
	GIE_bit = 1;
	PEIE_bit = 1;
	ADIE_bit = 1;
	ADCON0 |= 0x02; // Start A/D conversion

	// DAC
	PWM5_Init(60000);
	PWM5_Start();
	PWM5_Set_Duty(127);
	tick = 100;
   
	while (1) {   
		--tick;
		if (UART1_Data_Ready()) {
			ADIE_bit = 0;

			uart_rd = UART1_Read();
			if (uart_rd == '!') {
				buffer[pos] = '\0';
				pos = 0;
				newValue = 1;
				uart_rd = 0;
				
				while(buffer[pos]) 
					UART1_Write(buffer[pos++]);
				
				pos = 0;
         
				period = ((buffer[0] - '0') * 100 + (buffer[2] - '0') * 10 + (buffer[3] - '0'));
				period = (period / 500.) * 255.;
         
				PWM5_Start();
				PWM5_Set_Duty((int)period);
				ADIE_bit = 1;
				ADCON0 |= 0x02;
			} 
			else {
				buffer[pos++] = uart_rd;
			}
		}	
    
		if (newValue){
			ADIE_bit = 0;
			IntToStr((int)period / 255. * 100., temp);
			Lcd_Cmd(_LCD_CLEAR);
			Lcd_Out(1, 1, "DC:%");
			Lcd_Out_CP(temp);
			Lcd_Out(2, 1, "Napon:");
			Lcd_Out_CP(buffer);
			newValue = 0;
			ADIE_bit = 1;
			ADCON0 |= 0x02;
		}
    
		if (tick == 0 && !newValue && (currADCValue - oldADCValue > 2 || currADCValue - oldADCValue < -2)) {
			ADIE_bit = 0;
			oldADCValue = currADCValue;
			tmp = oldADCValue * 5.0 / 1023.0 * 100.0;
			adc_int=(unsigned)tmp;
			temp[0] = adc_int / 100 + '0';
			temp[1] = '.';
			temp[2] = (adc_int / 10) % 10 + '0';
			temp[3] = adc_int % 10 + '0';
			temp[4] = 'V';
			temp[5] = '\0';
			Lcd_Cmd(_LCD_CLEAR);
			Lcd_Out(1, 4, temp);
			tick = 10;
			ADIE_bit = 1;
			ADCON0 |= 0x02;
		}
	}
}