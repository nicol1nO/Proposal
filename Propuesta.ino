#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <EEPROM.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

// =====================================================
// ST7735
// =====================================================

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  6

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// =====================================================
// BOTONES
// =====================================================

#define BOTON_SI 7
#define BOTON_NO 8

// =====================================================
// DS1302
// =====================================================

#define RTC_DAT 3
#define RTC_CLK 2
#define RTC_RST 4

ThreeWire myWire(RTC_DAT, RTC_CLK, RTC_RST);
RtcDS1302<ThreeWire> rtc(myWire);

// =====================================================
// EEPROM
// =====================================================

#define EEPROM_MAGIC 0x4C415449UL
#define EEPROM_DIR 0

struct FechaInicio {
  uint32_t magic;
  uint16_t year;
  uint8_t month;
  uint8_t day;
};

FechaInicio fechaInicio;

// =====================================================
// COLORES
// =====================================================

#define NEGRO   ST77XX_BLACK
#define BLANCO  ST77XX_WHITE
#define ROJO    ST77XX_RED

// =====================================================
// CORAZÓN PIXEL ART
// =====================================================

const uint8_t corazonPequeno[] PROGMEM = {
  0b00011000,
  0b01111110,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00011000
};

// =====================================================
// DIBUJAR CORAZÓN PIXEL
// =====================================================

void dibujarCorazonPixel(
  int x,
  int y,
  const uint8_t *bitmap,
  int ancho,
  int alto,
  int escala
) {

  for (int fila = 0; fila < alto; fila++) {

    uint8_t datos = pgm_read_byte(&bitmap[fila]);

    for (int columna = 0; columna < ancho; columna++) {

      int bit = ancho - 1 - columna;

      if (datos & (1 << bit)) {

        tft.fillRect(
          x + columna * escala,
          y + fila * escala,
          escala,
          escala,
          ROJO
        );
      }
    }
  }
}

// =====================================================
// CORAZÓN GEOMÉTRICO
// Usado en celebración y pantalla NO
// =====================================================

void dibujarCorazon(int x, int y, int tamano) {

  tft.fillCircle(
    x - tamano / 2,
    y,
    tamano / 2,
    ROJO
  );

  tft.fillCircle(
    x + tamano / 2,
    y,
    tamano / 2,
    ROJO
  );

  tft.fillTriangle(
    x - tamano,
    y + 2,
    x + tamano,
    y + 2,
    x,
    y + tamano * 2,
    ROJO
  );
}

// =====================================================
// PANTALLA DE PREGUNTA
// =====================================================

void mostrarPregunta() {

  tft.fillScreen(NEGRO);

  // -------------------------
  // Corazón
  // -------------------------

  dibujarCorazonPixel(
    68,
    7,
    corazonPequeno,
    8,
    7,
    3
  );

  // -------------------------
  // Pregunta
  // -------------------------

  tft.setTextColor(BLANCO);
  tft.setTextSize(2);

  const char* linea1 = "QUERES SER";
  const char* linea2 = "MI NOVIA?";

  int16_t x1, y1;
  uint16_t w1, h1;
  uint16_t w2, h2;

  tft.getTextBounds(
    linea1,
    0,
    0,
    &x1,
    &y1,
    &w1,
    &h1
  );

  tft.getTextBounds(
    linea2,
    0,
    0,
    &x1,
    &y1,
    &w2,
    &h2
  );

  // Centrado automático

  int xLinea1 = (160 - w1) / 2;
  int xLinea2 = (160 - w2) / 2;

  tft.setCursor(xLinea1, 45);
  tft.print(linea1);

  tft.setCursor(xLinea2, 68);
  tft.print(linea2);

  // -------------------------
  // Botón SI
  // -------------------------

  int siX = 18;
  int siY = 96;

  int botonW = 52;
  int botonH = 25;

  tft.drawRoundRect(
    siX,
    siY,
    botonW,
    botonH,
    5,
    BLANCO
  );

  tft.setTextSize(1);

  tft.getTextBounds(
    "SI",
    0,
    0,
    &x1,
    &y1,
    &w1,
    &h1
  );

  tft.setCursor(
    siX + (botonW - w1) / 2,
    siY + (botonH - h1) / 2
  );

  tft.print("SI");

  // -------------------------
  // Botón NO
  // -------------------------

  int noX = 90;
  int noY = 96;

  tft.drawRoundRect(
    noX,
    noY,
    botonW,
    botonH,
    5,
    BLANCO
  );

  tft.getTextBounds(
    "NO",
    0,
    0,
    &x1,
    &y1,
    &w1,
    &h1
  );

  tft.setCursor(
    noX + (botonW - w1) / 2,
    noY + (botonH - h1) / 2
  );

  tft.print("NO");
}

// =====================================================
// CELEBRACIÓN
// =====================================================

void celebrar() {

  unsigned long inicio = millis();

  while (millis() - inicio < 5000) {

    // -------------------------
    // Corazón pequeño
    // -------------------------

    tft.fillScreen(NEGRO);

    dibujarCorazon(80, 30, 12);

    tft.setTextColor(BLANCO);
    tft.setTextSize(2);

    tft.setCursor(55, 75);
    tft.print("TE AMO");

    delay(250);

    // -------------------------
    // Corazón grande
    // -------------------------

    tft.fillScreen(NEGRO);

    dibujarCorazon(80, 25, 18);

    tft.setTextColor(BLANCO);
    tft.setTextSize(2);

    tft.setCursor(55, 75);
    tft.print("TE AMO");

    delay(250);
  }
}

// =====================================================
// RESPUESTA NO
// =====================================================

