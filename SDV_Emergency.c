#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <pthread.h>
#include <sched.h>
/* GPIO BASE */
#define GPIO_BASE 0xFE200000
#define BLOCK_SIZE 4096
#define GPFSEL0 (0x00/4) // pin mode
#define GPFSEL1 (0x04/4) // pin mode
#define GPFSEL2 (0x08/4) // pin mode
#define GPSET0  (0x1C/4) // High
#define GPCLR0  (0x28/4) // Low
#define GPLEV0  (0x34/4) // Read input
/* Pull-up registers */
#define GPPUD     (0x94/4)
#define GPPUDCLK0 (0x98/4)
/* Motor */
#define IN1 (1<<17) //Pin 11
#define ENA (1<<18) //Pin 12
#define IN3 (1<<22) //Pin 15
#define ENB (1<<23) //Pin 16
/* Ultrasonic */
#define TRIG  (1<<5) //Pin 29
#define ECHO_PIN (1<<6) //Pin 31
/* Button */
#define BUTTON (1<<13) //Pin 33
#define SERIAL_PORT "/dev/ser1" // UART
/* Shared */
volatile uint32_t *gpio;
double dis = 50; // initial distance
int em = 0; //Push
pthread_mutex_t lock;
/* -------- Ultrasonic -------- */
double get_distance()
{
    struct timespec start,end;
    gpio[GPCLR0]=TRIG; // clear trigger
    usleep(2);         // 2ms delay
    gpio[GPSET0]=TRIG; // initialize trigger (40KHz)
    usleep(10);        // 10ms delay
    gpio[GPCLR0]=TRIG; // clear
    clock_gettime(CLOCK_MONOTONIC,&start);
    while(!(gpio[GPLEV0]&ECHO_PIN)) // wait for echo high
    {
        clock_gettime(CLOCK_MONOTONIC,&end);
        double t=(end.tv_sec-start.tv_sec) // time measurement
        +(end.tv_nsec-start.tv_nsec)/1e9;
        if(t>0.03) return 999;     // timer to exit
    }
    clock_gettime(CLOCK_MONOTONIC,&start);
    while(gpio[GPLEV0]&ECHO_PIN)   // Measure echo high
    {
        clock_gettime(CLOCK_MONOTONIC,&end);
        double t=(end.tv_sec-start.tv_sec) // time measurement
        +(end.tv_nsec-start.tv_nsec)/1e9;
        if(t>0.03) return 999;  // timer to exit
    }
    clock_gettime(CLOCK_MONOTONIC,&end);
    double t=(end.tv_sec-start.tv_sec)+(end.tv_nsec-start.tv_nsec)/1e9; // time calculation
    return (t*34300)/2; // distance calculation
}
/* -------- BUTTON THREAD -------- */
void *button_thread(void *arg)
{
    int ls=1; // previous state
    printf("Button Thread Started\n");
    while(1)
    {
        int c=(gpio[GPLEV0]&BUTTON)?1:0; //current state
        /* Detect Falling Edge */
        if(ls==1 && c==0)
        {
            pthread_mutex_lock(&lock); // thread sync
            em=!em; // situation toggle
            if(em)
                printf("\n>>> EMERGENCY MODE ON\n");
            else
                printf("\n>>> NORMAL MODE\n");
            pthread_mutex_unlock(&lock);
            usleep(300000); // debounce
        }
        ls=c; // last as present
        usleep(20000); // delay
    }
}
/* -------- SENSOR THREAD -------- */
void *sensor_thread(void *arg)
{
    double d_fil=50; //initial assumption
    while(1)
    {
        double d_raw=get_distance(); //ultrasonic main function
        if(d_raw!=999) // if real value < 999
        {
            d_fil=0.4*d_fil+0.6*d_raw; // smooth value generation
            pthread_mutex_lock(&lock); // thread sync
            dis=d_fil;                 // final distance value
            pthread_mutex_unlock(&lock);
        }
        usleep(50000);
    }
}
/* -------- MOTOR THREAD -------- */
void *motor_thread(void *arg)
{
    while(1)
    {
        double d;
        int emergency;
        pthread_mutex_lock(&lock);
        d=dis;
        emergency=em;
        pthread_mutex_unlock(&lock);
        int duty;
        if(emergency) // Emergency mode
        {
            if(d<20) // in cm
                duty=0;
            else if(d<40)
                duty=50;
            else     // d>40
                duty=100;
        }
        else       // Normal mode
        {
            if(d<20)
                duty=0;
            else if(d<40)
                duty=50;
            else
                duty=100;
        }
        gpio[GPSET0]=IN1|IN3; // IN1 and IN3 High

        for(int i=0;i<20;i++) // PWM function
        {
            if(duty==0)
            {
                gpio[GPCLR0]=ENA|ENB; // Stop both motors
                usleep(10000);
            }
            else
            {
                int on=(10000*duty)/100; // ON duration
                int off=10000-on;        // OFF duration
                gpio[GPSET0]=ENA|ENB;    // High at ON
                usleep(on);
                gpio[GPCLR0]=ENA|ENB;    // Low at OFF
                usleep(off);
            }
        }
    }
}
/* -------- COMM THREAD -------- */
void *comm_thread(void *arg)
{
    int fd; // file descriptor variable
    struct termios options; // terminal Settings
    fd=open(SERIAL_PORT,O_RDWR|O_NOCTTY); // Common Channel
    tcgetattr(fd,&options); // Copies current serial configuration
    cfsetispeed(&options,B9600); // Input Baud rate
    cfsetospeed(&options,B9600); // Output Baud Rate
    options.c_cflag|=(CLOCAL|CREAD); // Common flags
    tcsetattr(fd,TCSANOW,&options); // Apply changing immediately
    while(1)
    {
        double d;
        int emergency;
        pthread_mutex_lock(&lock);
        d=dis;
        emergency=em;
        pthread_mutex_unlock(&lock);
        char msg[150]; // Message size
        if(emergency)
        {
            if(d>60)
            sprintf(msg,"ID:A1206;MODE:EMERGENCY;MSG:Far away\n");
            else if(d>20)
            sprintf(msg,"ID:A1206;MODE:EMERGENCY;MSG:Approaching, Be alert\n");
            else
            sprintf(msg,"ID:A1206;MODE:EMERGENCY;MSG:Clear the way, Emergency\n");
        }
        else
        {
            if(d<20)
            sprintf(msg,"ID:A1206;MODE:NORMAL;STATE:OBSTACLE\n");
            else if(d<40)
            sprintf(msg,"ID:A1206;MODE:NORMAL;STATE:TRAFFIC\n");
            else
            sprintf(msg,"ID:A1206;MODE:NORMAL;STATE:CLEAR\n");
        }
        write(fd,msg,strlen(msg)); // POSIX function to Communicate
        printf("Distance: %.2f | %s",d,msg);
        usleep(200000);
    }
}
/* -------- MAIN -------- */
int main()
{
    pthread_t t1,t2,t3,t4; // threads initialization
    struct sched_param param; //
    pthread_attr_t attr;
    ThreadCtl(_NTO_TCTL_IO,0); // Access low level Hardware
    pthread_mutex_init(&lock,NULL);
    void *gpio_ptr=mmap_device_memory(NULL,BLOCK_SIZE,PROT_READ|PROT_WRITE|PROT_NOCACHE,0,GPIO_BASE);
    gpio=(uint32_t*)gpio_ptr;
/* GPIO CONFIG */
/* TRIG output */
    gpio[GPFSEL0]&=~(7<<15);
	gpio[GPFSEL0]|=(1<<15);
	/* ECHO input */
	gpio[GPFSEL0]&=~(7<<18);
	/* IN1 */
	gpio[GPFSEL1]&=~(7<<21);
	gpio[GPFSEL1]|=(1<<21);
	/* ENA */
	gpio[GPFSEL1]&=~(7<<24);
	gpio[GPFSEL1]|=(1<<24);
	/* IN3 */
	gpio[GPFSEL2]&=~(7<<6);
	gpio[GPFSEL2]|=(1<<6);
	/* ENB */
	gpio[GPFSEL2]&=~(7<<9);
	gpio[GPFSEL2]|=(1<<9);
	/* BUTTON input */
	gpio[GPFSEL1]&=~(7<<9);
	/* Enable Pull-up */
	gpio[GPPUD]=0x02;
	usleep(5);
	gpio[GPPUDCLK0]=BUTTON;
	usleep(5);
	gpio[GPPUD]=0;
	gpio[GPPUDCLK0]=0;
	/* Thread Setup */
	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr,SCHED_FIFO);
	pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
	/* SENSOR */
	param.sched_priority=30;
	pthread_attr_setschedparam(&attr,&param);
	pthread_create(&t1,&attr,sensor_thread,NULL);
	/* BUTTON */
	param.sched_priority=25;
	pthread_attr_setschedparam(&attr,&param);
	pthread_create(&t2,&attr,button_thread,NULL);
	/* MOTOR */
	param.sched_priority=20;
	pthread_attr_setschedparam(&attr,&param);
	pthread_create(&t3,&attr,motor_thread,NULL);
	/* COMM */
	param.sched_priority=10;
	pthread_attr_setschedparam(&attr,&param);
	pthread_create(&t4,&attr,comm_thread,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
	return 0;
}
