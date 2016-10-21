#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

//#define veces 10    //Veces que se enciendes los leds en el juego
#define RESTART asm("jmp 0x0000")

#define tmaxenc 4000
#define tminenc 500
#define tmaxapa 4000
#define tminapa 500

const int led1 = 22;  //Salida 22 para el led 1
const int led2 = 23;  //Salida 23 para el led 2
const int led3 = 24;  //Salida 24 para el led 3
const int led4 = 25;  //Salida 25 para el led 4
const int led5 = 26;  //Salida 26 para el led 5

const int boton1 = 28;
const int boton2 = 29;
const int boton3 = 30;
const int boton4 = 31;
const int boton5 = 32;

const int on_off = 8; //boton para empezar otra vez el juego con los mismos parámetros

const byte Fils = 4;          //KeyPad de 4 filas
const byte Cols = 4;           //y 4 columnas

byte Pins_Filas[] = {7, 6, 5, 4};     //Pines Arduino para las filas.
byte Pins_Cols[] = {3, 2, 10, 9 };     // Pines Arduino para las columnas.

char Teclas [ Fils ][ Cols ] =
    {
        {'D','C','B','A'},
        {'#','9','6','3'},
        {'0','8','5','2'},
        {'*','7','4','1'}
     };

Keypad Teclado1 = Keypad(makeKeymap(Teclas), Pins_Filas, Pins_Cols, Fils, Cols);

int encender = 0;                 // Si está encendido el LED
int anterior = 0;                 // Pulsación anterior
int estado = 0;                   //El estado actual del botón
int buenos = 0;   //almacena las veces que he apagado el led antes de que pase el tiempo de encendido
int malos = 0;    //almacena las veces que NO he apagado el led antes de que pase el tiempo de encendido
int ver = 0; // la variable ver la utilizo para guardar el número de veces que se encienden los leds

int h = 0;  //guardamos en esta variable el led que se va a encender
int i = 0;  //guardamos en esta variable el led que se va a encender       
int j = 0;  //guardamos en esta variable el led que se va a encender
int k = 0;  //guardamos en esta variable el led que se va a encender

int option;   
int option2;
int option3;
int option4;

//unsigned long TIEMPOENCENDIDO=1500;
//unsigned long TIEMPOAPAGADO = 500;
float tiempT;


int leds=0;
int tleds = 0;
int taleds = 0;
int reset = 0;
int ledsim = 0;

char pulsacion;
char pulsacion2;
char pulsacion3;
char pulsacion4;

void setup()
{
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(boton3, INPUT);
  pinMode(boton4, INPUT);
  pinMode(boton5, INPUT);
  pinMode(on_off, INPUT);
  
  //Iniciamos el fondo retroiluminado
  lcd.backlight();
  //Iniciamos la pantalla
  lcd.init(); 
  
  lcd.setCursor(0,0);
  lcd.print ("ENTRENADOR DE");
  lcd.setCursor(0,1);
  lcd.print ("ESGRIMA");
  delay (2000);
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print ("Elige juego");
  lcd.setCursor(0,1);
  lcd.print ("1,2,3");
}

