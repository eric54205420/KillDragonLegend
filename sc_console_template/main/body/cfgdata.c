#include <cfgdata.h>
#include <scconf.h>
#include <scjson.h>

// ���ý�������
struct cfgparse {
	// ��Ӧconfig.ini��������Ϣ
	char * inikey;
	// return < 0 ��ʾerror
	int (* parse)(const cjson_t);
};

/*----------------------------begin skill �����������-------------------------------*/
/*
 * �õ������й��ڼ������õ���������
 *		: ���ؼ��ܶ�������
 */
static array_t _cfg_skills;
inline const array_t
data_skills(void) {
	return _cfg_skills;
}

// ���м������������Ϣ, �õ��ͽ���
static int _cfg_get_skills(const cjson_t arrs) {
	int i, len;
	cjson_t arr, aidx;
	struct skill * node;

	// ���� _cfg_skills �����������
	_cfg_skills = array_new(0, sizeof(struct skill));

	len = cjson_getlen(arrs);
	assert(len >= 1);
	for (i = 0; i < len; ++i) {
		// �õ��ض���json ��������
		arr = cjson_getarray(arrs, i);
		// �õ���������һ���������
		node = array_push(_cfg_skills);

		// ���ζ�ȡ����������, 
		aidx = arr->child;
		strncpy(node->name, aidx->vs, _INT_KNAME);	// ��������

		aidx = aidx->next;
		strncpy(node->desc, aidx->vs, _INT_BDESC);	// ��������

		aidx = aidx->next;
		node->keepround = (unsigned short)aidx->vd;	// ���ֵĻغ���

		aidx = aidx->next;
		node->addblood = (int)aidx->vd;				// ���ӵ�Ѫ��

		aidx = aidx->next;
		node->addattack = (short)aidx->vd;			// �ٷֱ����ӹ�����

		aidx = aidx->next;
		node->adddefense = (short)aidx->vd;			// ���ӷ�����

		aidx = aidx->next;
		node->addacceleration = (short)aidx->vd;	// �����ٶ�

		aidx = aidx->next;
		node->addfortunately = (short)aidx->vd;		// ��������

		aidx = aidx->next;
		node->needmp = (short)aidx->vd;				// �ͷ���Ҫmp(ħ��ֵ)
		
		aidx = aidx->next;
		node->level = (unsigned short)aidx->vd;		// ��Ҫ�ĵȼ�

		aidx = aidx->next;
		node->isallowed = aidx->vd >= 1;			// �Ƿ������ͷ�

		// �򵥼��һ��
		if (NULL != aidx->next) {
			CERR("_get_cfg_skills NULL != aidx->next i = %d.", i);
			return -1;
		}
	}

	return 0;
}
/*----------------------------e n d skill �����������-------------------------------*/

/*----------------------------begin goods ��Ʒ�������-------------------------------*/

/*
 * �õ������й�����Ʒ��������������
 *		: ������Ʒ��������
 */
static array_t _cfg_goods;
inline const array_t 
data_goods(void) {
	return _cfg_goods;
}

