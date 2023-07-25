#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "nokia5110.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// Definições dos pinos dos botões
#define EsqA        PD0
#define DirD        PD1
#define Espaço      PD2

// Variáveis
int recorde=0;
int timer;
int tempo;
char strTempo[16];
char strRecord[16];
char strTimer[16];

int xBola=42;
int yBola=5;
int rBola=5;
int xEsp;
int yEsp;
int rEsp=2;

int  opcao = 1;
bool menu = true;
bool regras = false;
bool game = false;
bool end = false;
bool colisao = false;

void setTimer() {
    TCCR1B |= (1 << WGM12);     // Modo CTC
    OCR1A = 15624;              // Valor para contar até 10 segundos (prescaler de 256)
    TIMSK1 |= (1 << OCIE1A);    // Habilita interrupção do Timer1 comparando com OCR1A
    TCCR1B |= (1 << CS12) | (1 << CS10);   // Prescaler de 1024
}
ISR(TIMER1_COMPA_vect) {
    if (timer > 0) {
        timer--; // Decrementa o tempo
    }
}


void criaEspinho()    {
    xEsp = (rand()%40) + 2; //define x
    yEsp = 42; //ponto de nascimento de espinho
    rEsp = 2; //raio
}
void contador()     {
    setTimer();
    sei();

    timer=3;
    nokia_lcd_clear();
    while (timer>0)
    {
        nokia_lcd_clear();
        sprintf(strTimer,"%d",timer);
        nokia_lcd_set_cursor(37,15);
        nokia_lcd_write_string(strTimer,2);
        nokia_lcd_render();
    }
    cli();
    nokia_lcd_render();
}
void menuTela()     {
    nokia_lcd_clear();
    nokia_lcd_set_cursor(5,0);
    nokia_lcd_write_string("Fallin'",2);    //titulo
    
    //botao de iniciar
    nokia_lcd_drawrect(30,17,50,37);    //quadrado
    //desenhar triangulo de play
    nokia_lcd_drawline(37,20,37,35);
    nokia_lcd_drawline(37,35,45,27);
    nokia_lcd_drawline(37,21,45,28);

    nokia_lcd_set_cursor(0,40);
    nokia_lcd_write_string("Regras",1);
    nokia_lcd_set_cursor(48,40);
    nokia_lcd_write_string("Fechar", 1);
    
    //destaca opcao selecionada
    if (opcao==1)//regras
    {
        nokia_lcd_set_cursor(10,33);
        nokia_lcd_write_string("*",1);    
    }
    if (opcao==2)//iniciar
    {
        nokia_lcd_drawline(37,22,44,27);
        nokia_lcd_drawline(37,23,44,27);
        nokia_lcd_drawline(37,24,44,27);
        nokia_lcd_drawline(37,25,44,27);
        nokia_lcd_drawline(37,26,44,27);
        nokia_lcd_drawline(37,27,44,27);
        nokia_lcd_drawline(37,28,44,27);
        nokia_lcd_drawline(37,29,44,27);
        nokia_lcd_drawline(37,30,44,27);
        nokia_lcd_drawline(37,31,44,27);
        nokia_lcd_drawline(37,32,44,27);
        nokia_lcd_drawline(37,33,44,27);
        nokia_lcd_drawline(37,34,44,27);

    }
    if (opcao==3)//fechar
    {
        nokia_lcd_set_cursor(58,33);
        nokia_lcd_write_string("*",1);    
    }

    nokia_lcd_render();
}
void attOpcao()     {
    //vai pra direita
    if(PIND & (1<<DirD))
    {
        if (menu==true)
        {
            if (opcao==3)
            {
                opcao=1;
            }
            else opcao++; 

            while (PIND & (1<<DirD))//debounce
            {
                _delay_ms(1);
            }
            _delay_ms(1);
        }
        if (end==true)
        {
            if (opcao==2)
            {
                opcao=1;
            }
            else opcao++;
            while (PIND & (1<<DirD))//debounce
            {
                _delay_ms(1);
            }
            _delay_ms(1);
        }
    }
    if(PIND & (1<<EsqA))
    {
        if (menu==true)
        {
            if (opcao==1)
            {
                opcao=3;
            }
            else opcao--; 

            while (PIND & (1<<EsqA))//debounce
            {
                _delay_ms(1);
            }
            _delay_ms(1);
        }
        if (end==true)
        {
            if (opcao==1)
            {
                opcao=2;
            }
            else opcao--;
            while (PIND & (1<<EsqA))//debounce
            {
                _delay_ms(1);
            }
            _delay_ms(1);
        }
                
    }
    if (PIND & (1<<Espaço))
    {
        if (menu==true)
        {
            if (opcao==1)
            {
                regras=true;
            }
            if (opcao==2)
            {
                contador();
                timer=3;
                tempo=0;
                colisao=false;
                criaEspinho();
                game=true;
            }
            if (opcao==3)
            {
                nokia_lcd_clear();
                nokia_lcd_set_cursor(10,15);
                nokia_lcd_write_string("Obrigada por jogar!",1);
                nokia_lcd_render();
                exit(0);
            }
            menu=false;
        }

        if (end==true)
        {
            if (opcao==1)
            {
                menu=true;
            }
            if (opcao==2)
            {
                nokia_lcd_clear();
                nokia_lcd_set_cursor(10,15);
                nokia_lcd_write_string("Obrigada por jogar!",1);
                nokia_lcd_render();
                exit(0);
            }
            end=false;
        }
        

        while (PIND & (1<<Espaço))//debounce
        {
            _delay_ms(1);
        }
        _delay_ms(1);

    }

}
void showRegras()   {
    nokia_lcd_clear();
    
    nokia_lcd_set_cursor(0,0);
    nokia_lcd_write_string("1.Voce so tem uma vida.",1);
    nokia_lcd_set_cursor(0,20);
    nokia_lcd_write_string("2.Cuidado com os espinhos.",1);
    nokia_lcd_set_cursor(50,40);
    nokia_lcd_write_string("MENU",1);
    nokia_lcd_set_cursor(40,40);
    nokia_lcd_write_string("*",1);

    nokia_lcd_render();

    
    if (PIND & (1<<Espaço))
    {
        regras=false;
        menu=true;
    }
    while (PIND & (1<<Espaço))//debounce
    {
        _delay_ms(1);
    }
    _delay_ms(1);
    
}
void gameOver()     {

    nokia_lcd_clear();
    nokia_lcd_set_cursor(15,10);
    nokia_lcd_write_string("Game Over",1);
    nokia_lcd_set_cursor(12,25);

    //define o recorde
    if (tempo>recorde)
    {
        recorde=tempo;
    }

    sprintf(strRecord,"Recorde: %d", recorde);
    nokia_lcd_write_string(strRecord,1);

    nokia_lcd_set_cursor(0,40);
    nokia_lcd_write_string("MENU",1);

    nokia_lcd_set_cursor(48,40);
    nokia_lcd_write_string("Fechar",1);

    //destaca opcao selecionada
    if (opcao==1)//menu
    {
        nokia_lcd_set_cursor(10,33);
        nokia_lcd_write_string("*",1);    
    }
    if (opcao==2)//fechar
    {
        nokia_lcd_set_cursor(58,33);
        nokia_lcd_write_string("*",1);    
    }
    nokia_lcd_render();

}
bool checaColisao(int xB,int yB, int rB, int xE, int yE, int rE) {
    int distX = xB - xE;
    int distY = yB - yE;
    int distQuad = distX*distX + distY*distY;

    int rXY = rB+rE;
    int rXYQuad = rXY*rXY;

    return(distQuad<=rXYQuad);
}

