# Interfaces

Projeto desenvolvido para implementar e explorar o uso de interfaces em sistemas embarcados utilizando o microcontrolador RP2040.

## Descrição

Este projeto foca na criação de um sistema modular que utiliza interfaces para conectar e controlar diferentes componentes de hardware, como LEDs, botões e sensores. O objetivo principal é promover a flexibilidade e escalabilidade, permitindo a fácil adição ou modificação de dispositivos.

## Funcionalidades

- Controle de LEDs comuns e LEDs WS2812.
- Gestão de eventos com interrupções e debounce para botões.
- Suporte à integração com novos periféricos via interfaces definidas.
- Código modular e adaptável.

## Estrutura do Projeto

├── include/               # Arquivos de cabeçalho (.h) ├── src/                   # Código-fonte (.c) ├── lib/                   # Bibliotecas externas ou personalizadas ├── build/                 # Arquivos gerados durante a compilação ├── CMakeLists.txt         # Arquivo de configuração do CMake └── README.md              # Documentação do projeto

## Requisitos

- Microcontrolador RP2040.
- SDK do Raspberry Pi Pico.
- Ferramentas de desenvolvimento como VS Code e CMake.

## Como Configurar e Executar

1. Clone este repositório:
   ```bash
   git clone <URL_DO_REPOSITORIO>

2. Configure o ambiente com o Pico SDK.


3. Compile o projeto utilizando CMake:

mkdir build
cd build
cmake ..
make


4. Suba o binário no microcontrolador.



Contribuição

Contribuições são bem-vindas! Siga estas etapas para contribuir:

1. Faça um fork do repositório.


2. Crie uma nova branch: git checkout -b feature/nova-feature.


3. Realize suas alterações e faça commit: git commit -m 'Adicionei nova feature'.


4. Envie para o repositório remoto: git push origin feature/nova-feature.


5. Abra um Pull Request.

## Links Importantes

- [Repositório no GitHub](https://github.com/Isabelrs02/interfaces)
- [Documentação do Pico SDK](https://github.com/raspberrypi/pico-sdk)
- [Simulador Wokwi](https://wokwi.com/projects/422244629999626241)
- [Guia do CMake](https://cmake.org/documentation/)

## Demonstração no YouTube

- [Assista à demonstração do projeto]()
