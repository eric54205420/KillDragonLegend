#include <scrwlock.h>

// add read lock
void 
rwlock_rlock(struct rwlock * lock) {
	for (;;) {
		// ���Ƿ���������ͼ��, �õ�����ֹ����λ���Ż�
		while (lock->wlock)
			ATOM_SYNC();

		ATOM_ADD_FETCH(lock->rlock, 1);
		// û��дռ��, ��ʼ����
		if (!lock->wlock)
			break;

		// ������д, ɾ����ӵĶ�
		ATOM_ADD_FETCH(lock->rlock, -1);
	}
}

// add write lock
void 
rwlock_wlock(struct rwlock * lock) {
	ATOM_LOCK(lock->wlock);
	// �ȴ���ռ����
	while (lock->rlock)
		ATOM_SYNC();
}

// add write lock
inline void 
rwlock_runlock(struct rwlock * lock) {
	ATOM_UNLOCK(lock->wlock);
}

// unlock write
inline void 
rwlock_wunlock(struct rwlock * lock) {
	ATOM_ADD_FETCH(lock->rlock, -1);
}