
#define ACCEL_ADDRESS ((int) 0x53)
#define WIRE_SEND(b) Wire.write((byte) b) 
#define WIRE_RECEIVE() Wire.read() 
#define pi 3.14

float acc_x,acc_y,acc_z,Ax,Ay,Ayold,Axold,Azold,ax,ay,avg,avgy,avgz,sumx=0.0,sumy=0.0,sumz=0.0,Acc_x,Acc_y,Acc_z,ans;
int GYRO_ADD = 105,count=0,time=0,samplescount;
float v[5],result,data;
float gyr_x, gyr_y, gyr_z ,gyr_xo, gyr_yo, gyr_zo,roll=0,roll1=0,pitch=0,yaw=0,yaw_1=0,pitch1=0,gyr_yp=0,gyr_max,peak;
float alpha=.3,roll_2=0,hold_time,sample_time,sampling_rate;
const int buttonPin = 3; 
int buttonState = 0,count4, peakcounter;  
int peak_count_interval=5,interval_flag=0;
#define mag_address 0x1E
//#ifndef __CC3200R1M1RGC__
 char accx[10],accy[10],accz[10],gyrx[10],gyry[10],gyrz[10],str_trig[10],str_sampling[10],str_roll[10],roll_1[10],pitch_1[10],str_emg[10];

int keyIndex = 0;
int p=0;


