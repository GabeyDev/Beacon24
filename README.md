# Beacon24
Código feito para ESP32 criado com a intenção de realizar a leitura de um Beacon, mostrando seu MAC, Major, Minor, RSSI e sua Distância do recebedor de informações 


O código começa importando as bibliotecas necessárias para a detecção de beacons Bluetooth de proximidade. Em seguida, são definidas algumas variáveis e constantes, como o tempo de varredura em segundos.

A classe MyAdvertisedDeviceCallbacks é criada para lidar com os eventos de detecção de dispositivos anunciados. Ela herda da classe BLEAdvertisedDeviceCallbacks e implementa o método onResult, que é chamado sempre que um dispositivo é detectado durante a varredura.

Dentro do método onResult, o código extrai os dados do fabricante do dispositivo anunciado e verifica se o UUID de proximidade corresponde ao UUID do beacon desejado. Se for correspondente, o código imprime informações relevantes, como o UUID, o RSSI, o Major e o Minor do beacon. Em seguida, ele calcula a distância estimada com base no RSSI.

Após imprimir as informações, o código redefine o UUID, o Major e o Minor do objeto id para limpar o histórico de varredura.

No método setup, o código inicializa a comunicação serial e o dispositivo BLE. Em seguida, ele obtém uma instância da classe BLEScan e define a classe MyAdvertisedDeviceCallbacks como o callback para os eventos de dispositivo anunciado. Por fim, ele ativa a varredura BLE e imprime uma mensagem indicando que a varredura está em andamento.

No método loop, o código inicia a varredura por um determinado período de tempo e imprime o número de dispositivos encontrados. Em seguida, ele limpa os resultados da varredura para liberar memória e aguarda um segundo antes de iniciar a próxima varredura.
