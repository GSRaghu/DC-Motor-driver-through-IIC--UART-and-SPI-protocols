int dont=1,yes=0,inc=0;
char readf[21]={0};
int dummy=0,dumv=0;
int wmotor=0;//which motor
int motorpwm=0;
int aspwm=0;

void setup()
{
  Serial.begin(9600);
  pinMode(3,OUTPUT);//load a
  pinMode(5,OUTPUT);//load a
  pinMode(6,OUTPUT);//load b
  pinMode(9,OUTPUT);//load b
  analogWrite(3,0);
  analogWrite(5,0);
  analogWrite(6,0);
  analogWrite(9,0);
}

void loop()
{
  if(Serial.available()>0)
    {
      readf[inc]=Serial.read();
      
      if(readf[0]=='$')
      dont=0;
    
      if(readf[inc]=='/')
      {
        dummy=inc;
      }
      
      dumv=inc-dummy;
      
      if(dumv==1)
      {
        if(readf[inc]=='n')
        {
          gopera();
          dont=1;
          for(inc=0;inc<21;inc++)
          readf[inc]='0';
          inc=0;
          dumv=0;
          dummy=0;
        }
      }
      
      if(dont==0)
      inc++;
    }
}

void gopera(void)
{
  for(inc=0;inc<21;inc++)
  Serial.print(readf[inc]); 
  Serial.println(" ");
  
  if((readf[2]=='M')||(readf[2]=='m'))
  motordrive();
  if((readf[2]=='s')||(readf[2]=='S'))
  haltmotor();
}

void motordrive(void)
{
  Serial.println("motor drive func");
  wmotor=readf[8]-48;
  Serial.println("the motor to be controlled");
  Serial.println(wmotor); 
  
  aspwm=readf[15]-48;
  motorpwm=aspwm*100;
  
  aspwm=readf[16]-48;
  motorpwm=motorpwm+(aspwm*10);
  
  aspwm=readf[17]-48;
  motorpwm=motorpwm+aspwm;
  
  if(motorpwm>255)
  motorpwm=0;
  
  Serial.println("pwm of motor");
  Serial.println(motorpwm);
  
  if((readf[10]=='a')||(readf[10]=='A'))
  amotor();
  
  if((readf[10]=='c')||(readf[10]=='C'))
  cmotor();
  
}

void amotor(void)//anti clock wise
{
  
 
  Serial.println("anti clock wise direction");
  Serial.println(wmotor); 
  Serial.println(motorpwm);
  if(wmotor==1)
  {
    analogWrite(5,motorpwm);
    analogWrite(3,0);
  }
  if(wmotor==2)
  {
   analogWrite(9,motorpwm);
   analogWrite(6,0);
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
  
  if((readf[7]=='b')||(readf[7]=='B'))
  {
    Serial.println("both the motor to be stoped");
    analogWrite(3,0);
    analogWrite(5,0);
    analogWrite(6,0);
    analogWrite(9,0);
  }
  else
  {
    wmotor=readf[7]-48;
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
