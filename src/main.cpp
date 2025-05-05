#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char channel = 0;  

void select_channel(char channel) { //configuracion del adc
    if (channel <= 15) {  
        ADMUX &= 0xF0;       
        ADMUX |= channel;         
    }
    }

void config_INT0() { //configruar interrupcion
        EIMSK |= (1 << INT0);    //configurar inetrrupcion
        EICRA |= (1 << ISC01);    // configurar interrupcion en puerto d
    
        DDRD &= ~0x04;      //pin d2 a utilizar
        PORTD |=0x04;
}  
    
ISR(INT0_vect) { //q haremos  con la interrupcion
    _delay_ms(50);  
        channel++;
        if (channel > 2){
            channel = 0;
        }
        select_channel(channel);  // Siempre actualizar el canal
    }


int main(void) {

    ADMUX |= (1 << REFS0); 
    ADCSRA |= (1 << ADEN) | (1 << ADPS2); 
    select_channel(0);
    config_INT0(); //agregar config para que lea las interrupciones
    sei();  

    DDRD |= 0xF8;    //los leds
    DDRB |= 0x1F;   

    char alow = 0; //variable
    char ahigh = 0;

    while (1) { //el while permanece con lso desplazamientos

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
