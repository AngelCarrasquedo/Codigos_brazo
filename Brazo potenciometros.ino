/*
 * RECUERDA SOMOS TEAM TESLA PASIÓN POR LA ELECTRÓNICA Y LA ROBÓTICA 
 * No olvides apoyar el canal y suerte con sus proyectos.
 * https://www.youtube.com/channel/UCkWI_-8ihjyzkn00COgXTaA/videos
 * ¿Dudas acerca de este video? No dudes en contactarnos  
 * https://www.facebook.com/roboitica 
 */

#include <Servo.h>
                          
int Grabar = 9;                                                                                                             //Pulsadores asociados a los pines de la placa
int Reproducir = 11; 
int Resetear = 10; 

int servoPinBase = 3;                                                                                                      //Servomotores asiaciados a cada pin
int servoPinHombro =4;
int servoPinHombro2 =5;
int servoPinCodo = 6;
int servoPinMuneca = 7;
int servoPinPinza = 2;

int POTBase = A1;                                                                                                            //Potenciómetros asciados
int POTHombro=A2;
int POTCodo=A3;
int POTMuneca=A4;
int POTPinza=A5;

int MovGuardado[20][5];                                                                                                      //Array de 20 filas y 4 columnas para guardar las posiciones

int i=0;                                                                                                                    //Variables para poder movernos por el array
int j=0;
int iGrabar=0;
int jGrabar=0;
int iGrabarActual=0;
int iReproducir=0;
int jReproducir=0;

int valorPOTBase;                                                                                                            //Variables para añadir los vales del potenciometro al array
int anguloBase;
int valorPOTHombro;
int anguloHombro;
int valorPOTCodo;
int anguloCodo;
int valorPOTMuneca;
int anguloMuneca;
int valorPOTPinza;
int anguloPinza;

int ultimaFila=0;                                                                                                           //para saber cual es la ultima fila utilizada 
int anguloBaseRep=0;                                                                                                        //para ir moviendo el servo al darle a reproducir
int anguloHombroRep=0;
int anguloCodoRep=0;
int anguloMunecaRep=0;
int anguloPinzaRep=0;
int angulo1=0;                                                                                                              //angulos para saber la posicion que esta en ese momento el servo
int angulo2=0;
int angulo3=0;
int angulo4=0;
int angulo5=0;
  
Servo Base;                                                                                                                 //servomotores utilizados
Servo Hombro;
Servo Hombro2;   
Servo Codo;
Servo Muneca;
Servo Pinza;
void setup() {
  Serial.begin(9600);                                                                                                       //Comunicacion entre la placa y el ordenador mediante el "monitor serie"
  pinMode(Grabar, INPUT);                                                                                                   //Los pines utilizados por los pulsadores lo utilizamos como entradas
  pinMode(Reproducir, INPUT);   
  pinMode(Resetear, INPUT);     
  
  pinMode(POTBase, INPUT);                                                                                                  //los pines de los potenciometros los asignamos a entradas
  pinMode(POTHombro, INPUT);
  pinMode(POTCodo, INPUT);
  pinMode(POTMuneca, INPUT);
  pinMode(POTPinza, INPUT);
  
  Base.attach(servoPinBase);                                                                                                //Asociamos los servos a los pines donde estan colocados
  Hombro.attach(servoPinHombro);
  Hombro2.attach(servoPinHombro2);
  Codo.attach(servoPinCodo);
  Muneca.attach(servoPinMuneca);
  Pinza.attach(servoPinPinza);

  for(i=0; i<20; i++)                                                                                                       //inicializamos a 0 todos las celdas del array 
     {
      for(j=0; j<5; j++)
      {
        MovGuardado[i][j]=0;                                               
      }
       
     }
  Serial.print("Inicializado: ");     
}

