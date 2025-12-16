#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//timer0
volatile int timer0_counter = 0;
volatile int blink_threshold = 122;
volatile int blink_speed = 0;

//timer1
volatile int brightness_level = 0;

//timer2 
volatile int debounce_counter = 0;
volatile int debounce_active = 0;


int btn1_last= 0;
int btn2_last= 0;

volatile int blink_values[3] = {122, 61, 31};
volatile int pwm_values[5] = {0, 64, 128, 192, 255};


//Timer0-Blink LED
ISR(TIMER0_COMPA_vect) {
  timer0_counter++;
  if (timer0_counter >= blink_threshold) {
    timer0_counter = 0;
    PORTB ^= (1 << PB0);
  }
}

//Timer2-Debounce
ISR(TIMER2_OVF_vect) {
  if (debounce_active) {
    debounce_counter++;
    if (debounce_counter >= 15) {
      debounce_counter = 0;
      debounce_active = 0;
    }
  }
}



void setup(void) {
  
//GPIO Setup 
  DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);  
  DDRD &= ~((1 << PD2) | (1 << PD3));            
  PORTD |= (1 << PD2) | (1 << PD3);              
  PORTB |= (1 << PB2);                          
  
//Timer0:CTC Mode
  TCCR0A = 0x00;
  TCCR0B = 0x00;
  TCCR0A |= (1 << WGM01);                        
  TCCR0B |= (1 << CS02) | (1 << CS00);          
  OCR0A = 255;                                   
  TIMSK0 |= (1 << OCIE0A);                      
  
//Timer1:FastPWM 8bit
  TCCR1A = 0x00;
  TCCR1B = 0x00;
  TCCR1A |= (1 << WGM10);                       
  TCCR1B |= (1 << WGM12);
  TCCR1A |= (1 << COM1A1);                       
  TCCR1B |= (1 << CS11) | (1 << CS10);      
//startoff 
  OCR1A = 0;                                     

//Timer2:NormalMode
  TCCR2A = 0x00;
  TCCR2B = 0x00;
//Prescaler1024
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); 
    // Enable overflow interrupt
   
  TIMSK2 |= (1 << TOIE2);                      
  
  sei(); 
}



void loop(void) {
  
  //current states
  int btn1_pressed = ((PIND & (1 << PD2)) == 0);  
  int btn2_pressed = ((PIND & (1 << PD3)) == 0);
  
//Button1-BlinkSpeed
  if (btn1_pressed && !btn1_last&& !debounce_active) {
    blink_speed++;
    if (blink_speed > 2) blink_speed = 0;
    blink_threshold = blink_values[blink_speed];
    timer0_counter = 0;
    debounce_active = 1;
    debounce_counter = 0;
    TCNT2 = 0;
  }
  btn1_last= btn1_pressed; 
//Button2-Brightness
  if (btn2_pressed && !btn2_last&& !debounce_active) {
    brightness_level++;
    if (brightness_level > 4) brightness_level = 0;
    OCR1A = pwm_values[brightness_level];
    debounce_active = 1;
    debounce_counter = 0;
    TCNT2 = 0;
  }
  btn2_last= btn2_pressed; 
  
  _delay_ms(10); 
}


int main(void) {
  setup();
  while (1) loop();
  return 0;
}
