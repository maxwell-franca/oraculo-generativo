# 🔮 Oráculo Generativo

**Servidor web local em ESP32 que gera frases por gramática de regras e roda o Jogo da Vida no display OLED.**

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-00878F?style=for-the-badge&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Licença](https://img.shields.io/badge/Licen%C3%A7a-MIT-green?style=for-the-badge)

---

## 🧠 O que é

Um projeto embarcado que une dois conceitos de **geração procedural** em um único microcontrolador:

- 📜 **Gramática generativa:** o ESP32 monta frases (as "profecias" do oráculo) combinando regras e listas de palavras, sem IA e sem internet.
- 🧬 **Jogo da Vida de Conway:** autômato celular rodando em tempo real no display OLED.
- 🌐 **Servidor web local:** acesse pelo navegador, na mesma rede Wi-Fi, para pedir uma nova frase.

---

## ✨ Funcionalidades

- Geração de frases por gramática de regras
- Jogo da Vida animado no OLED
- Servidor web embutido no ESP32
- Credenciais Wi-Fi separadas do código (`secrets.h`, fora do Git)

---

## 🔧 Hardware

| Componente | Observação |
| --- | --- |
| ESP32 | Placa de desenvolvimento |
| Display OLED | I2C (ajuste o modelo e os pinos conforme o seu) |
| Cabo USB | Para gravar e alimentar |

---

## 📁 Estrutura

```
oraculo-generativo/
├── oraculo_generativo/
│   ├── oraculo_generativo.ino   # código principal
│   └── secrets.example.h        # modelo das credenciais
├── COMMITS.md                   # padrão de commits do projeto
├── LICENSE
└── README.md
```

---

## 🚀 Como usar

1. **Clone o repositório**
   ```
   git clone https://github.com/maxwell-franca/oraculo-generativo.git
   ```
2. **Crie o seu `secrets.h`** a partir do modelo, dentro da pasta `oraculo_generativo`:
   ```
   copy oraculo_generativo\secrets.example.h oraculo_generativo\secrets.h
   ```
   Abra o `secrets.h` e preencha o nome e a senha do seu Wi-Fi. Esse arquivo está no `.gitignore` e **não vai para o GitHub**.
3. **Abra o `oraculo_generativo.ino`** na Arduino IDE, instale as bibliotecas do display OLED e selecione a sua placa ESP32.
4. **Grave no ESP32** e abra o Monitor Serial para ver o endereço IP.
5. **Acesse o IP no navegador** (na mesma rede) e peça uma nova frase.

---

## 📝 Padrão de commits

Este projeto usa commits com gitmoji. Os detalhes estão em [COMMITS.md](COMMITS.md).

---

## 📄 Licença

Distribuído sob a licença **MIT**. Veja o arquivo [LICENSE](LICENSE).

---

## 👤 Autor

**Maxwell França** · Contador & Administrador | IA, Automação e Sistemas Embarcados  
🇧🇷 São Luís – MA, Brasil

<a href="https://github.com/maxwell-franca">
  <img src="https://img.shields.io/badge/GitHub-maxwell--franca-181717?style=for-the-badge&logo=github" />
</a>

---

⭐ Se gostou do projeto, deixe uma estrela no repositório