// ������Ʒ���������Ϣ, �õ��ͽ���
static int _cfg_get_goods(const cjson_t arrs) {
	int i, len;
	cjson_t arr, aidx;
	struct good * node;

	// ���� _cfg_skills �����������
	_cfg_goods = array_new(0, sizeof(struct skill));

	len = cjson_getlen(arrs);
	assert(len >= 1);
	for (i = 0; i < len; ++i) {
		// �õ��ض���json ��������
		arr = cjson_getarray(arrs, i);
		// �õ���������һ���������
		node = array_push(_cfg_goods);

		// ���ζ�ȡ����������, 
		aidx = arr->child;
		strncpy(node->name, aidx->vs, _INT_KNAME);	// ��Ʒ����

		aidx = aidx->next;
		strncpy(node->desc, aidx->vs, _INT_KNAME);	// ��Ʒ����

		aidx = aidx->next;
		node->type = (unsigned char)aidx->vd;	// ��Ʒ����, see _GOOD_*
		assert(node->type >= _GOOD_DRUGS && node->type <= _GOOD_ELIXIR);
								
		aidx = aidx->next;
		node->addmp = (int)aidx->vd;				// ���ӵ�ħ��MPֵ

		aidx = aidx->next;
		node->addblood = (int)aidx->vd;				// ���ӵ�Ѫ��

		aidx = aidx->next;
		node->addattack = (short)aidx->vd;			// ���ӹ�����

		aidx = aidx->next;
		node->adddefense = (short)aidx->vd;			// ���ӷ�����

		aidx = aidx->next;
		node->addacceleration = (short)aidx->vd;	// �����ٶ�

		aidx = aidx->next;
		node->addfortunately = (short)aidx->vd;		// ��������

		aidx = aidx->next;
		node->pirce = (int)aidx->vd;				// ��Ʒ�۸�

		aidx = aidx->next;
		node->panel = (unsigned char)aidx->vd;		// ռ�ð���������

		aidx = aidx->next;
		node->addcrit = (short)aidx->vd;			// ���ӱ�����

		aidx = aidx->next;
		node->cnt = (unsigned short)aidx->vd;		// ��Ʒӵ������

		aidx = aidx->next;
		node->maxcnt = (unsigned short)aidx->vd;	// ��Ʒ���ӵ������
		
		aidx = aidx->next;
		node->isbuy = aidx->vd >= 1;				// ��Ʒ�Ƿ������
		
		aidx = aidx->next;						
		node->issell = aidx->vd >= 1;				// ��Ʒ�Ƿ������
		
		// �򵥼��һ��
		if (NULL != aidx->next) {
			CERR("_get_cfg_skills NULL != aidx->next i = %d.", i);
			return -1;
		}
	}

	return 0;
}

/*----------------------------e n d goods ��Ʒ�������-------------------------------*/

/*----------------------------begin maps ��ͼ�������-------------------------------*/



/*----------------------------e n d maps ��ͼ�������-------------------------------*/

/*
 * �õ������й��ڵ�ͼ��������������
 *		: ���ص�ͼ��������
 */
static array_t _cfg_maps;
inline const array_t 
data_maps(void) {
	return _cfg_maps;
}

// ����������, json -> ����ֵ
#define AIDXTONODE(type, val, msg) \
	node->##val = (type)aidx->vd;\
	aidx = aidx->next

// ���е�ͼ���������Ϣ, �õ��ͽ���
static int _cfg_get_maps(const cjson_t arrs) {
	int i, len;
	cjson_t arr, aidx;
	struct map * node;

	// ���� _cfg_skills �����������
	_cfg_maps = array_new(0, sizeof(struct skill));

	len = cjson_getlen(arrs);
	assert(len >= 1);
	for (i = 0; i < len; ++i) {
		// �õ��ض���json ��������, ���ҵõ��������������
		arr = cjson_getarray(arrs, i);
		aidx = arr->child;
		// �õ���������һ���������
		node = array_push(_cfg_maps);
		assert(arr && aidx && node);

		// ���ζ�ȡ����������, 
		strncpy(node->name, aidx->vs, _INT_KNAME);	// ��Ʒ����
		aidx = aidx->next;

		strncpy(node->desc, aidx->vs, _INT_SDESC);	// ��Ʒ����
		aidx = aidx->next;

		AIDXTONODE(short, start, "��ͼ�й������");
		AIDXTONODE(int, number, "��ͼ�й�������");
		AIDXTONODE(unsigned short, level, "��ͼ�й������Ƶȼ�");

		// �򵥼��һ��
		if (NULL != aidx) {
			CERR("_get_cfg_skills NULL != aidx->next i = %d.", i);
			return -1;
		}
	}

	return 0;
}

