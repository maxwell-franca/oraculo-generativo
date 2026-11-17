/*
  ORACULO GENERATIVO - ESP32-WROOM + OLED 0.96" SSD1306 (I2C)
  100% local: sem internet, sem API, sem custo.

  O que faz:
    - Sobe um servidor web no ESP32 (voce acessa pelo celular/PC).
    - Botao "Nova frase": gera uma frase poetica por gramatica (regras) e mostra no OLED e na pagina.
    - Botao "Arte": roda o Jogo da Vida (automato celular) no OLED.

  Bibliotecas (Gerenciador de Bibliotecas do Arduino IDE):
    - Adafruit GFX Library
    - Adafruit SSD1306
  (WiFi.h e WebServer.h ja vem com o pacote "esp32" da Espressif)

  Ligacao: OLED VCC->3V3 | GND->GND | SDA->GPIO21 | SCL->GPIO22
*/

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Credenciais ficam em secrets.h (ignorado pelo Git).
// Copie secrets.example.h para secrets.h e preencha com os seus dados.
#include "secrets.h"

#define LARGURA 128
#define ALTURA 64
#define ENDERECO_OLED 0x3C
#define PINO_SDA 21
#define PINO_SCL 22

Adafruit_SSD1306 display(LARGURA, ALTURA, &Wire, -1);
WebServer servidor(80);

enum Modo { MODO_TEXTO, MODO_ARTE };
Modo modo = MODO_TEXTO;

// ---------------------------------------------------------------
//  1) GRAMATICA GENERATIVA (texto)
// ---------------------------------------------------------------
#define QTD(a) (sizeof(a) / sizeof(a[0]))

struct Subst { const char* txt; bool fem; };
struct Adj   { const char* m; const char* f; };

const Subst SUBST[] = {
  {"lua", true}, {"sombra", true}, {"memória", true}, {"floresta", true},
  {"estrela", true}, {"porta", true}, {"nuvem", true}, {"carta", true},
  {"rio", false}, {"vento", false}, {"relógio", false}, {"silêncio", false},
  {"espelho", false}, {"sonho", false}, {"mapa", false}, {"segredo", false},
  {"mar", false}, {"caminho", false}
};

const Adj ADJ[] = {
  {"antigo", "antiga"}, {"distante", "distante"}, {"silencioso", "silenciosa"},
  {"curioso", "curiosa"}, {"invisível", "invisível"}, {"eterno", "eterna"},
  {"esquecido", "esquecida"}, {"luminoso", "luminosa"},
  {"inquieto", "inquieta"}, {"gentil", "gentil"}
};

// verbos que pedem objeto (3a pessoa do singular)
const char* VERB_TRANS[] = {
  "abraça", "esconde", "desenha", "protege", "segue",
  "lembra", "acorda", "carrega", "imita", "procura"
};

// verbos que nao pedem objeto
const char* VERB_INTR[] = {
  "dorme", "sonha", "espera", "dança", "respira",
  "brilha", "derrete", "voa", "cai", "recomeça"
};

String artDef(const Subst& s) { return s.fem ? "a" : "o"; }
String artInd(const Subst& s) { return s.fem ? "uma" : "um"; }
const char* adjDe(const Adj& a, const Subst& s) { return s.fem ? a.f : a.m; }

String gerarFrase() {
  const Subst& a = SUBST[random(QTD(SUBST))];
  int j;
  do { j = random(QTD(SUBST)); } while (&SUBST[j] == &a);   // evita repetir o substantivo
  const Subst& b = SUBST[j];

  const Adj& adj = ADJ[random(QTD(ADJ))];
  const char* vt = VERB_TRANS[random(QTD(VERB_TRANS))];
  const char* vi1 = VERB_INTR[random(QTD(VERB_INTR))];
  const char* vi2;
  do { vi2 = VERB_INTR[random(QTD(VERB_INTR))]; } while (vi2 == vi1);

  String f;
  switch (random(6)) {
    case 0:  // O rio antigo abraça a lua.
      f = artDef(a) + " " + a.txt + " " + adjDe(adj, a) + " " + vt + " " + artDef(b) + " " + b.txt + ".";
      break;
    case 1:  // O vento sonha quando a lua respira.
      f = artDef(a) + " " + a.txt + " " + vi1 + " quando " + artDef(b) + " " + b.txt + " " + vi2 + ".";
      break;
    case 2:  // Existe um mapa esquecido dentro de cada nuvem.
      f = String("Existe ") + artInd(a) + " " + a.txt + " " + adjDe(adj, a) + " dentro de cada " + b.txt + ".";
      break;
    case 3:  // Se a sombra dorme, o rio espera.
      f = String("Se ") + artDef(a) + " " + a.txt + " " + vi1 + ", " + artDef(b) + " " + b.txt + " " + vi2 + ".";
      break;
    case 4:  // A porta é eterna, como um segredo.
      f = artDef(a) + " " + a.txt + " é " + adjDe(adj, a) + ", como " + artInd(b) + " " + b.txt + ".";
      break;
    default: // Por que a lua dorme?
      f = String("Por que ") + artDef(a) + " " + a.txt + " " + vi1 + "?";
      break;
  }
  f.setCharAt(0, toupper(f[0]));   // primeira letra maiuscula (sempre comeca com letra ASCII)
  return f;
}

