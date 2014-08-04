///////////////////////////////////////////////////////////////////////////////
//Main code file for reading sensors (Three Ulrasonic and six Ir sensors)    //
//cSensorInfo is character array of length 5. First three data units of      //
//this array will have information of three Ultrasonic sensor namely left,   //
//central and right respectively. If reading of sensor is beyond threshold   //
//information character will be '1' else '0'. Fourth data will have Ir sensor//
//number with highest reading. Fifth data will be '1' if i90 robot is near   //
//to Ir beacon else it will be '0'.                                          //
//v1.0                                                                       //
//First created                                                              //
//MohammadShahid Memon                                                       //
//04.08.14                                                                   //
///////////////////////////////////////////////////////////////////////////////






#include "mbed.h"
#include "HCSR04.h"   //Standard Library for HCSR04 Ultrasonic sensor

///////////////////////////////////////////////////////////////////////////////
//Following is brief explaination of HCSR04 library:                         //
//First create the object HCSR04. To create object two variables are passed  //
//First variable is pin number assigned for trigger of Ultrasonic sensor and //
//other variable is pin number assigned for echo of Ultrasonic sensor. For   //
//example if pin 29 is assigned to trigger and 30 to echo pins of ultrasonic //
//sensor then "HCSR04 Sensor1(p29,p30)" Will create the object.              //
//If HCSR04 object name is Sensor1 then Sensor1.distance(0) will give        //
//distance in Inches and Sensor1.distance(1) will give distance in CM        //
///////////////////////////////////////////////////////////////////////////////

void UpdateSensorInfo(void);    // UpdateSensorInfo function will read the sensor readings and update the information to be sent to the computer
void ReadUltrasonic(void);      // ReadUltrasonic function will read the three ultrasonic sensors and update the part of to be sent info data array
void ReadIr(void);              // ReadIr function will read the six Ir sensors and update the part of to be sent info data array

Serial pc(USBTX, USBRX);        // Creates object for serial communication 

HCSR04 LeftUltrasonic(p21, p22);    // Creates Ultrasonic sensor objects
HCSR04 CentralUltrasonic(p25, p26);
HCSR04 RightUltrasonic(p29, p30);

AnalogIn IR1(p15);                  // Assigning Analog pins for Ir sensors
AnalogIn IR2(p16);
AnalogIn IR3(p17);
AnalogIn IR4(p18);
AnalogIn IR5(p19);
AnalogIn IR6(p20);
char cSensorInfo[5];                // Character Array to be sent to pc
int iThresholdForUltrasonic=0;      // Threshold value for ultrasonic sensors for obstacles
int iThresholdForIr=0;              // Threshold value for Ir sensor for searching Ir beacon
int main(){
    
   
    while(1){
        UpdateSensorInfo();   // Read sensors and updates data to be sent
        wait(0.1);            // Frequency of transmitting data
    }
}

void ReadUltrasonic(){
    float fLeft,fCentral,fRight;  // Readings of Ultrasonic sensor readings to be stored in these variables
    fLeft = LeftUltrasonic.distance(0);
    wait(0.01);
    fCentral = CentralUltrasonic.distance(0);
    wait(0.01);
    fRight = RightUltrasonic.distance(0);
    wait(0.01);
    if(fLeft<=iThresholdForUltrasonic) // Updates info data if reading is below threshold then '1' else '0'
    cSensorInfo[0]='1';
    else
    cSensorInfo[0]='0';  
    if(fCentral>=iThresholdForUltrasonic) 
    cSensorInfo[1]='1';
    else
    cSensorInfo[1]='0';  
    if(fRight>=iThresholdForUltrasonic) 
    cSensorInfo[2]='1';
    else
    cSensorInfo[2]='0';   
}

void ReadIr(){
    float fIrReading[6];  // Ir sensor readings are stored in this array
    float fHighest=0;        // Used to find Ir with highest reading
    int iIrSensorNumber=0;   // Stores Ir sensor number with highest reading
    
    fIrReading[0]=IR1.read()*3.3;  // Reads the analog value of IR and converts into voltage level
    fIrReading[1]=IR2.read()*3.3;
    fIrReading[2]=IR3.read()*3.3;
    fIrReading[3]=IR4.read()*3.3;
    fIrReading[4]=IR5.read()*3.3;
    fIrReading[5]=IR6.read()*3.3;
    
    for(int i=0;i<=5;i++){               // Compares all Ir sensor reading and assigns the sensor number with highest reading 
        if(fIrReading[i]>fHighest){
            fHighest=fIrReading[i];
            iIrSensorNumber=i+1;
        }    
    }  
    
    cSensorInfo[3]=char(iIrSensorNumber+'0'); //Fourth data is Ir sensor number with highest reading 
    if(fIrReading[iIrSensorNumber-1]>=iThresholdForIr)  // Fifth info data '1' if Ir reading is more than threshold else '0'
        cSensorInfo[4]='1';
    else
        cSensorInfo[4]='0';          
}   

void UpdateSensorInfo(){  // Function calls ReadUltrasonic and ReadIr
    ReadUltrasonic();
    ReadIr();
}         