void caminha()    {
    if (PIND & (1<<EsqA))
    {
        if (xBola>=9)
        {   
            xBola=xBola-5;
        }
        
        while (PIND & (1<<EsqA))//debounce
        {
            _delay_ms(1);
        }
        _delay_ms(1);


    }
    if (PIND & (1<<DirD))
    {
        if (xBola<=72)
        {   
            xBola=xBola+5;
        }
        while (PIND & (1<<DirD))//debounce
        {
            _delay_ms(1);
        }
        _delay_ms(1);

    }
}
void attEspinho()   {
    if (yEsp<3)
    {
        criaEspinho();
    }
    else    {
        yEsp--;
    }
    
}
void attTelaJogo()  {

    //atualiza bola e espinho
    caminha();
    attEspinho();

    //limpa tela
    nokia_lcd_clear();

    //verifica colisao
    colisao = checaColisao(xBola,yBola,rBola,xEsp,yEsp,rEsp);
    if (colisao)
    {
        end=true;
        game = false;
    }
    

    //desenha circulo
    nokia_lcd_drawcircle(xBola,yBola,rBola);

    //imprime espinho
    nokia_lcd_drawcircle(xEsp,yEsp,rEsp);


    //tempo passado no canto
    sprintf(strTempo,"%d",tempo);
    nokia_lcd_set_cursor(0,40);
    nokia_lcd_write_string(strTempo,1);

    nokia_lcd_render();
}
void jogaJogo() {

    //enquanto não houve colisao
    while (!colisao)
    {
        for (int i = 0; i < 10; i++)
        {
            attTelaJogo();
            _delay_ms(50);
            attTelaJogo();
            _delay_ms(50);
        }
        tempo++;
    }

}


int main(void)
{
    //inicia botoes de entrada
    DDRD &= ~(1<<EsqA);
    DDRD &= ~(1<<DirD);

    //inicia LCD
    nokia_lcd_init();

    //inicia espinhos
    criaEspinho();

    while (1)
    {        
        if (menu == true)   {
            menuTela();
            attOpcao();
        }

        if (regras == true)
        {
            showRegras();
        }
        
        if (end == true)    {
            gameOver();
            attOpcao();
        }

        if (game == true)   {
            jogaJogo();
        }    
    }

}

