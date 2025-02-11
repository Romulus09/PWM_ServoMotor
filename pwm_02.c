#include <stdio.h> //biblioteca padrão da linguagem C
#include "pico/stdlib.h" //subconjunto central de bibliotecas do SDK Pico
#include "hardware/pwm.h" //biblioteca para controlar o hardware de PWM

#define PWM_SERV 22 //pino do LED conectado a GPIO como PWM
const uint16_t WRAP_PERIOD = 20000; //valor máximo do contador - WRAP
const float PWM_DIVISER = 125.0; //divisor do clock para o PWM
const uint16_t LED_STEP = 5; //passo de incremento/decremento para o duty cycle do LED
uint16_t serv_level = 200; //nível inicial do pwm (duty cycle)
bool move = true;

//função para configurar o módulo PWM
void pwm_setup()
{
    gpio_set_function(PWM_SERV, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM

    uint slice = pwm_gpio_to_slice_num(PWM_SERV); //obter o canal PWM da GPIO

    pwm_set_clkdiv(slice, PWM_DIVISER); //define o divisor de clock do PWM

    pwm_set_wrap(slice, WRAP_PERIOD); //definir o valor de wrap

    pwm_set_gpio_level(PWM_SERV, 200); //definir o cico de trabalho (duty cycle) do pwm

    pwm_set_enabled(slice, true); //habilita o pwm no slice correspondente
}

//função para iniciar o programa posicionando o motor nos angulos 180 , 90 e 0, sequencialmente, com 5 segundos de intervalo em cada posição
void servo_move(){
if (move == true)
{
    pwm_set_gpio_level(PWM_SERV, 2400); //180 graus
    sleep_ms(5000);
    pwm_set_gpio_level(PWM_SERV, 1470); //90 graus
    sleep_ms(5000);
    pwm_set_gpio_level(PWM_SERV, 500); //0 graus
    sleep_ms(5000);



} move = false; //após ser executada uma vez, a função não se repete
}


//função principal
int main()
{
    stdio_init_all(); //inicializa o sistema padrão de I/O
    
    pwm_setup(); //configura o PWM

    uint up_down = 1; //variável para controlar se o nível do LED aumenta ou diminui

    //loop principal
    while (true) {

        servo_move();

        printf("Ciclo ativo:%d\n", serv_level);//imprimir ciclo ativo do PWM 
        pwm_set_gpio_level(PWM_SERV, serv_level); //define o nível atual do PWM (duty cycle)
        

        sleep_ms(10); // Atraso de 1 segundo

        if (up_down) 
        {
            serv_level += LED_STEP; // Incrementa o angulo do motor
            if (serv_level*(25/3) >= WRAP_PERIOD)
                up_down = 0; // Muda direção para diminuir quando atingir o período máximo
        }
        else
        {
            serv_level -= LED_STEP; // Decrementa o angulo do motor
            if (serv_level <= LED_STEP)
                up_down = 1; // Muda direção para aumentar quando atingir o mínimo
        }

    }
}
