#ifndef __H_SIMPLEC_CFGDATA
#define __H_SIMPLEC_CFGDATA

#include <array.h>

/*
 * ��������������ݻ�ȡ, ����
 */
extern void data_start(void);

/*
 * �������������������, �ͷ�
 */
extern void data_close(void);

/*----------------------------begin skill �����������-------------------------------*/
// �����ַ���
#define _INT_KNAME	24
// �������ַ���
#define _INT_BDESC	320

struct skill {
	char name[_INT_KNAME + 1];				// ��������
	char desc[_INT_BDESC + 1];				// ��������
	unsigned short keepround;				// ���ֵĻغ���
	int addblood;							// ���ӵ�Ѫ��
	short addattack;						// ���ӹ�����, ���հٷֱ�
	short adddefense;						// ���ӷ�����
	short addacceleration;					// �����ٶ�
	short addfortunately;					// ��������
	short needmp;							// �ͷ���Ҫmp(ħ��ֵ)
	unsigned short level;					// ��Ҫ�ĵȼ�
	bool isallowed;							// �Ƿ������ͷ�
};

/*
 * �õ������й��ڼ������õ���������
 *		: ���ؼ��ܶ�������
 */
extern const array_t data_skills(void);
/*----------------------------e n d skill �����������-------------------------------*/

/*----------------------------begin goods ��Ʒ�������-------------------------------*/

#define _GOOD_DRUGS			(1)		// ҩƷ
#define _GOOD_WEAPONS		(2)		// ����
#define _GOOD_ELIXIR		(3)		// �ɵ�, �汦

struct good {
	char name[_INT_KNAME + 1];				// ��Ʒ����
	char desc[_INT_KNAME + 1];				// ��Ʒ����
	unsigned char type;						// ��Ʒ����, see _GOOD_*
	int addmp;								// ���ӵ�ħ��MPֵ
	int addblood;							// ���ӵ�Ѫ��
	short addattack;						// ���ӹ�����
	short adddefense;						// ���ӷ�����
	short addacceleration;					// �����ٶ�
	short addfortunately;					// ��������
	int pirce;								// �۸�
	unsigned char panel;					// ռ�ð���������
	short addcrit;							// ���ӱ�����
	unsigned short cnt;						// ��Ʒӵ������
	unsigned short maxcnt;					// ��Ʒ���ӵ������
	bool isbuy;								// ��Ʒ�Ƿ������
	bool issell;							// ��Ʒ�Ƿ������
};

/*
 * �õ������й�����Ʒ��������������
 *		: ������Ʒ��������
 */
extern const array_t data_goods(void);

/*----------------------------e n d goods ��Ʒ�������-------------------------------*/

/*----------------------------begin maps ��ͼ�������-------------------------------*/



/*----------------------------e n d maps ��ͼ�������-------------------------------*/

// ����С�ַ���
#define _INT_SDESC	192

struct map {
	char name[_INT_KNAME + 1];				// ��ͼ����
	char desc[_INT_SDESC + 1];				// ��ͼ����
	short start;							// ��ͼ�й������
	int number;								// ��ͼ�й�������
	unsigned short level;					// ��ͼ�й������Ƶȼ�
};

/*
 * �õ������й��ڵ�ͼ��������������
 *		: ���ص�ͼ��������
 */
extern const array_t data_maps(void);

/*----------------------------begin monsters �����������-------------------------------*/

struct monster {
	char name[_INT_KNAME + 1];				// ��������
	char desc[_INT_SDESC + 1];				// ��������
	unsigned short level;					// ����ȼ�
	short attack;							// ������
	short defense;							// ������
	short acceleration;						// �ٶ� 
	int blood;								// ��ǰѪ��
	int maxblood;							// ���Ѫ��
	short mp;								// ��ǰħ����
	short maxmp;							// ���ħ����
	short experience;						// Я���ľ���
	short gold;								// Я���Ľ��
	bool isdrugre;							// �Ƿ�Я�������Ʒ����
	bool isweapons;							// �Ƿ�Я���������
	bool iselixir;							// �Ƿ�Я���汦
	bool isescape;							// �Ƿ���������
};

/*
 * �õ������й��ڹ����������������
 *		: ���ع����������
 */
extern const array_t data_monsters(void);

/*----------------------------e n d monsters �����������-------------------------------*/

#endif // !__H_SIMPLEC_CFGDATA