/*----------------------------begin monsters �����������-------------------------------*/

/*
 * �õ������й��ڹ����������������
 *		: ���ع����������
 */
static array_t _cfg_monsters;
inline const array_t 
data_monsters(void) {
	return _cfg_monsters;
}

// ���й������������Ϣ, �õ��ͽ���
static int _cfg_get_monsters(const cjson_t arrs) {
	int i, len;
	cjson_t arr, aidx;
	struct monster * node;

	// ���� _cfg_skills �����������
	_cfg_monsters = array_new(0, sizeof(struct skill));

	len = cjson_getlen(arrs);
	assert(len >= 1);
	for (i = 0; i < len; ++i) {
		// �õ��ض���json ��������, ���ҵõ��������������
		arr = cjson_getarray(arrs, i);
		aidx = arr->child;
		// �õ���������һ���������
		node = array_push(_cfg_monsters);
		assert(arr && aidx && node);

		// ���ζ�ȡ����������, 
		strncpy(node->name, aidx->vs, _INT_KNAME);	// ��������
		aidx = aidx->next;

		strncpy(node->desc, aidx->vs, _INT_SDESC);	// ��������
		aidx = aidx->next;

		AIDXTONODE(unsigned short, level, "����ȼ�");
		AIDXTONODE(short, attack, "������");
		AIDXTONODE(short, defense, "������");
		AIDXTONODE(short, acceleration, "�ٶ�");
		AIDXTONODE(int, blood, "��ǰѪ��");
		AIDXTONODE(int, maxblood, "���Ѫ��");
		AIDXTONODE(short, mp, "��ǰħ����");
		AIDXTONODE(short, maxmp, "���ħ����");
		AIDXTONODE(short, experience, "Я���ľ���");
		AIDXTONODE(short, gold, "Я���Ľ��");
		AIDXTONODE(bool, isdrugre, "Я���Ľ��");
		AIDXTONODE(bool, isweapons, "�Ƿ�Я���������");
		AIDXTONODE(bool, iselixir, "�Ƿ�Я���汦");
		AIDXTONODE(bool, isescape, " �Ƿ���������");

		// �򵥼��һ��
		if (NULL != aidx) {
			CERR("_get_cfg_skills NULL != aidx->next i = %d.", i);
			return -1;
		}
	}

	return 0;
}

/*----------------------------e n d monsters �����������-------------------------------*/


/*
 * ��������������ݻ�ȡ, ����
 */
void 
data_start(void) {
	const char * path;
	cjson_t arrs;
	int i = -1;

	// ��Ҫÿ��������ö�̬����������key��ӳ������������
	struct cfgparse cfgs[] = {
		{ "cfgdata_skills", _cfg_get_skills },
		{ "cfgdata_goods", _cfg_get_goods },
		{ "cfgdata_maps", _cfg_get_maps },
		{ "cfgdata_monsters", _cfg_get_monsters },
	};

	while (++i < LEN(cfgs)) {
		struct cfgparse * cfg = cfgs + i;

		// ��ʼ�������������
		path = mconf_get(cfg->inikey);
		if (!path || !*path)
			CERR_EXIT("mconf_get $CfgSkills is error");
		arrs = cjson_newfile(path);
		if (NULL == arrs)
			CERR_EXIT("cjson_newfile path = %s is error!", path);

		// ��ʼ�������������Ϣ
		if (0 > cfg->parse(arrs)) {
			cjson_delete(arrs);
			data_close();
			CERR_EXIT("cjson_newfile path = %s is error! need check!", path);
		}

		// �ͷ��Ѿ����������ʱjson��Ϣ��
		cjson_delete(arrs);
	}
}

/*
 * �������������������, �ͷ�
 */
inline void 
data_close(void) {
	array_die(_cfg_skills);
	array_die(_cfg_goods);
	array_die(_cfg_maps);
	array_die(_cfg_monsters);
}