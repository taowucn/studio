#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>
typedef union semun//semctl��Ҫ��
{
    int	val;//�����ź���ֵ
    struct semid_ds *buf;//�ź������ƽṹָ��
    ushort *array;//�޷��Ŷ����α���
} SEMCTL_UNION;

main()
{
    int n,semid;//�ź�����ʾ������
    key_t semkey;//��ֵ����
    SEMCTL_UNION semctl_arg;//�������ͱ���
    struct sembuf buf;//semop��������Ľṹ����
    if((semkey = ftok("/etc/profile",1)) < 0)//������ֵ
    {
        perror("ftok");
        exit(1);
    }
    if((semid = semget(semkey,1,0)) < 0)//�����ź���
    {
        perror("semget");
        exit(2);
    }
    semctl_arg.val = 2;//��ʼ��
    if (semctl(semid,0,SETVAL,semctl_arg) < 0)//�����ź�����ʼֵ
    {
        perror("semctl");
        exit(3);
    }
    memset(&buf,0x00,sizeof(struct sembuf));//���
    buf.sem_num = 0;//�ź�����Ŵ�0��ʼ����һ��
    buf.sem_op = 1;//P����������-1
    buf.sem_flg = IPC_NOWAIT;//������

    if(semop(semid,&buf,1) == -1)//P����
        {
            perror("semop");
        }

    buf.sem_op = -1;//P����������-1
    for(n=0;;n++)//ѭ������P������ֱ���ź�����Ϊ0
    {
        if(semop(semid,&buf,1) == -1)//P����
        {
            perror("semop");
            break;
        }
        printf("semop[%d]:current semphore value=%d\n",n,semctl(semid,0,GETVAL,semctl_arg));
    }
}