void loop()
{
 
    pulsacion = Teclado1.getKey() ;  //Esta función devuleve un char y no es bloqueante (en cada loop comprueba pulsación) http://playground.arduino.cc/Code/Keypad#Functions
    if (pulsacion != NO_KEY)
    {
      option = pulsacion - 48;   ////le restamos 48 que es el '0' en ASCII para que me de un entero en vez de un caracter
      Serial.println(option);
    }
    leds = 0;	//en cada loop inicalizo esta variable numero de leds
    tleds = 0;	//Tiempo leds
    if (option >= 1 && option <= 3)
    {
      switch (option) 
      {
        case 1:
          lcd.clear();
          lcd.print ("Juego 1");
          delay (2000);

          //MENU PARA EL NUMERO DE LEDS QUE SE QUIEREN ENCENDER EN EL JUEGO
          lcd.clear();
          lcd.print("NUMERO DE LEDS");
          lcd.setCursor(0,1);
          lcd.print("(1-15)");
          lcd.setCursor(9,1);
          do
          {
            pulsacion = Teclado1.getKey(); //Esta función no bloquea, si queremos esperar usar waitForKey()o bucle como este
          }while (pulsacion == NO_KEY || (pulsacion != '0' && pulsacion != '1')); //Eperaremos pulsacion hasta que se pulse un 0 o un 1

          option = pulsacion - 48;
          lcd.print(option);
          lcd.setCursor(10,1);
          Serial.println(option);
          Serial.println("Primer if");
          do
          {
            pulsacion2 = Teclado1.getKey(); //Esperamos la segunda pulsación en el teclado que tiene que ser 0,1,2,3,4 o 5
          }while (pulsacion2 == NO_KEY || (pulsacion2 != '0' && pulsacion2 != '1' && pulsacion2 != '2' && pulsacion2 != '3' && pulsacion2 != '4' && pulsacion2 != '5'));
		  
		  //seria mejor opcion leer la tecla pulsada y si no es la esperada sacar por LCD  "tecla incorrecta"

          option2 = pulsacion2 - 48; //le restamos 48 que es el '0' en ASCII para que me de un entero en vez de un caracter
          lcd.print(option2);

          leds = (option*10) + option2; //Multiplicamos option*10 para que sean decenas y le sumamos las option2 como unidades
          delay(3000);
      
          // MENU PARA EL TIEMPO QUE QUEREMOS QUE ESTEN ENCENDIDOS LOS LEDS
          lcd.clear();
          lcd.print("T. ENCENDI. LEDS");
          lcd.setCursor(0,1);
          lcd.print("(en ms)");
          lcd.setCursor(9,1);
          
          do
          {
            pulsacion = Teclado1.getKey();
          }while (pulsacion == NO_KEY || (pulsacion != '0' && pulsacion != '1' && pulsacion != '2' && pulsacion != '3' && pulsacion != '4'));
          option = pulsacion - 48;
          lcd.print(option);
          lcd.setCursor(10,1);
          
          if (pulsacion == '0')
          {
            do
            {
              pulsacion2 = Teclado1.getKey();
            }while (pulsacion2 == NO_KEY || (pulsacion2 != '5' && pulsacion2 != '6' && pulsacion2 != '7' && pulsacion2 != '8' && pulsacion2 != '9'));
          }
          else
          {
            while ( (pulsacion2 = Teclado1.getKey() ) == NO_KEY);
          }
          
          option2 = pulsacion2 - 48;
          lcd.print(option2);
          lcd.setCursor(11,1);
          
          while ( (pulsacion3 = Teclado1.getKey() ) == NO_KEY);
          option3 = pulsacion3 - 48;
          lcd.print(option3);
          lcd.setCursor(12,1);
          
          while ( (pulsacion4 = Teclado1.getKey() ) == NO_KEY);
          option4 = pulsacion4 - 48;
          lcd.print(option4);
          
          tleds = (option*1000)+(option2*100)+(option3*10)+option4;
          delay(3000);

          // MENU PARA EL TIEMPO QUE QUEREMOS QUE ESTEN APAGADOS LOS LEDS
          lcd.clear();
          lcd.print("T. APAGADO. LEDS");
          lcd.setCursor(0,1);
          lcd.print("(en ms)");
          lcd.setCursor(9,1);
          
          do
          {
            pulsacion = Teclado1.getKey();
          }while (pulsacion == NO_KEY || (pulsacion != '0' && pulsacion != '1' && pulsacion != '2' && pulsacion != '3' && pulsacion != '4'));
          option = pulsacion - 48;
          lcd.print(option);
          lcd.setCursor(10,1);
          
          if (pulsacion == '0')
          {
            do
            {
              pulsacion2 = Teclado1.getKey();
            }while (pulsacion2 == NO_KEY || (pulsacion2 != '5' && pulsacion2 != '6' && pulsacion2 != '7' && pulsacion2 != '8' && pulsacion2 != '9'));
          }
          else
          {
            while ( (pulsacion2 = Teclado1.getKey() ) == NO_KEY);
          }
          option2 = pulsacion2 - 48;
          lcd.print(option2);
          lcd.setCursor(11,1);
          
          while ( (pulsacion3 = Teclado1.getKey() ) == NO_KEY);
          option3 = pulsacion3 - 48;
          lcd.print(option3);
          lcd.setCursor(12,1);
          
          while ( (pulsacion4 = Teclado1.getKey() ) == NO_KEY);
          option4 = pulsacion4 - 48;
          lcd.print(option4);
          
          taleds = (option*1000)+(option2*100)+(option3*10)+option4;
          delay(3000);
          
          //LLAMADA A LA FUNCION ENTRENAMIENTO1
          entrenamiento1(leds, tleds, taleds);
          break;
---------------        
        case 2:
          lcd.clear();
          lcd.print ("Juego 2");
          delay (2000);
          Serial.println("Esto esperando");
          //MENU PARA EL NUMERO DE LEDS QUE SE QUIEREN ENCENDER EN EL JUEGO
          lcd.clear();
          lcd.print("NUMERO DE LEDS");
          lcd.setCursor(0,1);
          lcd.print("(1-15)");
          lcd.setCursor(9,1);
//          Serial.print ("leds ");
//          Serial.println(leds);
          
          do
          {
            if (Serial.available()>0)
            {
              leds = Serial.parseInt();
              if (leds >= 1 && leds <= 15)
              {
                //leds -= '0';
//                Serial.print ("Numero de leds ");
//                Serial.println(leds);
//                lcd.setCursor(0,1);
                lcd.print (leds);
                delay(3000);
//                ledsb = leds;
              }
            }
          }while (leds < 1 || leds > 15);

          //MENU PARA EL NUMERO DE LEDS QUE SE QUIEREN ENCENDER SIMULTANEAMENTE EN EL JUEGO
          lcd.clear();
          lcd.print("NUMERO LED SIMUL");
          lcd.setCursor(0,1);
          lcd.print("(2-4)");
          lcd.setCursor(9,1);
//          Serial.print ("leds ");
//          Serial.println(leds);
          
          do
          {
            if (Serial.available()>0)
            {
              ledsim = Serial.parseInt();
              if (ledsim >= 2 && ledsim <= 4)
              {
                //leds -= '0';
//                Serial.print ("Numero de leds ");
//                Serial.println(leds);
//                lcd.setCursor(0,1);
                lcd.print (ledsim);
                delay(3000);
//                ledsb = leds;
              }
            }
          }while (ledsim < 2 || ledsim > 4);
     
          // MENU PARA EL TIEMPO QUE QUEREMOS QUE ESTEN ENCENDIDOS LOS LEDS
          lcd.clear();
          lcd.print("T. ENCENDI. LEDS");
          lcd.setCursor(0,1);
          lcd.print("(en ms)");
          lcd.setCursor(9,1);
          
//          Serial.print ("tiempo encendido de los leds ");
//          Serial.println(tleds);
          
          do
          {
            if (Serial.available()>0)
            {
              tleds = Serial.parseInt();
              if (tleds >= tminenc && tleds <= tmaxenc)
              {
                //leds -= '0';
//                Serial.print ("TIEMPO DE LOS LEDS ");
//                Serial.println(tleds);
//                lcd.setCursor(9,1);
                lcd.print (tleds);
                lcd.print (" ms");
                delay(3000);
//                tledsb = tleds;
              }
            }
          }while (tleds < tminenc || tleds > tmaxenc);

          // MENU PARA EL TIEMPO QUE QUEREMOS QUE ESTEN APAGADOS LOS LEDS
          lcd.clear();
          lcd.print("T. APAGADO. LEDS");
          lcd.setCursor(0,1);
          lcd.print("(en ms)");
          lcd.setCursor(9,1);
          
//          Serial.print ("tiempo encendido de los leds ");
//          Serial.println(tleds);
          
          do
          {
            if (Serial.available()>0)
            {
              taleds = Serial.parseInt();
              if (taleds >= tminapa && taleds <= tmaxapa)
              {
                //leds -= '0';
//                Serial.print ("TIEMPO DE LOS LEDS ");
//                Serial.println(tleds);
//                lcd.setCursor(9,1);
                lcd.print (taleds);
                lcd.print (" ms");
                delay(3000);
//                taledsb = taleds;
              }
            }
          }while (taleds < tminapa || taleds > tmaxapa);
          
          Serial.print("Num. de leds ");
          Serial.println(leds);
          Serial.print("Tiempo encendido los leds ");
          Serial.println(tleds);
          Serial.print("Tiepo apagados los leds ");
          Serial.println(taleds);
          entrenamiento2(leds, ledsim, tleds, taleds);
          break;
         case 3:
          lcd.clear();
          lcd.print ("Juego 3");
          break;
      }
    }
  //}
}

