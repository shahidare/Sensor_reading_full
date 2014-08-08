///////////////////////////////////////////////////////////////////////////////
//Main code file for reading sensors (Three Ulrasonic and six Ir sensors)    //
//cSensorInfo is character array of length 15. First three data units of     //
//this array will have information of three Ultrasonic sensor namely left,   //
//central and right respectively. Rest 12 data will have Ir sensor reading.  //
// Each Ir sensor reading will have two character, first character represents//
//Integer part of reading and second character represents decimal value with //
//two digit precision                                                        //
//v1.2                                                                       //
//Based on: v1.0                                                             //
//Changelog:                                                                 //
//-Bluetooth communication added                                             //
//-Sending sensor data has been updated from 5 to 15 characters              //
//MohammadShahid Memon                                                       //
//Huseyin Emre Erdem                                                         //
//07.08.14                                                                   //
///////////////////////////////////////////////////////////////////////////////

#include "mbed.h"
#include "HCSR04.h"   //Standard Library for HCSR04 Ultrasonic sensor

/*Following is brief explaination of HCSR04 library:                         
 First create the object HCSR04. To create object two variables are passed  
 First variable is pin number assigned for trigger of Ultrasonic sensor and 
 other variable is pin number assigned for echo of Ultrasonic sensor. For   
 example if pin 29 is assigned to trigger and 30 to echo pins of ultrasonic 
 sensor then "HCSR04 Sensor1(p29,p30)" Will create the object.              
 If HCSR04 object name is Sensor1 then Sensor1.distance(0) will give        
 distance in Inches and Sensor1.distance(1) will give distance in CM*/

/*Prototypes*/
void UpdateSensorInfo(void);    // UpdateSensorInfo function will read the sensor readings and update the information to be sent to the computer
void ReadUltrasonic(void);      // ReadUltrasonic function will read the three ultrasonic sensors and update the part of to be sent info data array
void ReadIr(void);              // ReadIr function will read the six Ir sensors and update the part of to be sent info data array

/*Objects*/
HCSR04 LeftUltrasonic(p22, p21);    // Creates Ultrasonic sensor objects
HCSR04 CentralUltrasonic(p24, p23);
HCSR04 RightUltrasonic(p26, p25);
AnalogIn IR1(p15);                  // Assigning Analog pins for Ir sensors
AnalogIn IR2(p16);
AnalogIn IR3(p17);
AnalogIn IR4(p18);
AnalogIn IR5(p19);
AnalogIn IR6(p20);
Serial rn42(p9,p10);
//Serial pc(USBTX,USBRX);
/*Variables*/
char cSensorInfo[15];                // Character Array to be sent to pc
int iThresholdForUltrasonic=20;      // Threshold value for ultrasonic sensors for obstacles
int iThresholdForIr=2.2;              // Threshold value for Ir sensor for searching Ir beacon

int main(){
    rn42.baud(115200);
    while(1){
        /*Serial communication*/
        if(rn42.getc() == 's'){//If request is received
            UpdateSensorInfo();   // Read sensors and updates data to be sent
            for(int i=0;i<15;i++){//Send all characters
                rn42.putc(cSensorInfo[i]);
            }
        }
    }
}

void ReadUltrasonic(){
    int fLeft,fCentral,fRight;  // Readings of Ultrasonic sensor readings to be stored in these variables
    fLeft = LeftUltrasonic.distance();
    fCentral = CentralUltrasonic.distance();
    fRight = RightUltrasonic.distance();
    cSensorInfo[0]=char(fLeft);
    cSensorInfo[1]=char(fCentral);
    cSensorInfo[2]=char(fRight);
	
}

void ReadIr(){
    float fIrReading[6];  // Ir sensor readings are stored in this array
    float fIrReadingTemp[6]={0,0,0,0,0,0};  //Temp variable to read the highest value of 100 readings
    int iIntPart[6],iDeciPart[6];           // Sends Ir sensor readings in two characters. First character will represent integer part and another character decimal part
    for(int i=0;i<100;i++){
        fIrReading[0]=IR1.read();  // Reads the analog value of IR and converts into voltage level
        fIrReading[1]=IR2.read();
        fIrReading[2]=IR3.read();
        fIrReading[3]=IR4.read();
        fIrReading[4]=IR5.read();
        fIrReading[5]=IR6.read();
        for(int j=0;j<6;j++){          // Comparing the previous highest value to new read data and updating highest
            if(fIrReadingTemp[j]<=fIrReading[j])
                fIrReadingTemp[j]=fIrReading[j];
        }        
        wait(0.125/100);
    }
    for(int i=0;i<6;i++){                // Converting float into interger part and decimal part and assigning to differnt variable
        fIrReading[i]=fIrReadingTemp[i]*3.3;
        iIntPart[i]=int(fIrReading[i]);
        iDeciPart[i]=(fIrReading[i]-iIntPart[i])*100;   // To get precision upto two decimal.
        fIrReading[i]=0;
        fIrReadingTemp[i]=0;
    }   
    int iSensorInfoAddress=3;
    for(int i=0;i<6;i++){              // Converting integer to character to transfer bia bluetooth
		cSensorInfo[iSensorInfoAddress]=char(iIntPart[i]);
		iSensorInfoAddress++;
		cSensorInfo[iSensorInfoAddress]=char(iDeciPart[i]);
		iSensorInfoAddress++;
		
    }
	
}   

void UpdateSensorInfo(){  // Function calls ReadUltrasonic and ReadIr
    ReadUltrasonic();
    ReadIr();
}