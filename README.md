# Biblioteca driver do gamepad Gamesir G3S BLE para ESP32

## Introdução

Essa biblioteca vem da primeira vez que eu cursei a disciplina de **Sistemas Embarcados (INF351)** na **Universidade Federal de Viçosa - UFV**. Eu tive que cancelar minha matricula no período por motivos medicos, mas acabei ficando com um ESP32 do professor. Só que eu não tinha mais nada para interagir com a placa (ela tinha um sistema touch em alguns pinos, mas não funcionava na velocidade que eu queria). Só que eu tinha esse controle e a placa suportava BLE (Bluetooth Low Energy). O problema é que eu não encontrei documentação nenhuma de como comunicar e recuperar os dados de interação com o gamepad. Assim iniciei uma saga para descobrir como funcionava e na época deu relativamente certo (hoje depois de uns ajustes, eu descobri que estava incompleto o mapeamento, mas já consertei).

## Funções

- `JoystickClient()`;

    Cria um novo dispositivo Bluetooth e começa a pesquisar pelo dispositivo (no caso o MAC aqui é fixo, então só suporta meu controle por enquanto (Futuramente, será possível escolher, caso se prove que as configurações de controles Bluetooth são as mesmas)).

- `bool isFind()`;

    Retorna true quando o dispositivo foi encontrado.

- `void connectToServer()`;

    Cria uma nova instância de cliente Bluetooth e a conecta ao servidor.
 
- `bool isConnected()`;

    Retorna true quando a conexão cliente/servidor foi feita.

- `void update();`

    Envia uma requisição de leitura de valores para o servidor e guarda o retorno se ele estiver correto.

- `std::pair<int, int> getXY();`

    Retorna o valor do eixo X e Y entre -1 < 0 < 1, baseado nos valores do joystick esquerdo e direcionais digitais.

- `bool aPressed();`

    Retorna true se A for pressionado.

- `bool bPressed()`;

    Retorna true se B for pressionado.

- `bool startPressed();`

    Retorna true se START for pressionado.

## Casos de uso

São usados nos projetos [Jogo_da_Memória](https://github.com/hamsty/Jogo_da_Memoria/) e [Labirinto](https://github.com/hamsty/Labirinto/).
