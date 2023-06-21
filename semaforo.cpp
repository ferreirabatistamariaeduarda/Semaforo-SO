#include <stdio.h>
#include <windows.h>

HANDLE eventKill;
HANDLE eventExitThread;
HANDLE captureThread;

static int semaforo;
static int fim;
int cont = 0;

class TProcessThread
{
  public:
    int i;
    TProcessThread() { i = 0; }
    
    void Process(void) { 
         if( semaforo == 0){
          semaforo =1;                 
             puts("executando area critica thread 01"); 
             fim=fim+1;  
         semaforo=0;
         }
         else{ puts("thread 01 não pode ocupar area critica - semaforo vermelho #####");
		 cont++;
		 }
         _sleep(100000); 
         }
         
};

class TProcessThread2
{
  public:
    int i;
    TProcessThread2() { i = 0; }
    void Process(void) { 
         if( semaforo == 0){
         semaforo =1;
          puts("executando �rea critica thread 02");
          fim=fim+1;  
         semaforo =0;
         }
         else{ puts("thread 02 não pode ocupar area critica - semaforo vermelho ######");
		 cont++;
		 }
          _sleep(100);
         }
        
};

class TProcessThread3
{
  public:
    int i;
    TProcessThread3() { i = 0; }
    
    void Process(void) { 
         if( semaforo == 0){
          semaforo =1;                 
             puts("executando area critica thread 03"); 
             fim=fim+1;  
         semaforo=0;
         }
         else{ puts("thread 03 não pode ocupar area critica - semaforo vermelho #####");
		 cont++;
		 }
         _sleep(100); 
         }
         
};

DWORD WINAPI captureThreadProc(LPVOID lpvoid)
{
   TProcessThread *pf = (TProcessThread*)lpvoid;
   while (WaitForSingleObject(eventKill, 0) == WAIT_TIMEOUT)
   {
      pf->Process();
   }
   SetEvent(eventExitThread);
   ExitThread(0);
   return 0;
}

DWORD WINAPI captureThreadProc2(LPVOID lpvoid)
{
   TProcessThread2 *pf = (TProcessThread2*)lpvoid;
   while (WaitForSingleObject(eventKill, 0) == WAIT_TIMEOUT)
   {
      pf->Process();
   }
   SetEvent(eventExitThread);
   ExitThread(0);
   return 0;
}

DWORD WINAPI captureThreadProc3(LPVOID lpvoid)
{
   TProcessThread3 *pf = (TProcessThread3*)lpvoid;
   while (WaitForSingleObject(eventKill, 0) == WAIT_TIMEOUT)
   {
      pf->Process();
   }
   SetEvent(eventExitThread);
   ExitThread(0);
   return 0;
}

int main(int argc, char *argv[])
{
   TProcessThread *pt;
   pt = new TProcessThread();
   int x;
   
   semaforo=0;
   fim=0;

   eventKill = CreateEvent(NULL, TRUE, TRUE, NULL);
   ResetEvent(eventKill);
   eventExitThread = CreateEvent(NULL, TRUE, TRUE, NULL);
   ResetEvent(eventExitThread);
   while (fim <= 50)
   {

         //captura da execução da thread 01
         captureThread= CreateThread(0, 0, captureThreadProc, (LPVOID)pt,
                               CREATE_SUSPENDED, NULL);
         ResumeThread(captureThread);

         //captura da execução da thread 02
         captureThread= CreateThread(0, 0, captureThreadProc2, (LPVOID)pt,
                               CREATE_SUSPENDED, NULL);
         ResumeThread(captureThread);
         
         //captura da execução da thread 03
         captureThread= CreateThread(0, 0, captureThreadProc3, (LPVOID)pt,
                               CREATE_SUSPENDED, NULL);
         ResumeThread(captureThread);


         _sleep(500);
        }
    printf("%d\n\n", cont);
        
   SetEvent(eventKill);
   system("PAUSE");
   WaitForSingleObject(eventExitThread, INFINITE);

   return 0;
}
