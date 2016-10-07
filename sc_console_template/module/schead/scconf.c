#include <scconf.h>
#include <tree.h>
#include <tstr.h>
#include <sclog.h>

//�򵥶������ṹ
struct sconf {
	_TREE_HEAD;
	char * key;
	char * value;
};

// ������������, kv �� [ abc\012345 ]�����Ľṹ
static void* _sconf_new(tstr_t tstr) {
	char* ptr; //��ʱ�õı���
	struct sconf * node = sm_malloc(sizeof(struct sconf) + sizeof(char)*(tstr->len+1));
	// �����, ʣ�µľ����˸�, 1% ,������,
	node->key = ptr = (char*)node + sizeof(struct sconf);
	memcpy(ptr, tstr->str, tstr->len + 1);
	// �ҵ���һ���ָ���
	while (*ptr++)
		;
	node->value = ptr;

	return node;
}

// ��ʼ���
static inline int _sconf_acmp(tstr_t tstr, struct sconf * rnode) {
	return strcmp(tstr->str, rnode->key);
}

//���Һ�ɾ��
static inline int _sconf_gdcmp(const char * lstr, struct sconf * rnode) {
	return strcmp(lstr, rnode->key);
}

static inline void _sconf_del(void * arg) {
	sm_free(arg);
}

/*
 * ����sconf_new���صĶ���
 */
inline void 
sconf_delete(sconf_t conf) {
	tree_destroy((tree_t *)&conf);
}

// ��ʼ������
static void _analysis_start(FILE * txt, tree_t * proot) {
	char c, n;
	TSTR_NEW(tstr);

	//���ﴦ���ȡ����
	while ((c = fgetc(txt)) != EOF) {
		//1.0 �������հ��ַ�
		while ((c != EOF) && sh_isspace(c))
			c = fgetc(txt);

		//2.0 ���������һ���ַ����� '$'
		if (c != '$') { //����һ�ж�ȡ���
			while (c != EOF && c != '\n')
				c = fgetc(txt);
			continue;
		}
		//2.1 ��һ���ַ��� $ �Ϸ��ַ�, ��ͷ�����ǿո�,����Ҳ��ȡ���
		if ((c = fgetc(txt)) != EOF && isspace(c)) {
			while (c != EOF && c != '\n')
				c = fgetc(txt);
			continue;
		}
		//��ʼ��¼��
		tstr->len = 0;

		//3.0 �ҵ���һ���Ⱥ� 
		while (c != EOF && c != '=') {
			if(!sh_isspace(c))
				tstr_append(tstr, c);
			c = fgetc(txt);
		}
		if (c != '=') //��Ч�Ľ���ֱ�ӽ���
			break;

		c = '\0';
		//4.0 �ҵ� ��һ�� "
		while (c != EOF && c != '\"') {
			if (!sh_isspace(c))
				tstr_append(tstr, c);
			c = fgetc(txt);
		}
		if (c != '\"') //��Ч�Ľ���ֱ�ӽ���
			break;

		//4.1 Ѱ�ҵڶ����Ⱥ�
		for (n = c; (c = fgetc(txt)) != EOF; n = c) {
			if (c == '\"' ) {
				if (n != '\\')
					break;
				// ����һ�� '\\' �ַ�
				--tstr->len;
			}
			tstr_append(tstr, c);
		}
		if (c != '\"') //��Ч�Ľ���ֱ�ӽ���
			break;

		//������ǺϷ��ַ���,��ʼ��� ��, 
		tree_add(proot, tstr);

		//����ȡ����ĩβ
		while (c != EOF && c != '\n')
			c = fgetc(txt);
		if (c != '\n')
			break;
	}

	TSTR_DELETE(tstr);
}

/*
* ͨ���ƶ�����·���������������, ʧ�ܷ���NULL
* path	: ��������·��
*		: ���ؽ���������ö���
*/
extern 
sconf_t sconf_new(const char * path) {
	tree_t conf = NULL;
	
	FILE * txt = fopen(path, "rb");
	if (NULL == txt) {
		SL_WARNING("fopen  r is error! path = %s.", path);
		return NULL;
	}

	// �����������ö���������
	conf = tree_create(_sconf_new, _sconf_acmp, _sconf_gdcmp, _sconf_del);
	// ������Ӿ�������
	_analysis_start(txt, &conf);

	fclose(txt);
	return conf;
}

/*
 * �õ������о�������
 * conf	: sconf_new ���ص����ö���
 * key	: �����ֵ
 *		: �ɹ��õ��������õĴ�, ʧ�ܻ򲻴��ڷ���NULL
 */
extern const char * 
sconf_get(sconf_t conf, const char * key) {
	struct sconf * kv = NULL;
	if ((!conf) || (!key) || (!*key) || !(kv = tree_get(conf, key, NULL))) {
		SL_WARNING("conf, key, kv => %p, %p, %p.", conf, key, kv);
		return NULL;
	}
	return kv->value;
}

// �����õ�������
static tree_t _mconf;
/*
 * ����������ϵͳ, ֻ����ϵͳ������ʱ��ִ��һ��
 *		: ���ش����õ������ö���
 */
inline sconf_t 
mconf_start(void) {
	return _mconf ? (_mconf) : (_mconf = sconf_new(_STR_MCCONF_PATH));
}

/*
 * �õ������ö��������õ�����, �����ڳ�������ʱ����ִ�� mconf_new
 * key	: �����õ�keyֵ
 *		: �õ������������õ�����
 */
inline const char * 
mconf_get(const char * key) {
	return sconf_get(_mconf, key);
}