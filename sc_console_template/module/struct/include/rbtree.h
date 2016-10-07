#ifndef _H_RBTREE
#define _H_RBTREE

#include <schead.h>

struct rbnode {
	unsigned long	parent_color;
	struct rbnode * right;
	struct rbnode * left;
};

typedef struct {
	struct rbnode * root;
	new_f new;
	cmp_f cmp;
	die_f die;
} * rbtree_t;

/*
 * ÿ����ʹ�ú�����Ľṹ, ��Ҫ��ͷ�����������. 
 * ���� :
	struct person {
		_HEAD_RBTREE;
		... // �Զ�����Ϣ
	};
 */
#define _HEAD_RBTREE	struct rbnode __node

/*
 * ����һ�ź����ͷ��� 
 * new		: ע�ᴴ�����ĺ���
 * cmp		: ע��Ƚϵĺ���
 * die		: ע��������ٺ���
 *			: ���ش����õĺ�������
 */
extern rbtree_t rb_new(new_f new, cmp_f cmp, die_f die);

/*
 * ����һ�����, ����� new(pack)
 * root		: �����ͷ���
 * pack		: ������Ľ�㵱cmp(x, pack) �ҽ��
 */
extern void rb_insert(rbtree_t tree, void * pack);

/*
 * ɾ���ܺ�packƥ��Ľ��
 * root		: ��������
 * pack		: ��cmp(x, pack) �ҽ��
 */
extern void rb_remove(rbtree_t tree, void * pack);

/*
 * �õ��������ƥ��Ľ��
 * root		: ƥ��Ľ����Ϣ
 * pack		: ��ǰ��ƥ����, cmp(x, pack)���ҽ�㴦��
 */
extern void * rb_get(rbtree_t tree, void * pack);

/*
 * ������Ŷ�����
 * root		: ��ǰ��������
 */
extern void rb_die(rbtree_t tree);

#endif /* _H_RBTREE */