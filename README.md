# 🔮 Oráculo Generativo (ESP32)

Servidor web local no **ESP32** que gera frases poéticas por **gramática de regras** e roda o **Jogo da Vida de Conway** em um display OLED.

**100% local:** sem internet, sem API e sem custo.

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-00878F?style=for-the-badge&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)

---

## ✨ O que faz

- Sobe um servidor web no ESP32, acessível pelo celular ou PC na mesma rede.
- **Nova frase:** gera uma frase poética por gramática (com concordância de gênero) e mostra no OLED e na página.
- **Arte:** roda o Jogo da Vida (autômato celular) no display, com mundo toroidal e reinício automático quando a população estabiliza.
- Se não conectar no seu Wi-Fi, o ESP32 cria a própria rede (modo AP) e mostra o endereço no display.

Exemplos de frases geradas:

> A lua antiga abraça o rio.
> Se a sombra dorme, o vento espera.
> Existe um mapa esquecido dentro de cada nuvem.

---

## 🧰 Hardware

| Item | Detalhe |
| --- | --- |
| Placa | ESP32-WROOM |
| Display | OLED 0,96" SSD1306 (I2C, 128x64) |
| Jumpers | 4 fios |

### Ligação

| OLED | ESP32 |
| --- | --- |
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

---

## 📚 Bibliotecas

Instale pelo Gerenciador de Bibliotecas da Arduino IDE:

- **Adafruit GFX Library**
- **Adafruit SSD1306**

`WiFi.h` e `WebServer.h` já vêm com o pacote **esp32** da Espressif.

---

## 🚀 Como usar

1. Clone o repositório:

   ```bash
   git clone https://github.com/maxwell-franca/esp32-oraculo-generativo.git
   ```

2. Na pasta `oraculo_generativo/`, copie `secrets.example.h` para `secrets.h` e preencha o nome e a senha do seu Wi-Fi:

   ```bash
   cp oraculo_generativo/secrets.example.h oraculo_generativo/secrets.h
   ```

3. Abra `oraculo_generativo/oraculo_generativo.ino` na Arduino IDE.
4. Selecione a placa **ESP32 Dev Module** e a porta correta.
5. Envie o código para a placa.
6. O display mostra o IP. Abra `http://<ip>` no navegador do celular ou PC.

> ⚠️ O arquivo `secrets.h` está no `.gitignore`. Nunca publique suas credenciais.

---

## 🧠 Como funciona

**Gramática generativa:** listas de substantivos (com gênero), adjetivos (masculino e feminino) e verbos (transitivos e intransitivos) são combinadas em 6 modelos de frase. Os artigos e adjetivos concordam com o gênero do substantivo, e o mesmo substantivo não se repete na frase.

**Jogo da Vida:** regras de Conway em uma grade de 128x64, com bordas conectadas. Uma célula viva com 2 ou 3 vizinhas sobrevive, e uma morta com exatamente 3 vizinhas nasce. A simulação reinicia sozinha quando a população para de mudar.

**Display:** a fonte padrão do OLED não tem acentos, então o texto é convertido para ASCII no display. A página web mostra a frase completa, com acentos.

---

## 🗺️ Estrutura

```
esp32-oraculo-generativo/
├── oraculo_generativo/
│   ├── oraculo_generativo.ino
│   └── secrets.example.h
├── .gitignore
├── LICENSE
├── COMMITS.md
└── README.md
```

---

## 🤝 Contribuindo

Este projeto usa o padrão de commits descrito em [COMMITS.md](COMMITS.md).

---

## 📄 Licença

MIT. Veja [LICENSE](LICENSE).

---

Feito por **[Maxwell França](https://github.com/maxwell-franca)** · Contador & Administrador apaixonado por IA, automação e sistemas embarcados.
