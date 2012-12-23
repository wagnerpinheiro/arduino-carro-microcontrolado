//PROJETO CARRO MICROCONTROLADO: Com Rotas Pré-Definidas e Aleatórias
//Projeto da disciplina: Projeto Integrado II
//Outubro de 2012

//define portas de controle:
//direcao
#define dir_en 2
#define dir_es 3
#define dir_di 5
//tracao
#define tra_en 8
#define tra_fr 9
#define tra_tr 10
//led
#define led 11
//DIP Switch
#define dip_s1 7
#define dip_s2 6
//Detector de Colisao
#define cd_wire 12

//define o tempo de parada para as manobras
long t_m = 250;
//define o tempo de tracao
long t_t = 1500;

int estado = 0;
int s1,s2,cd = 0;
long random_dir, random_time;

//variaveis para prova de conceito
float velocidade = 0.5; //m/s
float d_trajetoria_1 = 2; //distancia para trajetoria 1
float d_trajetoria_2 = 0.8; //lateral do quadrado circunscrito
float p, t;

void setup() {
  Serial.begin(9600);

  //define IO
  //motor dianteiro
  pinMode(dir_en, OUTPUT); //enable motor 1 
  pinMode(dir_es, OUTPUT); 
  pinMode(dir_di, OUTPUT);

  //motor traseiro
  pinMode(tra_en, OUTPUT); //enable motor 2
  pinMode(tra_fr, OUTPUT);
  pinMode(tra_tr, OUTPUT);

  //led colisao
  pinMode(led, OUTPUT);

  //detector colisao
  pinMode(cd_wire, INPUT);

  //DIP Switch
  pinMode(dip_s2, INPUT);
  pinMode(dip_s1, INPUT);

  //Ativa os resistores internos no modo pullup
  digitalWrite(dip_s1, HIGH);
  digitalWrite(dip_s2, HIGH);
  digitalWrite(cd_wire, HIGH);

  //inicializa o random utilizando um numero do ruido da entrada no pino 0
  randomSeed(analogRead(0));

}

void loop() {
  turn_off();
  delay(10);
  s1 = digitalRead(dip_s1);
  s2 = digitalRead(dip_s2);
  cd = digitalRead(cd_wire);
  //imprime as leituras dos pullups
  Serial.print("s1: ");
  Serial.print(s1);
  Serial.print("\t");
  Serial.print("s2: ");
  Serial.print(s2);
  Serial.print("\t");
  Serial.print("cd: ");
  Serial.println(cd);
  Serial.println("--------------------");

  if(s1==LOW && s2==HIGH && cd==HIGH){
    rota_5();
  }
  else if(s2==LOW && s1==HIGH && cd==HIGH){
    rota_6();    
  }
  else if(s2==LOW && s1==LOW){    
    rota_3();
  }
  else  if(cd==LOW){
    //colisao detectada, led aceso
    digitalWrite(led, HIGH);
    delay(t_t / 2);  
  }
  else{
    //aguardando rota, led pisca por 50ms
    digitalWrite(led, HIGH);
    delay(50);
    digitalWrite(led, LOW);
    delay(950);
  }    
}

//manobra o carro
void rota_1(){
  //necessario dobro do tempo de manobra, para nao sobrecarregar o motor
  //uma vez uqe o carro vai para frente e para tras
  delay(t_m * 2);
  digitalWrite(tra_en, HIGH);
  digitalWrite(dir_en, HIGH);
  if(estado==0){  
    estado = 1;
	digitalWrite(dir_di, HIGH);
	delay(t_m);
    digitalWrite(tra_fr, HIGH); //frente        
  }
  else{    
    estado = 0;  
    digitalWrite(dir_es, HIGH);
	delay(t_m);
    digitalWrite(tra_tr, HIGH); //ré
  }
  delay(t_t);
}

//roda para a direita
void rota_2(){
  digitalWrite(tra_en, HIGH);
  digitalWrite(dir_en, HIGH);

  digitalWrite(tra_fr, HIGH); //frente    
  digitalWrite(dir_di, HIGH);

  delay(t_t);
}

//desliga todas as saidas
void turn_off(){
  digitalWrite(tra_fr, LOW);
  digitalWrite(tra_tr, LOW);
  digitalWrite(dir_es, LOW);
  digitalWrite(dir_di, LOW);
  digitalWrite(led, LOW);  
  digitalWrite(tra_en, LOW);
  digitalWrite(dir_en, LOW);  
}

//aleatorio para busca
void rota_3(){
  delay(t_m);
  digitalWrite(tra_en, HIGH);
  digitalWrite(dir_en, HIGH);
  //gera numeros randomicos para a rota
  // 7 8 9
  // 4 5 6
  // 1 2 3
  if(cd==HIGH){
    random_dir = random(7, 10);
    random_time = random((t_t) / 2, t_t);
  }
  else if(cd==LOW){
    //colisao detectada, led aceso, anda para tras por 2s
    digitalWrite(led, HIGH);
    random_dir = random(1, 4);
	//volta 120% do tempo max de tracao
    random_time = (t_t * 1.2);  
    delay(t_m);    
  } 
  switch(random_dir){
  case 1:
    digitalWrite(dir_es, HIGH);
    delay(t_m);
    digitalWrite(tra_tr, HIGH);    
    break;
  case 2:
    digitalWrite(tra_tr, HIGH);      
    break;
  case 3:
    digitalWrite(dir_di, HIGH);
    delay(t_m);
    digitalWrite(tra_tr, HIGH);    
    break;

  case 7:
    digitalWrite(dir_es, HIGH);
    delay(t_m);
    digitalWrite(tra_fr, HIGH);
    break;
  case 8:
    digitalWrite(tra_fr, HIGH);      
    break;
  case 9:
    digitalWrite(dir_di, HIGH);
    delay(t_m);
    digitalWrite(tra_fr, HIGH);
    break;

  default:
    digitalWrite(led, HIGH);
    delay(t_m);
    digitalWrite(led, LOW);
    delay(t_m);
    break;
  }
  delay(random_time);  
}

//rota de apoio para calculo da velocidade
void rota_4(){
  delay(t_m * 2);
  digitalWrite(tra_en, HIGH);
  digitalWrite(tra_fr, HIGH); //frente        
  delay(1000);
  digitalWrite(tra_en, LOW); //desliga
  delay(3000); //aguarda 3s para medir
}

//prova de conceito - trajetoria 1
void rota_5(){  
  delay(t_m * 2);
  digitalWrite(tra_en, HIGH);
  if(estado==0){  
    estado = 1;
    digitalWrite(tra_fr, HIGH); //frente        
  }else{    
    estado = 0;  
    digitalWrite(tra_tr, HIGH); //ré
  }
  t = (d_trajetoria_1/velocidade) * 1000;
  //t = t - (t_m * 2); //subtraia o tempo de manobra
  Serial.println(t);
  delay(t); //tempo para percorrer distancia para trajetoria 1
}

//prova de conceito - trajetoria 2
void rota_6(){  
  delay(t_m * 2);
  digitalWrite(tra_en, HIGH);
  digitalWrite(dir_en, HIGH);
  digitalWrite(dir_di, HIGH); //roda sempre para direita
  delay(t_m);
  if(estado==0){  
    estado = 1;
    digitalWrite(tra_fr, HIGH); //frente        
  }else{    
    estado = 0;  
    digitalWrite(tra_tr, HIGH); //ré
  }
  p = 3.1415 * (d_trajetoria_2 * sqrt(2));
  t = (p / velocidade) * 1000;
  Serial.println(t);
  delay(t); //tempo para percorrer o perimetro da trajetoria 2
}










