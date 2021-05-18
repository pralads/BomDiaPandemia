/** 
 * Programa controlador do display MAX7219
 * @author Prahlada Hargreaves
 * @author Artur Cabral
 * @author Leandro Caju Ferreira
 * @data 20/04/2021
 */

#include <MD_MAXPanel.h>
#include <string.h>     // std::string, std::to_string
#include "bitmaps.h"    // aba de imagens
#include "tags.h"       // aba de assinaturas

const MD_MAX72XX::moduleType_t HARDWARE_TYPE = MD_MAX72XX::PAROLA_HW;
const uint8_t X_DEVICES = 3; //quantidade de telas 8x8 na horizontal
const uint8_t Y_DEVICES = 3; //quantidade de telas 8x8 na vertical

// SPI hardware interface
const uint8_t CLK_PIN = 13;   // or SCK
const uint8_t DATA_PIN = 11;  // or MOSI
const uint8_t CS_PIN = 10;    // or SS

long numerocontagem = 1000000000;
long const SEGUNDOSREINICIO = 3600;
long const LIMITEREINICIO1 =  1000002523 ; //escolha aqui o numero final, média móvel atual (2523)
long const LIMITEREINICIO2 =  1000004249 ; //escolha aqui o numero final, recorde atual (4249)
long const LIMITEREINICIO3 =  1000406565 ; //escolha aqui o numero final, total de mortos (406565)

MD_MAXPanel mp = MD_MAXPanel(HARDWARE_TYPE, CS_PIN, X_DEVICES, Y_DEVICES);
// We always wait a bit between updates of the display
#define  DELAYTIME  1 // in milliseconds 

/**
 * Mapeia a tela em linhas e colunas
 */
void text (MD_MAX72XX::fontType_t *fontData,long numero)
{
   mp.setFont(fontData);
   String numerostring = String(numero);
   mp.clear();
   mp.drawText (16, mp.getYMax()-16, String(numerostring.charAt(9)).c_str(), MD_MAXPanel::ROT_0);
   mp.drawText (8, mp.getYMax()-16, String(numerostring.charAt(8)).c_str(), MD_MAXPanel::ROT_0);
   mp.drawText (0, mp.getYMax()-16,  String(numerostring.charAt(7)).c_str(), MD_MAXPanel::ROT_0);
   mp.drawText (16, mp.getYMax()-8, String(numerostring.charAt(6)).c_str(), MD_MAXPanel::ROT_0);
   mp.drawText (8, mp.getYMax()-8, String(numerostring.charAt(5)).c_str(), MD_MAXPanel::ROT_0);
   mp.drawText (0, mp.getYMax()-8, String(numerostring.charAt(4)).c_str(), MD_MAXPanel::ROT_0);
   mp.drawText (16, mp.getYMax(), String(numerostring.charAt(3)).c_str(), MD_MAXPanel::ROT_0);
   mp.drawText (8, mp.getYMax(),  String(numerostring.charAt(2)).c_str(), MD_MAXPanel::ROT_0);
   mp.drawText (0, mp.getYMax(),  String(numerostring.charAt(1)).c_str(), MD_MAXPanel::ROT_0);
   delay(1); // em milisegundos
   mp.clear();

}
/**
 * Organiza a tela em 24 linhas, 3 colunas e 8 estados
 **/
void digBitmap (byte bitmap[24][3]) 
{
   mp.setRotation (mp.getRotation() == MD_MAXPanel::ROT_90 ? MD_MAXPanel::ROT_90 : MD_MAXPanel::ROT_90);
    for (int i =0; i < 24; i++) 
    {
      for (int b = 1; b < 9; b++) {
          mp.setPoint (i,(((1 * b)/1)+0)-1, bitRead(bitmap[i][1-1], ((b-1) - 7)*(-1)));
      }
      for (int b = 1; b < 9; b++) {
          mp.setPoint (i,(((2 * b)/2)+8)-1, bitRead(bitmap[i][2-1],((b-1) - 7)*(-1)));
      }
      for (int b = 1; b < 9; b++) {
          mp.setPoint (i,(((3 * b)/3)+16)-1, bitRead(bitmap[i][3-1],((b-1) - 7)*(-1)));
      }
   }
   mp.setRotation (MD_MAXPanel::ROT_0);
}
/**
 * valor 100 (0,100) pode ser substituído 
 * para aumentar/diminuir 
 * a frequência das imagens 
 */
void DesenhaNumeroloop (const long limite) {
  long contadorAux = 1000000000;
  while ( contadorAux <= limite  && numerocontagem <= limite){
    text ( nullptr,numerocontagem );
    delay (100);
    if ( random (0, 100) < 2 ){  
      imprimeImagemAleatoria();
      delay (1500);      // trocar e definir melhor valor
    }
    
    numerocontagem++;
    contadorAux++;
  }
}
/**
 * Seleciona e imprime um dos cases (bitmaps) 
 * em momentos randomicos
 * Utiliza o resto da equação 
 * para escolher o case
 */
void imprimeImagemAleatoria () {
    int selecao = (int) random (0,100)%11;
    switch (selecao) {
      case 0:
      digBitmap (tagskull);
      break;
      case 1:
      digBitmap (skullinvert);
      break;
      case 2:
//      digBitmap (tagbolso);
//      break;
//      case 3:
      digBitmap (genocida);
      break;
      case 4:
      digBitmap (probespo);
      break;
      case 5:
      digBitmap (falota);
      break;
      case 6:
      digBitmap (carpento);
      break;
      case 7:
      digBitmap (graspilo);
      break;
      case 8:
      digBitmap (girulento);
      break;
      case 9:
      digBitmap (crocho);
      break;
      case 10:
      digBitmap (castonheto);
      break;
      case 11:
      digBitmap (trestassudo);
      break;
    }

}
/**
 * Imprime as TAG dos participantes da obra 
 * tag prah, tag artur e tag caju
 * intervala com delay as imagens
 */
void imprimeCabecalho()
{ 
    digBitmap(tagprah);
    delay(1300);
    digBitmap(tagartur);
    delay(1300);
    digBitmap(tagcaju);
    delay(1300);
}

void setup(void) 
{
  mp.begin();
  mp.clear();
 imprimeCabecalho();
}

void loop(void)  
{
  delay(500);
  digBitmap (total);  //trocar pra cada programa
  delay(2500);
  DesenhaNumeroloop(LIMITEREINICIO3); // escolhe o numero a contar
}
