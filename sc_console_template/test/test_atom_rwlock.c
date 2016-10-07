#include <stdio.h>
#include <scrwlock.h>
#include <pthread.h>

#define _INT_BZ     (13)
#define _INT_WTH    (2)
#define _INT_RTH    (10)

struct rwarg {
	pthread_t id;
	struct rwlock lock;    // �����õ�  
	int idx;                 // ָʾbuf��д������
	char buf[BUFSIZ];        // �洢��ʱ����
};

// д�߳�, ��Ҫ���д�ַ���ȥ
void twrite(struct rwarg * arg);
// ���߳�
void treads(struct rwarg * arg);

/*
* �Լ�д��д���ײ�
*/
int main_atom_rwlock(int argc, char * argv[]) {
	// ��ʼ��������Ҫʹ�õ���
	int i;
	struct rwarg arg = { 0 };

	// ���߳�������
	for (i = 0; i<_INT_RTH; ++i)
		pthread_create((pthread_t *)&arg, NULL, (void * (*)(void *))treads, &arg);

	// д�߳���������
	for (i = 0; i<_INT_WTH; ++i)
		pthread_create((pthread_t *)&arg, NULL, (void * (*)(void *))twrite, &arg);

	// �򵥵ȴ�һ��
	printf("sleep input enter:");
	getchar();

	return 0;
}

// д�߳�, ��Ҫ���д�ַ���ȥ
void
twrite(struct rwarg * arg) {
	pthread_detach(pthread_self());

	while (arg->idx < _INT_BZ) {
		rwlock_wlock(&arg->lock);
		arg->buf[arg->idx] = 'a' + arg->idx;
		++arg->idx;
		rwlock_wunlock(&arg->lock);
	}
	puts("twrite is exit...");
}

// ���߳�
void
treads(struct rwarg * arg) {
	pthread_detach(pthread_self());

	while (arg->idx < _INT_BZ) {
		rwlock_rlock(&arg->lock);
		puts(arg->buf);
		rwlock_runlock(&arg->lock);
	}
	puts("treads is exit...");
}