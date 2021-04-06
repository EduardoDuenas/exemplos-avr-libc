/**
 * 
 * @file main.c
 * @author Eduardo Dueñas / Daniel Quadros
 * @brief Exemplo tocar musicas usando buzzer
 * @version 0.3
 * @date 03-04-2021
 * 
 * 
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16500000L


#define bitToggle(endereco,bit) (endereco ^= (1<<bit))
#define bitSet(endereco,bit) (endereco |= (1<<bit))
#define bitClear(endereco,bit) (endereco &= ~(1<<bit))
#define bitTest(endereco,bit) (endereco & (1<<bit))
#define setBit(valor,bit) (valor |= (1<<bit))
#define clearBit(valor,bit) (valor &= ~(1<<bit))
#define toogleBit(valor,bit) (valor ^= (1<<bit))
#define testBit(valor,bit)    (valor & (1<<bit))

#define NumNotas 31
#define define CONT(freq) ((F_CPU*10L)/(256L*freq))

enum notas{Pausa,Do, DoS, Re, ReS, Mi, Fa, FaS, Sol, SolS, La, LaS, Si, DoM, DoSM, ReM};
long f[15] = { 255L, (long)CONT(2616L), (long)CONT(2772L), (long)CONT(2937L), (long)CONT(3111L), (long)CONT(3296L), (long)CONT(3492L),
             (long)CONT(3700L), (long)CONT(3920L), (long)CONT(4153L), (long)CONT(4400L), (long)CONT(4662L), (long)CONT(4939L),
             (long)CONT(5233L), (long)CONT(5543L), (long)CONT(5873L) };
            /*{0xFF,(long)63067L, (long)59527L, (long)56186L, (long)53033L, 
            (long)50056L, (long)47247L, (long)44595L, (long)42092L, (long)39729L, (long)37500L, 
            (long)35395L, (long)33408L, (long)31534L, (long)29764L};*/   //valor a ser colocado na flag do timer para cada nota
            
char Partitura[NumNotas] = {Re,Mi,Mi,Re,Sol,FaS,FaS,FaS,Re,Mi,Mi,Re,La,Sol,Sol,Sol,Re,ReM,ReM,Si,Sol,FaS,FaS,Mi,
                            DoM,Si,Sol,Fa,Sol,Sol,Sol};                          //partitura da música
volatile char cont = 0;                                          //local da partitura
volatile long aux = 0;


//tratamento de interrupção 
ISR (TIM0_COMPB_vect){  //vetor de comparação B
    if (aux<=0xff) {                        //se aux menor que 8bits 
        OCR0B=(TCNT0+aux)&(0xff);           //mandar aux para o contador
        aux=f[Partitura[cont]];             //reinicia o aux
        toogleBit(PORTB,PB1);               //inverter o buzzer
    }
    else{                                   //se não
       OCR0B=TCNT0;                         //mandar o tempoatual para o contador, o mesmo que esperar um overflow
       aux-=0xff;                           //subitrair 8bits do aux
    }            
}


//função main
void main(){
    enum notas nota;  
    setBit(DDRB,PB1);            //configura o PortB1 como saida, pino do buzzer

    //configuração do timer
    TCCR0A=0x00;                            //configura pino de compararação desconectado
    TCCR0B=0x04;                            //configura o prescaler como 256
    setBit(TIMSK,OCIE0B);        //habilita a interrupção por comparação de COMPB
    sei();                                  //habilita interrupções globais
    aux=f[Partitura[cont]];                 //inicia o contador de COMPB

    if (aux<=0xff) {
        OCR0B=(TCNT0+aux)&(0xff);
        aux=f[Partitura[cont]];
        toogleBit(PORTB,PB1);
    }
    else{
       OCR0B=TCNT0;
       aux-=0xff;
    }
    

    //loop infinito
    for(;;){                    
        

        
        /*long i, j;
        for(i=0;i>1000000L;i++){
                for(j=0;j>10L;j++){}
        }       //espera um tempo (altere o valor central para acelerar ou desacelerar a música)*/
        cont++;                         //avança na partitura
        if (cont >= NumNotas)cont=0;    //toca de novo
    }
}