// ---------------------------------------------------------------
//  Display: a fonte padrao nao tem acentos, entao removemos
//  (a pagina web mostra a frase completa, com acentos)
// ---------------------------------------------------------------
String semAcento(const String& s) {
  String o;
  o.reserve(s.length());
  for (size_t i = 0; i < s.length(); i++) {
    uint8_t c = s[i];
    if (c == 0xC3 && i + 1 < s.length()) {   // caractere acentuado em UTF-8
      uint8_t d = s[++i];
      char r = '?';
      if      (d >= 0xA0 && d <= 0xA5) r = 'a';
      else if (d == 0xA7)              r = 'c';
      else if (d >= 0xA8 && d <= 0xAB) r = 'e';
      else if (d >= 0xAC && d <= 0xAF) r = 'i';
      else if (d >= 0xB2 && d <= 0xB6) r = 'o';
      else if (d >= 0xB9 && d <= 0xBC) r = 'u';
      else if (d >= 0x80 && d <= 0x85) r = 'A';
      else if (d == 0x87)              r = 'C';
      else if (d >= 0x88 && d <= 0x8B) r = 'E';
      else if (d >= 0x8C && d <= 0x8F) r = 'I';
      else if (d >= 0x92 && d <= 0x96) r = 'O';
      else if (d >= 0x99 && d <= 0x9C) r = 'U';
      o += r;
    } else {
      o += (char)c;
    }
  }
  return o;
}

void imprimirLinha(const String& linha, int y) {
  if (y <= 56 && linha.length() > 0) {
    display.setCursor(0, y);
    display.print(linha);
  }
}

// quebra o texto em linhas de ate 21 caracteres, sem cortar palavras
void mostrarTexto(const String& txt) {
  String t = semAcento(txt);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  const int MAX_COL = 21;
  int y = 0;
  String linha = "";
  int inicio = 0;
  while (inicio < (int)t.length()) {
    int fim = t.indexOf(' ', inicio);
    String palavra = (fim == -1) ? t.substring(inicio) : t.substring(inicio, fim);
    inicio = (fim == -1) ? t.length() : fim + 1;

    int extra = linha.length() ? 1 : 0;
    if ((int)(linha.length() + extra + palavra.length()) > MAX_COL) {
      imprimirLinha(linha, y);
      y += 9;
      linha = palavra;
    } else {
      if (extra) linha += " ";
      linha += palavra;
    }
  }
  imprimirLinha(linha, y);
  display.display();
}

void mostrarMensagem(const String& l1, const String& l2 = "", const String& l3 = "", const String& l4 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);  display.print(l1);
  display.setCursor(0, 12); display.print(l2);
  display.setCursor(0, 24); display.print(l3);
  display.setCursor(0, 36); display.print(l4);
  display.display();
}

// ---------------------------------------------------------------
//  2) AUTOMATO CELULAR: Jogo da Vida de Conway (arte)
//     Regras: viva com 2 ou 3 vizinhas sobrevive; morta com 3 nasce.
// ---------------------------------------------------------------
uint8_t grade[ALTURA][LARGURA];
uint8_t proxima[ALTURA][LARGURA];
int geracao = 0;
int ultimaPopulacao = -1;
int repeticoes = 0;
unsigned long ultimoQuadro = 0;

void semear() {
  for (int y = 0; y < ALTURA; y++)
    for (int x = 0; x < LARGURA; x++)
      grade[y][x] = (random(100) < 25) ? 1 : 0;   // 25% de celulas vivas
  geracao = 0;
  ultimaPopulacao = -1;
  repeticoes = 0;
}