void entrenamiento1(int veces, unsigned long TIEMPOENCENDIDO, unsigned long TIEMPOAPAGADO)
{
  lcd.clear();
  randomSeed(analogRead(0));  //para generar una nueva semilla para la funcion random
  while (1)
  {   
    estado = digitalRead(on_off);    //Comprobamos el estado
  
  //  Serial.print
    if (estado && anterior == 0)     // Comprobamos si el anterior era sin pulsar y el estado actual es pulsado
    {
  
      encender = 1 - encender;       // Asignamos el estado contrario
      delay(30);                     // Anti-rebote
      ver = 0;  //contador para saber si he llegado al numero de leds a encender que le paso a la funcion.
      buenos = 0;
      malos = 0;
  //    lcd.clear();
      tiempT = 0;
    }
    anterior = estado;    
    if(encender && ver < veces)
    {
        i = random(22,27);  //El numero aleatorio entre el 22 y el 26 correspondiente a la salida digital de los led
        unsigned long tiempoInicio=millis();
        unsigned long tiempoTranscurrido;
        digitalWrite(i, HIGH); //Encendemos el led correspondiente al numero aleatorio que ha salido
        switch (i)
          {
            case 22:
            //Se enciende el led 1 y esperamos a que sea apagado con el boton asociado a ese led o a que pase el tiempo de estar encendido
              do 
               {
                 tiempoTranscurrido=millis()-tiempoInicio; //contamos el tiempo que nos ha costado apagar el led
                 if (digitalRead(boton1)) 
                 {
                    break;
                 }
               } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
               break;
            case 23:
              do 
               {
                 tiempoTranscurrido=millis()-tiempoInicio;
                 if (digitalRead(boton2)) 
                 {
                  break;
                 }
               } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
               break;
            case 24:
             do 
              {
                tiempoTranscurrido=millis()-tiempoInicio;
                if (digitalRead(boton3)) 
                {
                 break;
                }
              }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
              break;
            case 25:
             do 
              {
                tiempoTranscurrido=millis()-tiempoInicio;
                if (digitalRead(boton4)) 
                {
                 break;
                }
              }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
              break;
            case 26:
             do 
              {
                tiempoTranscurrido=millis()-tiempoInicio;
                if (digitalRead(boton5)) 
                {
                 break;
                }
              }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
              break;
          }
        digitalWrite(i, LOW); //apagamos el led correspondiente segun el número aleatorio
        if(tiempoTranscurrido<TIEMPOENCENDIDO) //He apagado el led antes de que pase el tiempo de encendido, por lo tanto ha sido bueno
        {
          buenos++;
          Serial.print("Pulsado boton tras ");
          Serial.print(tiempoTranscurrido);
          Serial.println(" milisegundos");
          tiempT = tiempT + tiempoTranscurrido;
        } 
        else // No he apagado el led antes del tiempo de encendido por lo tanto ha sido malo
        {
          malos++;
          tiempT = tiempT + TIEMPOENCENDIDO;
          Serial.println("Transcurrieron los dos segundos sin pulsar.");
        }
        ver++;
        delay(TIEMPOAPAGADO);
    }
    else  //Muestro los resultados del juego en pantalla
    {
      while (estado == LOW)
      {
//        Serial.println("ESTOY EN EL WHILE DEL ELSE");
        estado = digitalRead(on_off);    //Comprobamos el estado
        digitalWrite(i,LOW);  
  
        lcd.setCursor(0,0);
        lcd.print ("Leds: ");
        lcd.setCursor (5,0);
        lcd.print (ver);
        lcd.setCursor (8,0);
        lcd.print ("T: ");
        lcd.setCursor(10,0);
        lcd.print(tiempT/1000);
        lcd.setCursor(0,1);
        lcd.print("Bue. ");
        lcd.setCursor (5,1);
        lcd.print(buenos);
        lcd.setCursor(8,1);
        lcd.print("Mal: ");
        lcd.setCursor (13,1);
        lcd.print(malos);

 
          pulsacion = Teclado1.getKey(); //Esperamos la primera pulsacion en el teclado
          if (pulsacion == '#')
            RESTART;

        
//        if (Serial.available()>0)
//        {
//          reset = Serial.parseInt();  
//          if (reset == 0)
//            RESTART;
//        }
      }
    }  
  }  
}