void loop() 
{
  
    if(digitalRead(Reproducir) == LOW)                                                                                      //Si no hemos pulsado el pulsador de reproducir, copiara los movimientos de los potenciometros
  {
    valorPOTBase = analogRead(POTBase);
    anguloBase = map(valorPOTBase,0,1023,180,0);
    Base.write(anguloBase);
    valorPOTHombro = analogRead(POTHombro);
    anguloHombro = map(valorPOTHombro,110,890,50,150);
    Hombro.write(anguloHombro);
    Hombro2.write(anguloHombro); 
    valorPOTCodo = analogRead(POTCodo);
    anguloCodo = map(valorPOTCodo,110,890,0,180);
    Codo.write(anguloCodo); 
    valorPOTMuneca = analogRead(POTMuneca);
    anguloMuneca = map(valorPOTMuneca,110,890,0,180);
    Muneca.write(anguloMuneca);
     valorPOTPinza = analogRead(POTPinza);
     anguloPinza = map(valorPOTPinza,110,890,10,75);
    Pinza.write(anguloPinza);
  
  Serial.println(anguloBase);
  Serial.print(anguloHombro);
  Serial.print(anguloCodo);
  Serial.print(anguloMuneca);
  Serial.print(anguloPinza);
  
  
  }
   if(digitalRead(Resetear) == HIGH)                 //Al pulsar resetear, ira colocando 0 a todo el array para borrar los valores existentes
  {  
     Serial.println("Borrando, espere...");  
     delay(100);
     for(i=0; i<20; i++)
     {
      for(j=0; j<5; j++)
      {
        MovGuardado[i][j]=0;                                               
      }
      Serial.print("Borrado: ");  Serial.println(i);
     }
     Serial.println("Borrado Total"); 
     iGrabar=0;                                                      //Se pone a 0 estas variables ya que no hay valores guardados
     ultimaFila=0;
  }   

  if ((digitalRead(Grabar) == HIGH)&&(ultimaFila!=19))      //Cuando se de al pulsador de guardar posicion y no se haya excedido el numero de filas del array
  {   
    Serial.println("Grabando posicion");   
    delay(100);
     for(iGrabar; iGrabar<=ultimaFila; iGrabar++)                                                                         //Primero iremos recorriendo cada fila
     {
      for(jGrabar=0; jGrabar<5; jGrabar++)                                                                                //Entonces al estar en cada fila iremos pasando columna por columna
      {
       if(jGrabar==0)                                                                                                     //En la primera columna grabara la posicion de la base, copiando el valor que tuviera en ese momento "anguloBase"
       {
        MovGuardado[iGrabar][jGrabar]=anguloBase;
        Serial.println("Base guardada");
        delay(300); 
       }
       if(jGrabar==1)                                                                                                     //En la segunda columna grabara la posicion del hombro
       {
        MovGuardado[iGrabar][jGrabar]=anguloHombro;                                               
        Serial.println("Hombro guardado");
        delay(300);        
       }
       if(jGrabar==2) {
        MovGuardado[iGrabar][jGrabar]=anguloCodo;                                                                         //En la tercera columna grabara la posicion del codo
        Serial.println("Codo guardado");
        delay(300);        
       }
       if(jGrabar==3) {                                                                                                   //En la cuarta columna grabara la posicion de la Muneca
        MovGuardado[iGrabar][jGrabar]=anguloMuneca;                                               
        Serial.println("Muneca guardada");
        delay(300);        
       }
       if(jGrabar==4) {                                                                                                   //En la cuarta columna grabara la posicion de la Muneca
        MovGuardado[iGrabar][jGrabar]=anguloPinza;                                               
        Serial.println("Muneca pinza");
        delay(300);        
       }       
 
      }
     
      
     }
     ultimaFila+=1;                                                                                                       //Ira sumando uno cada vez que se cree una fila nueva
  
      if(ultimaFila==19)                                                                                                  //Si llega a la ultima fila, le saldra el siguiente dialogo
      {
        Serial.println("Maximo de posiciones guardadas, no se pueden añadir mas posiciones");
      }
    
  }

  if ((digitalRead(Grabar) == HIGH)&&(ultimaFila==19))                                                                    //Si intenta grabar mas posiciones estando el maximo de filas ocupadas, no dejara guardar mas posiciones hasta que se borren las existentes
  {
     Serial.println("Maximo de posiciones guardadas, no se pueden añadir mas posiciones");
  }
  
  if (digitalRead(Reproducir) == HIGH)                                                                                    //AL pulsar el pulsador de reproducir
  {       
    Serial.println("Reproduciendo posicion");
    delay(100);
    angulo1=anguloBase;                                                                                                   //copia los angulos que tienen los servos en el momento de reproducir, para saber donde empezar a cambiar angulos
    angulo2=anguloHombro;
    angulo3=anguloCodo;
    angulo4=anguloMuneca;
    angulo5=anguloPinza;
    
     for(iReproducir=0; iReproducir<iGrabar; iReproducir++)                                                               //Empezaremos a reproducir cada fila hasta que lleguemos al numero de filas que hemos guardado
     {
      for(jReproducir=0; jReproducir<5; jReproducir++)                                                                    //Iremos columna por colmuna en cada fila
      {
       delay(100);
       if(jReproducir==0)                                                                                                 //empezamos por la base
       {
         delay(100);
         Serial.print("Reproduciendo base posicion: "); Serial.println(iReproducir);
         if(angulo1<MovGuardado[iReproducir][jReproducir])                                                                //si el angulo que hay guardado en el array es mayor que el angulo que tiene en ese momento, ocurrira esta secuencia, aumentando en grados
         {                                                                                                                
          for(anguloBaseRep=angulo1; anguloBaseRep <= MovGuardado[iReproducir][jReproducir]; anguloBaseRep ++)            //ira escribiendo los valores de reproducir al servo y despues se guardara la ultima posicion donde se quedo
          {   
        
           Base.write(anguloBaseRep);
          delay(15);      
          angulo1=anguloBaseRep;                  
          }
         }
         Serial.print("Reproduciendo base posicion: "); Serial.println(iReproducir); 
         if(angulo1>MovGuardado[iReproducir][jReproducir])                                                                //si por el contrario es menos, ira disminuyendo grados
         {
           
          for(anguloBaseRep=angulo1; anguloBaseRep >= MovGuardado[iReproducir][jReproducir]; anguloBaseRep --) 
          {   
           
          Base.write(anguloBaseRep);
          delay(15);      
          angulo1=anguloBaseRep;                  
          }
         } 
       }
        if(jReproducir==1)                                                                                                  //Ocurre lo mismo que con la base pero en el hombro
        {
         delay(100);
         Serial.print("Reproduciendo Hombro posicion: "); Serial.println(iReproducir);
         if(angulo2<MovGuardado[iReproducir][jReproducir])
         {
          
          for(anguloHombroRep=angulo2; anguloHombroRep <= MovGuardado[iReproducir][jReproducir]; anguloHombroRep ++) 
          {   
              
            Hombro.write(anguloHombroRep);
            Hombro2.write(anguloHombroRep);
            delay(15);      
            angulo2=anguloHombroRep;                  
          }
         }
         Serial.print("Reproduciendo Hombro posicion: "); Serial.println(iReproducir);
         if(angulo2>MovGuardado[iReproducir][jReproducir])
         {
           
          for(anguloHombroRep=angulo2; anguloHombroRep >= MovGuardado[iReproducir][jReproducir]; anguloHombroRep --) 
          {   
           
          Hombro.write(anguloHombroRep);
          Hombro2.write(anguloHombroRep);
          delay(15);      
          angulo2=anguloHombroRep;                  
          }
         }
       }
       if(jReproducir==2)                                                                                                    //Ocurre lo mismo que con la base pero en el codo
        {
         delay(100);
         Serial.print("Reproduciendo Codo posicion: "); Serial.println(iReproducir);
         if(angulo3<MovGuardado[iReproducir][jReproducir])
         {
          
          for(anguloCodoRep=angulo3; anguloCodoRep <= MovGuardado[iReproducir][jReproducir]; anguloCodoRep ++) 
          {   
              
            Codo.write(anguloCodoRep);
            delay(15);      
            angulo3=anguloCodoRep;                  
          }
         }
         Serial.print("Reproduciendo Codo posicion: "); Serial.println(iReproducir);
         if(angulo3>MovGuardado[iReproducir][jReproducir])
         {
           
          for(anguloCodoRep=angulo3; anguloCodoRep >= MovGuardado[iReproducir][jReproducir]; anguloCodoRep --) 
          {   
           
          Codo.write(anguloCodoRep);
          delay(15);      
          angulo3=anguloCodoRep;                  
          }
         }
        }
        if(jReproducir==3)                                                                                                   //Ocurre lo mismo que con la base pero en la Muneca
        {
         delay(100);
         Serial.print("Reproduciendo Muneca posicion: "); Serial.println(iReproducir);
         if(angulo4<MovGuardado[iReproducir][jReproducir])
         {
          
          for(anguloMunecaRep=angulo4; anguloMunecaRep <= MovGuardado[iReproducir][jReproducir]; anguloMunecaRep ++) 
          {   
              
            Muneca.write(anguloMunecaRep);
            delay(15);      
            angulo4=anguloMunecaRep;                  
          }
         }
         Serial.print("Reproduciendo Muneca posicion: "); Serial.println(iReproducir);
         if(angulo4>MovGuardado[iReproducir][jReproducir])
         {
           
          for(anguloMunecaRep=angulo4; anguloMunecaRep >= MovGuardado[iReproducir][jReproducir]; anguloMunecaRep --) 
          {   
           
          Muneca.write(anguloMunecaRep);
          delay(15);      
          angulo4=anguloMunecaRep;                  
          }
         }
        }
                if(jReproducir==4)                                                                                                   //Ocurre lo mismo que con la base pero en la Muneca
        {
         delay(100);
         Serial.print("Reproduciendo Pinza posicion: "); Serial.println(iReproducir);
         if(angulo5<MovGuardado[iReproducir][jReproducir])
         {
          
          for(anguloPinzaRep=angulo5; anguloPinzaRep <= MovGuardado[iReproducir][jReproducir]; anguloPinzaRep ++) 
          {   
              
            Pinza.write(anguloPinzaRep);
            delay(7);      
            angulo5=anguloPinzaRep;                  
          }
         }
         Serial.print("Reproduciendo Pinza posicion: "); Serial.println(iReproducir);
         if(angulo5>MovGuardado[iReproducir][jReproducir])
         {
           
          for(anguloPinzaRep=angulo5; anguloPinzaRep >= MovGuardado[iReproducir][jReproducir]; anguloPinzaRep --) 
          {   
           
          Pinza.write(anguloPinzaRep);
          delay(15);      
          angulo5=anguloPinzaRep;                  
          }
         }
        }
 
       }
      
                                          
      }
      
     
      delay(1000);                                                                                                        //Cuando termina de reproducir toda la secuendia guardada, ira a la posicion de los potenciometros que estan colocados en ese momento
      if(anguloBase>angulo1)                                                                                              //si el angulo del potenciometro es mayor, la base ira aumentando en grados
         {
          Serial.println("posicion inicial Base"); 
          for(angulo1; angulo1 <= anguloBase; angulo1 ++) 
          {   
        
          Base.write(angulo1);
          delay(15);      
                  
          }
         }

      if(anguloBase<angulo1)                                                                                              //si por el contrario es menor, ira disminuyendo grados
         {
          Serial.println("posicion inicial Base"); 
          for(angulo1; angulo1 >= anguloBase; angulo1 --) 
          {   
           
          Base.write(angulo1);
          delay(15);      
                            
          }
         }
      if(anguloHombro>angulo2)                                                                                            //esto ocurre en los demas servos
         {
          Serial.println("posicion inicial Hombro"); 
          for(angulo2; angulo2 <= anguloHombro; angulo2 ++) 
          {   
        
          Hombro.write(angulo2);
          Hombro2.write(angulo2);
          delay(15);      
                  
          }
         }

      if(anguloHombro<angulo2)
         {
          Serial.println("posicion inicial Hombro"); 
          for(angulo2; angulo2 >= anguloHombro; angulo2 --) 
          {   
           
          Hombro.write(angulo2);
          Hombro2.write(angulo2);
          delay(15);      
                            
          }
         }
       if(anguloCodo>angulo3)
         {
          Serial.println("posicion inicial Codo"); 
          for(angulo3; angulo3 <= anguloCodo; angulo3 ++) 
          {   
        
          Codo.write(angulo3);
          delay(15);      
                  
          }
         }

      if(anguloCodo<angulo3)
         {
          Serial.println("posicion inicial Codo"); 
          for(angulo3; angulo3 >= anguloCodo; angulo3 --) 
          {   
           
          Codo.write(angulo3);
          delay(15);      
                            
          }
         }
         if(anguloMuneca>angulo4)
         {
          Serial.println("posicion inicial Muneca"); 
          for(angulo4; angulo4 <= anguloMuneca; angulo4 ++) 
          {   
        
          Muneca.write(angulo4);
          delay(15);      
                  
          }
         }

      if(anguloMuneca<angulo4)
         {
          Serial.println("posicion inicial Muneca"); 
          for(angulo4; angulo4 >= anguloMuneca; angulo4 --) 
          {   
           
          Muneca.write(angulo4);
          delay(15);      
                            
          }

  }
           if(anguloMuneca>angulo5)
         {
          Serial.println("posicion inicial Pinza"); 
          for(angulo5; angulo5 <= anguloPinza; angulo5 ++) 
          {   
        
          Pinza.write(angulo5);
          delay(15);      
                  
          }
         }

      if(anguloPinza<angulo5)
         {
          Serial.println("posicion inicial PINZA"); 
          for(angulo5; angulo5 >= anguloPinza; angulo5 --) 
          {   
           
          Pinza.write(angulo5);
          delay(15);      
                            
          }

  }
}
}