void respuestaNo() {

  tft.fillScreen(NEGRO);

  tft.setTextColor(BLANCO);
  tft.setTextSize(2);

  tft.setCursor(50, 30);
  tft.print("SEGURA?");

  dibujarCorazon(80, 65, 10);

  tft.setTextColor(BLANCO);
  tft.setTextSize(1);

  tft.setCursor(50, 105);
  tft.print("PENSA BIEN...");

  delay(2500);

  mostrarPregunta();
}

// =====================================================
// CALCULAR DÍAS
// =====================================================

long diasDesdeCivil(int year, int month, int day) {

  year -= month <= 2;

  long era =
    (year >= 0 ? year : year - 399) / 400;

  unsigned long yoe =
    (unsigned long)(year - era * 400);

  unsigned long doy =
    (153 * (month + (month > 2 ? -3 : 9)) + 2) / 5
    + day - 1;

  unsigned long doe =
    yoe * 365
    + yoe / 4
    - yoe / 100
    + doy;

  return era * 146097L + (long)doe;
}

long calcularDias(RtcDateTime ahora) {

  long diasActual =
    diasDesdeCivil(
      ahora.Year(),
      ahora.Month(),
      ahora.Day()
    );

  long diasInicio =
    diasDesdeCivil(
      fechaInicio.year,
      fechaInicio.month,
      fechaInicio.day
    );

  return diasActual - diasInicio;
}

// =====================================================
// GUARDAR FECHA DE INICIO
// =====================================================

void guardarFechaInicio(RtcDateTime fecha) {

  fechaInicio.magic = EEPROM_MAGIC;
  fechaInicio.year = fecha.Year();
  fechaInicio.month = fecha.Month();
  fechaInicio.day = fecha.Day();

  EEPROM.put(EEPROM_DIR, fechaInicio);
}

// =====================================================
// COMPROBAR FECHA GUARDADA
// =====================================================

bool existeFechaInicio() {

  EEPROM.get(EEPROM_DIR, fechaInicio);

  return fechaInicio.magic == EEPROM_MAGIC;
}

// =====================================================
// CONTADOR
// =====================================================

void mostrarContador() {

  RtcDateTime ahora = rtc.GetDateTime();

  long dias = calcularDias(ahora);

  tft.fillScreen(NEGRO);

  // -------------------------
  // Corazón
  // -------------------------

  dibujarCorazon(80, 8, 6);

  // -------------------------
  // LLEVAMOS
  // -------------------------

  tft.setTextColor(BLANCO);
  tft.setTextSize(2);

  tft.setCursor(43, 25);
  tft.print("LLEVAMOS");

  // -------------------------
  // Días
  // -------------------------

  tft.setTextSize(3);

  if (dias < 10) {

    tft.setCursor(71, 48);

  } else if (dias < 100) {

    tft.setCursor(62, 48);

  } else {

    tft.setCursor(53, 48);
  }

  tft.print(dias);

  // -------------------------
  // DIAS
  // -------------------------

  tft.setTextSize(1);

  tft.setCursor(72, 78);
  tft.print("DIAS");

  // -------------------------
  // DESDE
  // -------------------------

  tft.setCursor(48, 95);
  tft.print("DESDE:");

  tft.setCursor(51, 108);

  if (fechaInicio.day < 10) {
    tft.print("0");
  }

  tft.print(fechaInicio.day);
  tft.print("/");

  if (fechaInicio.month < 10) {
    tft.print("0");
  }

  tft.print(fechaInicio.month);
  tft.print("/");

  tft.print(fechaInicio.year);
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);

  // Botones

  pinMode(BOTON_SI, INPUT_PULLUP);
  pinMode(BOTON_NO, INPUT_PULLUP);

  // TFT horizontal

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  // RTC

  rtc.Begin();

  // -------------------------
  // Comprobar EEPROM
  // -------------------------

  if (existeFechaInicio()) {

    Serial.println("Fecha de inicio encontrada.");

    mostrarContador();

  } else {

    Serial.println("No existe fecha de inicio.");

    mostrarPregunta();
  }
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  // ===================================================
  // BOTÓN SI
  // ===================================================

  if (digitalRead(BOTON_SI) == LOW) {

    delay(50);

    if (digitalRead(BOTON_SI) == LOW) {

      Serial.println("SI presionado");

      RtcDateTime ahora = rtc.GetDateTime();

      guardarFechaInicio(ahora);

      Serial.print("Fecha guardada: ");
      Serial.print(ahora.Day());
      Serial.print("/");
      Serial.print(ahora.Month());
      Serial.print("/");
      Serial.println(ahora.Year());

      celebrar();

      mostrarContador();

      // Esperar a que se suelte el botón

      while (digitalRead(BOTON_SI) == LOW) {
        delay(10);
      }
    }
  }

  // ===================================================
  // BOTÓN NO
  // ===================================================

  if (digitalRead(BOTON_NO) == LOW) {

    delay(50);

    if (digitalRead(BOTON_NO) == LOW) {

      Serial.println("NO presionado");

      respuestaNo();

      // Esperar a que se suelte el botón

      while (digitalRead(BOTON_NO) == LOW) {
        delay(10);
      }
    }
  }

  // ===================================================
  // ACTUALIZAR CONTADOR
  // Cada minuto
  // ===================================================

  if (fechaInicio.magic == EEPROM_MAGIC) {

    static unsigned long ultimaActualizacion = 0;

    if (millis() - ultimaActualizacion >= 60000) {

      ultimaActualizacion = millis();

      mostrarContador();
    }
  }
}