void entrenamiento2(int veces, int ledsim, unsigned long TIEMPOENCENDIDO, unsigned long TIEMPOAPAGADO)
{
    Serial.print ("VECES ");
    Serial.println (veces);
    Serial.print ("LEDS SIMULTANEOS ");
    Serial.println (ledsim);
    Serial.print ("TIEMPO ENCENDIDO ");
    Serial.println (TIEMPOENCENDIDO);
    Serial.print ("TIEMPO APAGADO ");
    Serial.println(TIEMPOAPAGADO);
   
    
    lcd.clear();
    randomSeed(analogRead(0));  //para generar una nueva semilla para la funcion random

  while (1)
  {   
    estado = digitalRead(on_off);    //Comprobamos el estado
  
    if (estado && anterior == 0)     // Comprobamos si el anterior era sin pulsar y el estado actual es pulsado
    {
  
      encender = 1 - encender;       // Asignamos el estado contrario
      delay(30);                     // Anti-rebote
      ver = 0;
      buenos = 0;
      malos = 0;
  //    lcd.clear();
      tiempT = 0;
    }
    anterior = estado;    
    if(encender && ver < veces)
    {
      switch (ledsim)
        {
          case 2: // DOS ENCENDIDOS SIMULTANEOS
              Serial.println ("2 SIMULTANEOS ");
            i = random(22,27);
            Serial.print ("RANDOM i ");
            Serial.println (i);
            do
              j = random(22,27);  
            while (j==i);
            Serial.print ("RANDOM j ");
            Serial.println (j);
            break;
          case 3:  // TRES ENCENDIDOS SIMULTANEOS
          Serial.println ("3 SIMULTANEOS ");
            i = random(22,27);
            Serial.print ("RANDOM i ");
            Serial.println (i);
            do 
              j = random(22,27);
            while (j==i);
            Serial.print ("RANDOM j ");
            Serial.println (j);
            do
              k = random(22,27);
            while (k == i || k == j);
            Serial.print ("RANDOM k ");
            Serial.println (k);
            break;
          case 4:  // CUATRO ENCENDIDOS SIMULTANEOS
          Serial.println ("4 SIMULTANEOS ");
             i = random(22,27);
             Serial.print ("RANDOM i ");
            Serial.println (i);
            do 
              j = random(22,27);
            while (j==i);
            Serial.print ("RANDOM j ");
            Serial.println (j);
            do
              k = random(22,27);
            while (k == i || k == j);
            Serial.print ("RANDOM k ");
            Serial.println (k);
            do 
              h = random(22,27);
            while (h == i || h == j || h == k);
            Serial.print ("RANDOM h ");
            Serial.println (h);
            break;
        }
      
        unsigned long tiempoInicio=millis();
        unsigned long tiempoTranscurrido;
        switch (ledsim)
          {
            case 2:
            {
              digitalWrite(i, HIGH);
              digitalWrite(j, HIGH);
              switch (i)
              {
                case 22:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton1)) 
                     {
                        break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 23:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton2)) 
                     {
                      break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 24:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton3)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 25:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton4)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 26:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton5)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
              }
              digitalWrite(i, LOW);
              if(tiempoTranscurrido<TIEMPOENCENDIDO) 
              {
                buenos++;
                Serial.print("Pulsado boton tras ");
                Serial.print(tiempoTranscurrido);
                Serial.println(" milisegundos");
                tiempT = tiempT + tiempoTranscurrido;
              } 
              else 
              {
                malos++;
                tiempT = tiempT + TIEMPOENCENDIDO;
                Serial.println("Transcurrieron los dos segundos sin pulsar.");
              }

                
