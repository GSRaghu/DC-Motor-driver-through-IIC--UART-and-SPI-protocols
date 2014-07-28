
#include <SPI.h>


char buf [26];
volatile byte pos;
volatile boolean process_it;

int inc=0;
int wmotor=0;//which motor
int motorpwm=0;
int aspwm=0;


void setup (void)
{
  Serial.begin (9600);   // debugging

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  // get ready for an interrupt 
  pos = 0;   // buffer empty
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();
  
  pinMode(3,OUTPUT);//load a
  pinMode(5,OUTPUT);//load a
  pinMode(6,OUTPUT);//load b
  pinMode(9,OUTPUT);//load b
  analogWrite(3,0);
  analogWrite(5,0);
  analogWrite(6,0);
  analogWrite(9,0);

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
byte c = SPDR;  // grab byte from SPI Data Register
  
  // add to buffer if room
  if (pos < sizeof buf)
    {
    buf [pos++] = c;
    
    // example: newline means time to process buffer
    if (c == '\n')
      process_it = true;
      
    }  // end of room available
}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  if (process_it)
    {
    buf [pos] = 0;  
    Serial.println (buf);
    pos = 0;
    process_it = false;
    gopera();
    }  // end of flag set
    
    
}  // end of loop

void gopera(void)
{
  for(inc=0;inc<21;inc++)
  Serial.print(buf[inc]); 
  Serial.println(" ");
  
  if((buf[2]=='M')||(buf[2]=='m'))
  motordrive();
  if((buf[2]=='s')||(buf[2]=='S'))
  haltmotor();
  
  for(inc=0;inc<26;inc++)
  {
    buf[inc]='0';
  }
}

void motordrive(void)
{
  Serial.println("motor drive func");
  wmotor=buf[8]-48;
  Serial.println("the motor to be controlled");
  Serial.println(wmotor); 
  
  aspwm=buf[15]-48;
  motorpwm=aspwm*100;
  
  aspwm=buf[16]-48;
  motorpwm=motorpwm+(aspwm*10);
  
  aspwm=buf[17]-48;
  motorpwm=motorpwm+aspwm;
  
  if(motorpwm>255)
  motorpwm=0;
  
  Serial.println("pwm of motor");
  Serial.println(motorpwm);
  
  if((buf[10]=='a')||(buf[10]=='A'))
  amotor();
  
  if((buf[10]=='c')||(buf[10]=='C'))
  cmotor();
  
}

void amotor(void)//anti clock wise
{
  
  analogWrite(6,0);//load2
  Serial.println("anti clock wise direction");
  Serial.println(wmotor); 
  if(wmotor==1)
  
  Serial.println(motorpwm);
  if(wmotor==1)
  {
    analogWrite(5,motorpwm);
    analogWrite(3,0);//load1
  }
  if(wmotor==2)
  {
    analogWrite(9,motorpwm);
    analogWrite(6,0);//load2
  }
}

void cmotor(void)//clock wise
{
  
  
  Serial.println("clock wise direction");
  Serial.println(wmotor); 
  Serial.println(motorpwm);
  if(wmotor==1)
  {
    analogWrite(3,motorpwm);
    analogWrite(5,0);//load 1
  }
  if(wmotor==2)
  {
    analogWrite(6,motorpwm);
    analogWrite(9,0);//load 2
  }
}

void haltmotor(void)
{
  Serial.println("motor halt func");
  Serial.println("the motor to be stopped");
  
  if((buf[7]=='b')||(buf[7]=='B'))
  {
    Serial.println("both the motor to be stoped");
    analogWrite(3,0);
    analogWrite(5,0);
    analogWrite(6,0);
    analogWrite(9,0);
  }
  else
  {
    wmotor=buf[7]-48;
    Serial.println(wmotor); 
    if(wmotor==1)
    {
      analogWrite(3,0);
      analogWrite(5,0);
    }
    if(wmotor==2)
    {
      analogWrite(6,0);
      analogWrite(9,0);
    }
  }
  
  
}
