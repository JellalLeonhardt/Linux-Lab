#include<stdio.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<sched.h>
//全局变量
pthread_mutex_t *mutex;
pthread_t *tid;
int PhilosopherNum = 0; //总数量
int PhilosopherNumber = 0;

void philosopher(void);

void philosopher(void){
    int state = 1; 
    int Philosophernum = PhilosopherNumber++;
    srand(time(0));
    printf("哲学家%d开始思考了\n",Philosophernum+1);
    while(1){
        switch(state){
            case 0:    
            sleep(rand()%10);
            state = 1;
            printf("哲学家%d饿了\n",Philosophernum+1);
            break;
            case 1:
            if(pthread_mutex_trylock(&mutex[Philosophernum]) != 0){
                continue;
            }
            else{
                if(pthread_mutex_trylock(&mutex[(Philosophernum+1)%PhilosopherNum]) != 0){
                    pthread_mutex_unlock(&mutex[Philosophernum]);
                }
                else{
                    state = 2;
                    printf("哲学家%d开始进餐\n",Philosophernum+1);
                }
            }
            break;
            case 2:
            sleep(rand()%10);
            state = 0;
            printf("哲学家%d进餐完毕，开始思考了\n",Philosophernum+1);
            pthread_mutex_unlock(&mutex[Philosophernum]);
            pthread_mutex_unlock(&mutex[(Philosophernum+1)%PhilosopherNum]);
            break;
            default:
            printf("错误状态！！！\n");
            exit(0);
        }
    }
}
int main(){ 
    int i;
    pthread_attr_t attr;
    struct sched_param Param;
    pthread_attr_init(&attr);
    pthread_attr_getschedparam(&attr,&Param);
    Param.sched_priority = 20;
    pthread_attr_setschedparam(&attr,&Param);
    printf("输入哲学家数量：");
    scanf("%d",&PhilosopherNum);
    if(PhilosopherNum < 2) PhilosopherNum = 5;
    mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)*PhilosopherNum);
    tid = (pthread_t *)malloc(sizeof(pthread_t)*PhilosopherNum);
    int res;
    for(i = 0;i < PhilosopherNum;i++){
        res = pthread_mutex_init(&mutex[i],NULL);
        if(res!=0){
            printf("互斥量初始化失败!!!\n");
            exit(0);
        }
    }
    for(i = 0;i < PhilosopherNum;i++){
        printf("%d\n",i);
        Param.sched_priority = 20 + i;
        pthread_attr_setschedparam(&attr,&Param);
        res = pthread_create(&tid[i],NULL,(void*)philosopher,NULL);
        if(res!=0){
            printf("线程初始化失败!!!\n");
            exit(0);
        }
    }
    rewind(stdin);
    char key = 0;
    while(key != 'q') key = getchar();
    for(i = 0;i < PhilosopherNum;i++){
        pthread_cancel(&tid[i]);
    }
    return 0;
}