//              digitalWrite(j, HIGH);
              switch (j)
              {
                case 22:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton1)) 
                     {
                        break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 23:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton2)) 
                     {
                      break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 24:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton3)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 25:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton4)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 26:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton5)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
              }
//              digitalWrite(i, LOW);
              digitalWrite(j, LOW);
              if(tiempoTranscurrido<TIEMPOENCENDIDO) 
              {
                buenos++;
                Serial.print("Pulsado boton tras ");
                Serial.print(tiempoTranscurrido);
                Serial.println(" milisegundos");
                tiempT = tiempT + tiempoTranscurrido;
              } 
              else 
              {
                malos++;
                tiempT = tiempT + TIEMPOENCENDIDO;
                Serial.println("Transcurrieron los dos segundos sin pulsar.");
              }
            }
            
            break;
            
            
            case 3:
            {
              digitalWrite(i, HIGH);
              digitalWrite(j, HIGH);
              digitalWrite(k, HIGH);
              switch (i)
              {
                case 22:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton1)) 
                     {
                        break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 23:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton2)) 
                     {
                      break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 24:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton3)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 25:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton4)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 26:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton5)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
              } 
//              digitalWrite(i, LOW);
              if(tiempoTranscurrido<TIEMPOENCENDIDO) 
              {
                buenos++;
                Serial.print("Pulsado boton tras ");
                Serial.print(tiempoTranscurrido);
                Serial.println(" milisegundos");
                tiempT = tiempT + tiempoTranscurrido;
              } 
              else 
              {
                malos++;
                tiempT = tiempT + TIEMPOENCENDIDO;
                Serial.println("Transcurrieron los dos segundos sin pulsar.");
              }
               
