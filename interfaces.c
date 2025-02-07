#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"


#include "include/ssd1306.h"
#include "include/font.h"
#include "ws2812.pio.h"


// ---------------- Variáveis - Início ----------------


static volatile uint32_t last_time = 0; // Variável para armazenar o tempo do último callback
static ssd1306_t *ssd_pointer; // Variável para armazenar o ponteiro do display
static char *string_a_pointer, *string_b_pointer; // Variáveis para armazenar as strings de estado dos leds


// ---------------- Variáveis - Fim ----------------



// ---------------- Defines - Início ----------------


#define UART_ID uart0 // Define a porta UART a ser utilizada


#define I2C_PORT i2c1 // Define a porta I2C a ser utilizada
#define I2C_SDA 14 // Define o pino de dados do barramento I2C
#define I2C_SCL 15 // Define o pino de clock (SCL) do barramento I2C
#define endereco 0x3C // Define o endereço do display


#define button_A 5 // Define o pino do botão verde.
#define button_B 6   // Define o pino do botão vermelho.


#define red_rgb 13     // Define o pino vermelho do LED.
#define green_rgb 11   // Define o pino verde do LED.
#define blue_rgb 12    // Define o pino azul do LED.


#define luz 1        // Define a intensidade da luz (1-255)


// ---------------- Defines - Fim ----------------



// ---------------- WS2812 - Início ----------------


// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7


// Definição de pixel GRB
struct pixel_t
{
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.


// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];


// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;


// Inicializa a máquina PIO para controle da matriz de LEDs.
void npInit(uint pin)
{


  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2812_program);
  np_pio = pio0;


  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0)
  {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }


  // Inicia programa na máquina PIO obtida.
  ws2812_program_init(np_pio, sm, offset, pin, 800000.f);


  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}


// Atribui uma cor RGB a um LED.
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b)
{
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}


// Limpa o buffer de pixels.
void npClear()
{
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}


// Escreve os dados do buffer nos LEDs.
void npWrite()
{
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}


// ---------------- WS2812 - Fim ----------------



// ---------------- Desenhar - Início ----------------


// Função para facilitar o desenho no WS2812 utilizando 3 matrizes para o R, G e B.
void npDraw(uint8_t vetorR[5][5], uint8_t vetorG[5][5], uint8_t vetorB[5][5])
{
  int i, j,idx,col;
    for (i = 0; i < 5; i++) {
        idx = (4 - i) * 5; // Calcula o índice base para a linha.
        for (j = 0; j < 5; j++) {
            col = (i % 2 == 0) ? (4 - j) : j; // Inverte a ordem das colunas nas linhas pares.
            npSetLED(idx + col, vetorR[i][j], vetorG[i][j], vetorB[i][j]); // Preenche o buffer com os valores da matriz.
        }
    }
}


// ---------------- Desenhar - Fim ----------------



// ---------------- Números Coloridos - Início ----------------

// Número 0 - Azul
void num_0() {
  uint8_t vetorR[5][5] = {0}; // Vermelho
  uint8_t vetorG[5][5] = {0}; // Verde
  uint8_t vetorB[5][5] = {    // Azul
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorR, vetorG, vetorB);
  npWrite();
  npClear();
}

