#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "main_asm.h" /* interface to the assembly module */

#define totalTasks 3
unsigned char index = 0;

struct PCB
{
	char ID;							//Process Identifier
	char priority;						//The greater the more priority
	char autoStart;						//If executed at start or not
	unsigned int *initPC;						//Initial program address
	unsigned int *restorePC;					//Address to restore Program exec.
};

struct PCB readyTasks[totalTasks];
struct PCB tasksPCBs[totalTasks];

struct PCB Task1;
struct PCB Task2;
struct PCB Task3;

void MCUInit(void)
{
	ICSC1 = 0x04; 
	ICSC2 = 0x40; 
	SOPT2 = 0x00;
	FCDIV = 0x13;
	SOPT1 = 0x53;
}

void ordena(struct PCB dats[],char dim)
{
    char cambio=1,cont,aux;
    while (cambio==1)
    {
        cambio=0;
        for(cont=1;cont<dim;cont++)
            if(dats[cont].priority<dats[cont+1].priority){
                aux=dats[cont].priority;
                dats[cont].priority=dats[cont+1].priority;
                dats[cont+1].priority=aux;
                cambio=1;
    }}
}

void activateTask (struct PCB Task){//1
	//Mover a RAM o no.
	readyTasks[index] =  Task;
	ordena(readyTasks,index);


	index++;
	
}

void terminateTask (void){


}

void chainTask (char task_id){


}

void task3(void){
	char temp = 0;
	PTBD_PTBD6=1;
	PTBDD_PTBDD6=1;
	PTBD_PTBD7=1;
	PTBDD_PTBDD7=1;

	PTBD_PTBD7=0;
	PTBD_PTBD6=0;
	while(temp <= 3){
		do{
			__RESET_WATCHDOG();
		}while (SRTISC_RTIF==0);
		SRTISC_RTIACK=1;
		PTBD_PTBD7=1;
		PTBD_PTBD6=1;

		do{
			__RESET_WATCHDOG();
		}while (SRTISC_RTIF==0);
		SRTISC_RTIACK=1;
		PTBD_PTBD7=0;
		PTBD_PTBD6=0;
		temp++;
	}
	PTBD_PTBD7=0;
	PTBD_PTBD6=0;
	terminateTask();
}

void task2(void){
	char temp = 0;
	PTBD_PTBD7=1;
	PTBDD_PTBDD7=1;

	PTBD_PTBD7=0;
	while(temp <= 3){
		do{
			__RESET_WATCHDOG();
		}while (SRTISC_RTIF==0);
		SRTISC_RTIACK=1;
		PTBD_PTBD7=1;

		do{
			__RESET_WATCHDOG();
		}while (SRTISC_RTIF==0);
		SRTISC_RTIACK=1;
		PTBD_PTBD7=0;
		temp++;
	}
	PTBD_PTBD7=0;
	task3();
	terminateTask();
}

void task1(void){
	char temp = 0;
	PTBD_PTBD6=1;
	PTBDD_PTBDD6=1;

	PTBD_PTBD6=0;
	while(temp <= 3){
		do{
			__RESET_WATCHDOG();
		}while (SRTISC_RTIF==0);
		SRTISC_RTIACK=1;
		PTBD_PTBD6=1;

		do{
			__RESET_WATCHDOG();
		}while (SRTISC_RTIF==0);
		SRTISC_RTIACK=1;
		PTBD_PTBD6=0;
		temp++;
	}
	PTBD_PTBD6=0;
	task2();
	terminateTask();
}

unsigned int *t1 = &task1; //Ver lo del &
unsigned int *t2= &task2; //Ver lo del &
unsigned int *t3 = &task3; //Ver lo del &

void main(void) {
	//EnableInterrupts;
	/* include your code here */

	MCUInit();

	//Codigo para iniciar el QG8

	Task1.ID = 1;
	Task1.priority = 0;
	Task1.initPC = t1;
	Task1.autoStart = 1;
	
	Task2.ID = 2;
	Task2.priority = 1;
	Task2.initPC = t2;
	Task2.autoStart = 0;

	Task3.ID = 3;
	Task3.priority = 2;
	Task3.initPC = t3;
	Task3.autoStart = 0;

	tasksPCBs[0] = Task1;
	tasksPCBs[1] = Task2;
	tasksPCBs[2] = Task3;






	//asm_main();		/* call the assembly function */

	for(;;) {
		//__RESET_WATCHDOG();	/* feeds the dog */
		unsigned char i=0;
		for(i;i<totalTasks-1;i++){
			if(tasksPCBs[i].autoStart == 1){ 
				activateTask(tasksPCBs[i]);
			}
		}
		//Empiezo a ejecutar 
		if(readyTasks[0] =! null){
			
		}
		
	} /* loop forever */
	/* please make sure that you never leave main */
}
