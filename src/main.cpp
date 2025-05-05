//tarea
//codigo sin interrupciones 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

char canal = 0;

void select_channel(char channel) {
    if (channel <= 15) {
        ADMUX &= 0xF0;      
        ADMUX |= channel;   
    }
}

int main(void) {
    ADMUX |= (1 << REFS0);               
    ADCSRA |= (1 << ADEN) | (1 << ADPS2); 
    select_channel(0);                   
    
    DDRD &= ~0x04;  
    PORTD |= 0x04;  
    DDRD |= 0xF8;  
    DDRB |= 0x1F;   

    char alow = 0;
    char ahigh = 0;

    while (1) {
        // Leer botón con antirrebote
        if (!(PIND & 0x04)) { 
            _delay_ms(50); // Debounce
            if (!(PIND & 0x04)) { 
                canal++;
                if (canal > 2) canal = 0;
                select_channel(canal);
                while (!(PIND & 0x04)); 
            }
        }

        ADCSRA |= (1 << ADSC);             
        while (!(ADCSRA & (1 << ADIF)));   
        alow = ADCL;
        ahigh = ADCH;
        ADCSRA |= (1 << ADIF);           

        PORTD &= ~0xF8;
        PORTB &= ~0x1F;

        PORTD |= (alow << 3) & 0xF8;             
        PORTB |= ((alow >> 5) & 0x07) | ((ahigh << 3) & 0x18);
    }
}