// Número 1 - Verde
void num_1() {
  uint8_t vetorR[5][5] = {0}; // Vermelho
  uint8_t vetorG[5][5] = {    // Verde
    {  0  ,  0  , luz ,  0  ,  0  },
    {  0  , luz , luz ,  0  ,  0  },
    {  0  ,  0  , luz ,  0  ,  0  },
    {  0  ,  0  , luz ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  uint8_t vetorB[5][5] = {0}; // Azul
  npDraw(vetorR, vetorG, vetorB);
  npWrite();
  npClear();
}

// Número 2 - Vermelho
void num_2() {
  uint8_t vetorR[5][5] = {    // Vermelho
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  uint8_t vetorG[5][5] = {0}; // Verde
  uint8_t vetorB[5][5] = {0}; // Azul
  npDraw(vetorR, vetorG, vetorB);
  npWrite();
  npClear();
}

// Número 3 - Amarelo (Vermelho + Verde)
void num_3() {
  uint8_t vetorR[5][5] = {    // Vermelho
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  uint8_t vetorG[5][5] = {    // Verde
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  uint8_t vetorB[5][5] = {0}; // Azul
  npDraw(vetorR, vetorG, vetorB);
  npWrite();
  npClear();
}

// Número 4 - Ciano (Verde + Azul)
void num_4() {
  uint8_t vetorR[5][5] = {0}; // Vermelho
  uint8_t vetorG[5][5] = {    // Verde
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  }
  };
  uint8_t vetorB[5][5] = {    // Azul
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  }
  };
  npDraw(vetorR, vetorG, vetorB);
  npWrite();
  npClear();
}

// Número 5 - Magenta (Vermelho + Azul)
void num_5() {
  uint8_t vetorR[5][5] = {    // Vermelho
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  uint8_t vetorG[5][5] = {0}; // Verde
  uint8_t vetorB[5][5] = {    // Azul
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorR, vetorG, vetorB);
  npWrite();
  npClear();
}

// Número 6 - Branco (Vermelho + Verde + Azul)
void num_6() {
  uint8_t vetorR[5][5] = {    // Vermelho
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  uint8_t vetorG[5][5] = {    // Verde
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  uint8_t vetorB[5][5] = {    // Azul
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorR, vetorG, vetorB);
  npWrite();
  npClear();
}

// Número 7 - Azul
void num_7() {
  uint8_t vetorR[5][5] = {0}; // Vermelho
  uint8_t vetorG[5][5] = {0}; // Verde
  uint8_t vetorB[5][5] = {    // Azul
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  luz , luz ,  luz  ,  luz  },
    {  0  ,  0  , 0 ,  luz  ,  0  },
    {  0  ,  0  , 0 ,  luz ,  0  }
  };
  npDraw(vetorR, vetorG, vetorB);
  npWrite();
  npClear();
}

// Número 8 - Rosa (Magenta + Verde suave)
void num_8() {
  uint8_t vetorR[5][5] = {    // Vermelho
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  uint8_t vetorG[5][5] = {    // Verde (suave)
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
  };
  uint8_t vetorB[5][5] = {    // Azul
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorR, vetorG, vetorB);
  npWrite();
  npClear();
}

// Número 9 - Laranja (Vermelho + Verde forte)
void num_9() {
  uint8_t vetorR[5][5] = {    // Vermelho
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  uint8_t vetorG[5][5] = {    // Verde
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  uint8_t vetorB[5][5] = {0}; // Azul
  npDraw(vetorR, vetorG, vetorB);
  npWrite();
  npClear();
}

// ---------------- Números Coloridos - Fim ----------------



// Função para para exibir o número passado como parâmetro na matriz de LEDS.
int handle_numbers(char num) {
  switch(num) {
    case '0':
      num_0();
      break;
    case '1':
      num_1();
      break;
    case '2':
      num_2();
      break;
    case '3':
      num_3();
      break;
    case '4':
      num_4();
      break;
    case '5':
      num_5();
      break;
    case '6':
      num_6();
      break;
    case '7':
      num_7();
      break;
    case '8':
      num_8();
      break;
    case '9':
      num_9();
      break;
    default:
      npClear();
      npWrite();
      return 1;
  }
  printf("-------- %c Digitado na matriz! --------\n", num); // Printf para visualização no terminal
  return 0;
}


// ---------------- Números - Fim ----------------


// ---------------- Inicializações - Início ----------------


void start_display(ssd1306_t *ssd) {
   
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);


    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line


    ssd1306_init(ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(ssd); // Configura o display
    ssd1306_send_data(ssd); // Envia os dados para o display
   
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);
}


// Inicializa os botões configurando os pinos apropriados.
void init_buttons() {
  gpio_init(button_B);
  gpio_init(button_A);
  gpio_set_dir(button_B, GPIO_IN);
  gpio_set_dir(button_A, GPIO_IN);
  gpio_pull_up(button_B);
  gpio_pull_up(button_A);
}


// Inicializa o LED RGB configurando os pinos apropriados.
void init_RGB() {
  gpio_init(red_rgb);
  gpio_init(green_rgb);
  gpio_init(blue_rgb);
  gpio_set_dir(red_rgb,GPIO_OUT);
  gpio_set_dir(green_rgb,GPIO_OUT);
  gpio_set_dir(blue_rgb,GPIO_OUT);
  gpio_put(red_rgb, 0);
  gpio_put(green_rgb, 0);
  gpio_put(blue_rgb, 0);
}


// ---------------- Inicializações  - Fim ----------------


// Callback da interrupção dos botões
void gpio_irq_callback(uint gpio, uint32_t events) {


  // Obtém o tempo atual em microssegundos
  uint32_t current_time = to_ms_since_boot(get_absolute_time());


  // Verifica se passou tempo suficiente desde o último evento
  if (current_time - last_time > 200) { // 200 ms de debouncing
    last_time = current_time; // Atualiza o tempo do último evento


    // Verifica se e o botão A foi pressionado
    if( (gpio == button_A)) {


      gpio_put(green_rgb, !gpio_get(green_rgb)); // Alterna o estado do LED verde


      // Atualiza a string que representa o estado do LED verde
      if(gpio_get(green_rgb)) {
        string_a_pointer[0] = 'n';
        string_a_pointer[1] = ' ';
      }else {
        string_a_pointer[0] = 'f';
        string_a_pointer[1] = 'f';      
      }


      // Atualiza o trecho no display que representa o estado do LED verde
      ssd1306_draw_string(ssd_pointer, string_a_pointer, 96, 40);
      printf("-------- Botão A acionado! LED verde: o%s --------\n", string_a_pointer); // Printf para visualização no terminal
     
    }else


    // Verifica se e o botão B foi pressionado
    if( (gpio == button_B)) {


      gpio_put(blue_rgb, !gpio_get(blue_rgb)); // Alterna o estado do LED azul


      // Atualiza a string que representa o estado do LED azul
      if(gpio_get(blue_rgb)) {
        string_b_pointer[0] = 'n';
        string_b_pointer[1] = ' ';
      }else {
        string_b_pointer[0] = 'f';
        string_b_pointer[1] = 'f';      
      }


      // Atualiza o trecho no display que representa o estado do LED azul
      ssd1306_draw_string(ssd_pointer, string_b_pointer, 88, 48); // Desenha uma string
      printf("-------- Botão B acionado! LED azul: o%s --------\n", string_b_pointer); // Printf para visualização no terminal
    }


    ssd1306_send_data(ssd_pointer); // Atualiza o display


  }
}


int main() {
  char c[] = "?\0"; // String do caractere pressionado
  char string_a[] = " green led:off\0"; // String do estado do LED verde
  char string_b[] = " blue led :off\0"; // String do estado do LED azul


  ssd1306_t ssd; // Variável que representa o display


  stdio_init_all();


  // Inicializa a comunicação UART
  uart_init(UART_ID, 115200);
  gpio_set_function(0, GPIO_FUNC_UART);
  gpio_set_function(1, GPIO_FUNC_UART);


  // Inicializa o display OLED
  start_display(&ssd);


  // Inicializa a matriz de LEDs NeoPixel
  npInit(LED_PIN);
  npClear();
  npWrite();


  // Inicializa os botões
  init_buttons();


  // Inicializa o LED RGB
  init_RGB();


  // Configura interrupções para os botões
  gpio_set_irq_enabled_with_callback(button_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_callback);
  gpio_set_irq_enabled_with_callback(button_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_callback);


  // Ponteiros para a chamada o callback
  ssd_pointer = &ssd; // Ponteiro para o display
  string_a_pointer = &string_a[11]; // Ponteiro para a string_a
  string_b_pointer = &string_b[10]; // Ponteiro para a string_b


  // Desenha a interface inicial do display
  ssd1306_rect(&ssd, 0, 0, 128, 64, true, false); // Desenha o retângulo de fora
  ssd1306_rect(&ssd, 2, 2, 124, 60, true, false); // Desenha um retângulo de dentro
  ssd1306_draw_string(&ssd, "caractere ", 8, 8); // Desenha uma string
  ssd1306_draw_string(&ssd, "escrito:", 8, 16); // Desenha uma string
  ssd1306_draw_string(&ssd, c, 80, 16); // Desenha o caractere digitado
  ssd1306_draw_string(&ssd, string_a, 8, 40); // Desenha o estado do led verde
  ssd1306_draw_string(&ssd, string_b, 8, 48); // Desenha o estado do led azul


  ssd1306_send_data(&ssd); // Atualiza o display


  while(true) {
    sleep_ms(20);


    // Código para a comunicação serial com a BitDogLab pelo terminal do VScode
    if(stdio_usb_connected) {


      c[0] = getc(stdin); // Recebe o caractere digitado


      handle_numbers(c[0]); // Aciona a matriz


      // Atualiza a interface toda do display com o caractere recebido
      ssd1306_rect(&ssd, 0, 0, 128, 64, true, false); // Desenha o retângulo de fora
      ssd1306_rect(&ssd, 2, 2, 124, 60, true, false); // Desenha o retângulo de dentro
      ssd1306_draw_string(&ssd, "caractere", 8, 8); // Desenha uma string
      ssd1306_draw_string(&ssd, "escrito:", 8, 16); // Desenha uma string
      ssd1306_draw_string(&ssd, c, 80, 16); // Desenha o caractere digitado
      ssd1306_draw_string(&ssd, string_a, 8, 40); // Desenha o estado do led verde
      ssd1306_draw_string(&ssd, string_b, 8, 48); // Desenha o estado do led azul


      ssd1306_send_data(&ssd); // Atualiza o display
    }


    /*
    // Cuidado ao retirar esse comentário,
    // tanto a simulação no wokwi quanto o teste na placa funcionam com ambos os códigos rodando, entretanto,
    // a simulação no wokwi ficará bem lenta devido ao código da BitDogLab.
    // Recomendo comentar o código da BitDogLab para simular no Wokwi.
   


    // Código para a simulação no Wokwi
    if(uart_is_readable(UART_ID)) {


      c[0] = uart_getc(UART_ID); // Recebe o caractere digitado


      handle_numbers(c[0]); // Aciona a matriz


      // Atualiza a interface do display com o caractere recebido
      ssd1306_rect(&ssd, 0, 0, 128, 64, true, false); // Desenha o retângulo de fora
      ssd1306_rect(&ssd, 2, 2, 124, 60, true, false); // Desenha o retângulo de dentro
      ssd1306_draw_string(&ssd, "caractere", 8, 8); // Desenha uma string
      ssd1306_draw_string(&ssd, "digitado:", 8, 16); // Desenha uma string
      ssd1306_draw_string(&ssd, c, 80, 16); // Desenha o caractere digitado
      ssd1306_draw_string(&ssd, string_a, 8, 40); // Desenha o estado do led verde
      ssd1306_draw_string(&ssd, string_b, 8, 48); // Desenha o estado do led azul


      ssd1306_send_data(&ssd); // Atualiza o display
    }
    */


  }
}



