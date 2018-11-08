//this part is for servo's motion

#include <AudioAnalyzer.h>
#include <Servo.h> 
Analyzer Audio = Analyzer(2,3,5);

Servo myservo; 
 
int FreqVal[7];//
int xielv[2]={0,0};
int note=0;  //coungt times i have coughed
long coughtime[7]; 
long shaketime[7];
long lasttime=0;
boolean detect=true;
boolean drive=true;
long offtime;
long startdrivetime;


void setup()
{
  Serial.begin(57600);
  Audio.Init();//Init module
  for(int i=0;i<6;i++){
    coughtime[i]=0; 
    shaketime[i]=0;

  }
  //servo initialization
  myservo.attach(9); 

  
}

void loop()
{

//detection stage
Audio.ReadFreq(FreqVal);
xielv[0]=xielv[1];
xielv[1]=max((FreqVal[4]-100),0);
Serial.print(max((FreqVal[4]-100),0)); //only for detection, no meaning
Serial.println();

if(detect){
if(xielv[0]<370 && xielv[1]>370){
  if(xielv[0]<xielv[1]){
coughtime[note]=millis(); 
lasttime=coughtime[note];
note+=1;
Serial.print("Coughing Times count:");
Serial.print(note);
Serial.println();
}
}
}


//judge when to stop
if(detect){
if(note>0){
if((millis()-lasttime)>3000){
detect=false;    //stop detecting

for(int j=1;j<6;j++){
  if(coughtime[j]>0){
    shaketime[j-1]=coughtime[j]-coughtime[j-1];
  }
}
startdrivetime=millis();
}
}
}


//drive stage!!!

  if(detect==false){
    if(drive){
      if(note>0){
        
         if(millis()-startdrivetime<500){
          myservo.write(180);  
         }
         else if(millis()-startdrivetime<1000){
          myservo.write(0);  
         }else{
          startdrivetime=millis();
          note=note-1;
         }
               
          }
          if(note==0){
            drive=false;
            offtime=millis();
          }
  
  }
  }

 
  
//reset
if(detect==false){
  if(drive==false){
    if(millis()-offtime>3500){
      detect=true;
      Serial.print("RESET DETECT");
      Serial.println();
      drive =true;
      Serial.print("RESET DRIVE");
      Serial.println();
        for(int l=0;l<6;l++){
    coughtime[l]=0; 
    shaketime[l]=0;
  }
  note=0;
  lasttime=millis();

  
    }
  }

  
}




  delay(20);
}