int vizinhas(int x, int y) {
  int n = 0;
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      if (dx == 0 && dy == 0) continue;
      int xx = (x + dx + LARGURA) % LARGURA;   // bordas conectadas (mundo "toroidal")
      int yy = (y + dy + ALTURA) % ALTURA;
      n += grade[yy][xx];
    }
  }
  return n;
}

void passoVida() {
  int populacao = 0;
  for (int y = 0; y < ALTURA; y++) {
    for (int x = 0; x < LARGURA; x++) {
      int n = vizinhas(x, y);
      uint8_t viva = grade[y][x];
      uint8_t nova = (viva && (n == 2 || n == 3)) || (!viva && n == 3);
      proxima[y][x] = nova;
      populacao += nova;
    }
  }
  memcpy(grade, proxima, sizeof(grade));
  geracao++;

  // se a populacao parou de mudar (ou ficou velha demais), recomeca
  if (populacao == ultimaPopulacao) repeticoes++; else repeticoes = 0;
  ultimaPopulacao = populacao;
  if (repeticoes > 20 || geracao > 600 || populacao == 0) semear();
}

void desenharVida() {
  display.clearDisplay();
  for (int y = 0; y < ALTURA; y++)
    for (int x = 0; x < LARGURA; x++)
      if (grade[y][x]) display.drawPixel(x, y, SSD1306_WHITE);
  display.display();
}

// ---------------------------------------------------------------
//  3) PAGINA WEB
// ---------------------------------------------------------------
const char PAGINA[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Oráculo Generativo</title>
<style>
  body { font-family: sans-serif; background:#111; color:#eee; text-align:center; padding:20px; }
  h1 { font-weight:300; }
  #frase { min-height:80px; font-size:1.5em; margin:30px 10px; font-style:italic; }
  button { font-size:1.1em; padding:14px 22px; margin:6px; border:0; border-radius:10px;
           background:#3b82f6; color:white; }
  button:active { background:#1d4ed8; }
</style>
</head>
<body>
  <h1>Oráculo Generativo</h1>
  <div id="frase">Aperte o botão...</div>
  <button onclick="frase()">Nova frase</button>
  <button onclick="arte()">Arte (Jogo da Vida)</button>
<script>
  function frase() {
    fetch('/frase').then(r => r.text()).then(t => {
      document.getElementById('frase').textContent = t;
    });
  }
  function arte() {
    fetch('/arte').then(() => {
      document.getElementById('frase').textContent = 'Olhe o display...';
    });
  }
</script>
</body>
</html>
)rawliteral";

void rotaRaiz() {
  servidor.send_P(200, "text/html; charset=utf-8", PAGINA);
}

void rotaFrase() {
  modo = MODO_TEXTO;
  String f = gerarFrase();
  mostrarTexto(f);
  servidor.send(200, "text/plain; charset=utf-8", f);
}

void rotaArte() {
  modo = MODO_ARTE;
  semear();
  servidor.send(200, "text/plain; charset=utf-8", "ok");
}

// ---------------------------------------------------------------
//  SETUP / LOOP
// ---------------------------------------------------------------
void conectarWiFi() {
  mostrarMensagem("Conectando no WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_SENHA);
  unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - inicio < 20000) {
    delay(300);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    String ip = WiFi.localIP().toString();
    Serial.println("\nConectado! IP: " + ip);
    mostrarMensagem("WiFi OK! Abra no", "navegador:", "http://" + ip);
  } else {
    Serial.println("\nFalhou. Criando rede propria.");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_SENHA);
    String ip = WiFi.softAPIP().toString();
    mostrarMensagem("Sem WiFi. Conecte", "na rede:", String(AP_SSID) + " e abra", "http://" + ip);
  }
  delay(5000);   // tempo para voce anotar o IP
}

void setup() {
  Serial.begin(115200);
  Wire.begin(PINO_SDA, PINO_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, ENDERECO_OLED)) {
    Serial.println("OLED nao encontrado! Confira a ligacao (SDA=21, SCL=22) e o endereco 0x3C.");
    while (true) delay(1000);
  }

  randomSeed(esp_random());   // gerador de numeros aleatorios por hardware
  semear();

  conectarWiFi();

  servidor.on("/", HTTP_GET, rotaRaiz);
  servidor.on("/frase", HTTP_GET, rotaFrase);
  servidor.on("/arte", HTTP_GET, rotaArte);
  servidor.begin();

  mostrarTexto(gerarFrase());   // primeira frase ao ligar
}

void loop() {
  servidor.handleClient();

  if (modo == MODO_ARTE && millis() - ultimoQuadro > 80) {
    ultimoQuadro = millis();
    passoVida();
    desenharVida();
  }
}
