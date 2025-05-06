#define F_CPU 16000000UL
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

char channel = 0;  // Canal ADC actual
char alow = 0;
char ahigh = 0;

//canal
void select_channel(char channel) {
    if (channel <= 15) {
        ADMUX &= 0xF0;
        ADMUX |= channel;
    }
}

//configuracion de la intercepcion externa
void config_INT0() {
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC01);  

    DDRD &= ~0x04;  
    PORTD |= 0x04;  
}
//interrrupcion del pulsador
ISR(INT0_vect) {
    _delay_ms(50);  
    if (!(PIND & 0x04)) {
        channel++;
        if (channel > 2) {
            channel = 0;
        }
        select_channel(channel);  
    }
}

// interrucpion del adc
//operaciones
ISR(ADC_vect) {
    alow = ADCL;
    ahigh = ADCH;

    PORTD &= ~0xF8;
    PORTB &= ~0x1F;

    PORTD |= (alow << 3) & 0xF8;
    PORTB |= ((alow >> 5) & 0x07) | ((ahigh << 3) & 0x18);

    ADCSRA |= (1 << ADSC);  
}

int main(void) {
    // Configuración ADC
    ADMUX |= (1 << REFS0);  
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADIE);  

    DDRD |= 0xF8;  
    DDRB |= 0x1F; 

    sei();  
    select_channel(0);
    config_INT0();
    
    ADCSRA |= (1 << ADSC);  

    while (1) {
    }
}