//              digitalWrite(j, HIGH);
              switch (j)
              {
                case 22:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton1)) 
                     {
                        break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 23:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton2)) 
                     {
                      break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 24:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton3)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 25:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton4)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 26:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton5)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
              }
//              digitalWrite(j, LOW);
              if(tiempoTranscurrido<TIEMPOENCENDIDO) 
              {
                buenos++;
                Serial.print("Pulsado boton tras ");
                Serial.print(tiempoTranscurrido);
                Serial.println(" milisegundos");
                tiempT = tiempT + tiempoTranscurrido;
              } 
              else 
              {
                malos++;
                tiempT = tiempT + TIEMPOENCENDIDO;
                Serial.println("Transcurrieron los dos segundos sin pulsar.");
              }



              
//              digitalWrite(k, HIGH);
              switch (k)
              {
                case 22:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton1)) 
                     {
                        break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 23:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton2)) 
                     {
                      break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 24:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton3)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 25:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton4)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 26:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton5)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
              }
              digitalWrite(i, LOW);
              digitalWrite(j, LOW);
              digitalWrite(k, LOW);
              if(tiempoTranscurrido<TIEMPOENCENDIDO) 
              {
                buenos++;
                Serial.print("Pulsado boton tras ");
                Serial.print(tiempoTranscurrido);
                Serial.println(" milisegundos");
                tiempT = tiempT + tiempoTranscurrido;
              } 
              else 
              {
                malos++;
                tiempT = tiempT + TIEMPOENCENDIDO;
                Serial.println("Transcurrieron los dos segundos sin pulsar.");
              }
            }
            break;
            case 4:
            {
              digitalWrite(i, HIGH);
              digitalWrite(j, HIGH);
              digitalWrite(k, HIGH);
              digitalWrite(h, HIGH);
              switch (i)
              {
                case 22:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton1)) 
                     {
                        break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 23:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton2)) 
                     {
                      break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 24:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton3)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 25:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton4)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 26:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton5)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
              } 
//              digitalWrite(i, LOW);
              if(tiempoTranscurrido<TIEMPOENCENDIDO) 
              {
                buenos++;
                Serial.print("Pulsado boton tras ");
                Serial.print(tiempoTranscurrido);
                Serial.println(" milisegundos");
                tiempT = tiempT + tiempoTranscurrido;
              } 
              else 
              {
                malos++;
                tiempT = tiempT + TIEMPOENCENDIDO;
                Serial.println("Transcurrieron los dos segundos sin pulsar.");
              }
               
//              digitalWrite(j, HIGH);
              switch (j)
              {
                case 22:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton1)) 
                     {
                        break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 23:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton2)) 
                     {
                      break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 24:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton3)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 25:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton4)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 26:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton5)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
              }
//              digitalWrite(j, LOW);
              if(tiempoTranscurrido<TIEMPOENCENDIDO) 
              {
                buenos++;
                Serial.print("Pulsado boton tras ");
                Serial.print(tiempoTranscurrido);
                Serial.println(" milisegundos");
                tiempT = tiempT + tiempoTranscurrido;
              } 
              else 
              {
                malos++;
                tiempT = tiempT + TIEMPOENCENDIDO;
                Serial.println("Transcurrieron los dos segundos sin pulsar.");
              }


              
//              digitalWrite(k, HIGH);
              switch (k)
              {
                case 22:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton1)) 
                     {
                        break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 23:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton2)) 
                     {
                      break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 24:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton3)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 25:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton4)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 26:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton5)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
              }

//              digitalWrite(k, LOW);
              if(tiempoTranscurrido<TIEMPOENCENDIDO) 
              {
                buenos++;
                Serial.print("Pulsado boton tras ");
                Serial.print(tiempoTranscurrido);
                Serial.println(" milisegundos");
                tiempT = tiempT + tiempoTranscurrido;
              } 
              else 
              {
                malos++;
                tiempT = tiempT + TIEMPOENCENDIDO;
                Serial.println("Transcurrieron los dos segundos sin pulsar.");
              }

              
//              digitalWrite(h, HIGH);
              switch (h)
              {
                case 22:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton1)) 
                     {
                        break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 23:
                  do 
                   {
                     tiempoTranscurrido=millis()-tiempoInicio;
                     if (digitalRead(boton2)) 
                     {
                      break;
                     }
                   } while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                   break;
                case 24:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton3)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 25:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton4)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
                case 26:
                 do 
                  {
                    tiempoTranscurrido=millis()-tiempoInicio;
                    if (digitalRead(boton5)) 
                    {
                     break;
                    }
                  }  while(tiempoTranscurrido<=TIEMPOENCENDIDO);
                  break;
              }
              digitalWrite(i, LOW);
              digitalWrite(j, LOW);
              digitalWrite(k, LOW);
              digitalWrite(h, LOW);
              if(tiempoTranscurrido<TIEMPOENCENDIDO) 
              {
                buenos++;
                Serial.print("Pulsado boton tras ");
                Serial.print(tiempoTranscurrido);
                Serial.println(" milisegundos");
                tiempT = tiempT + tiempoTranscurrido;
              } 
              else 
              {
                malos++;
                tiempT = tiempT + TIEMPOENCENDIDO;
                Serial.println("Transcurrieron los dos segundos sin pulsar.");
              }

              
            }
            break;
          }

        ver++;
        delay(TIEMPOAPAGADO);
    }
    else
    {
      while (estado == LOW)
      {
        Serial.println("ESTOY EN EL WHILE DEL ELSE");
        estado = digitalRead(on_off);    //Comprobamos el estado
        digitalWrite(i,LOW);  
  
        lcd.setCursor(0,0);
        lcd.print ("Leds: ");
        lcd.setCursor (5,0);
        lcd.print (ver);
        lcd.setCursor (8,0);
        lcd.print ("T: ");
        lcd.setCursor(10,0);
        lcd.print(tiempT/1000);
        lcd.setCursor(0,1);
        lcd.print("Bue. ");
        lcd.setCursor (5,1);
        lcd.print(buenos);
        lcd.setCursor(8,1);
        lcd.print("Mal: ");
        lcd.setCursor (13,1);
        lcd.print(malos);
        if (Serial.available()>0)
        {
          reset = Serial.parseInt();  
          if (reset == 0)
            RESTART;
        }
      }
    }  
  }  
}

