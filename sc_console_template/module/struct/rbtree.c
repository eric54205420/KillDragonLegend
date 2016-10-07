#include "rbtree.h"

/*
 * ����������, �õ�������о��常���, ��ɫ. ������ϸ������Ϣ
 * r	: ͷ���
 * p	: �������ֵ
 * c	: ��ǰ��ɫ
 */
#define rb_parent(r)		((struct rbnode *)((r)->parent_color & ~3))
#define rb_color(r)			((r)->parent_color & 1)
#define rb_is_red(r)		(!rb_color(r))
#define rb_is_black(r)		rb_color(r)
#define rb_set_black(r)		(r)->parent_color |= 1
#define rb_set_red(r)		(r)->parent_color &= ~1

static inline void rb_set_parent(struct rbnode * r, struct rbnode * p) {
     r->parent_color = (r->parent_color & 3) | (unsigned long)p;
}

static inline void rb_set_color(struct rbnode * r, int color) {
     r->parent_color = (r->parent_color & ~1) | (1 & color);
}

static inline int _rb_cmp(const void * ln, const void * rn) {
	return (const char *)ln - (const char *)rn;
}

 /*
  * ����һ�ź����ͷ���
  * new		: ע�ᴴ�����ĺ���
  * cmp		: ע��Ƚϵĺ���
  * die		: ע��������ٺ���
  *			: ���ش����õĺ�������
  */
rbtree_t 
rb_new(new_f new, cmp_f cmp, die_f die) {
	rbtree_t tree = malloc(sizeof(*tree));
	if(NULL == tree) {
		CERR("rb_new malloc is error!");
		return NULL;	
	}
	
	tree->root = NULL;
	tree->new = new;
	tree->cmp = cmp ? cmp : _rb_cmp;
	tree->die = die;

	return tree;
}

static inline struct rbnode * _rb_new(rbtree_t tree, void * pack) {
	struct rbnode * node = tree->new ? tree->new(pack) : pack;
	memset(node, 0, sizeof(struct rbnode));
	return node;
}

/* 
 * �Ժ�����Ľڵ�(x)��������ת
 *
 * ����ʾ��ͼ(�Խڵ�x��������)��
 *      px                              px
 *     /                               /
 *    x                               y                
 *   /  \      --(����)-->           / \                #
 *  lx   y                          x  ry     
 *     /   \                       /  \
 *    ly   ry                     lx  ly  
 *
 */
static void _rbtree_left_rotate(rbtree_t tree, struct rbnode * x) {
    // ����x���Һ���Ϊy
    struct rbnode * y = x->right;
	struct rbnode * xparent = rb_parent(x);

    // �� ��y�����ӡ� ��Ϊ ��x���Һ��ӡ���
    x->right = y->left;
	// ���y�����ӷǿգ��� ��x�� ��Ϊ ��y�����ӵĸ��ס�
    if (y->left != NULL)
		rb_set_parent(y->left, x);

    // �� ��x�ĸ��ס� ��Ϊ ��y�ĸ��ס�
	rb_set_parent(y, xparent);

    if (xparent == NULL)
        tree->root = y;            // ��� ��x�ĸ��ס� �ǿսڵ㣬��y��Ϊ���ڵ�
    else {
        if (xparent->left == x)
            xparent->left = y;     // ��� x�������ڵ�����ӣ���y��Ϊ��x�ĸ��ڵ�����ӡ�
        else
            xparent->right = y;    // ��� x�������ڵ�����ӣ���y��Ϊ��x�ĸ��ڵ�����ӡ�
    }
    
    // �� ��x�� ��Ϊ ��y�����ӡ�
    y->left = x;
    // �� ��x�ĸ��ڵ㡱 ��Ϊ ��y��
	rb_set_parent(x, y);
}

/* 
 * �Ժ�����Ľڵ�(y)��������ת
 *
 * ����ʾ��ͼ(�Խڵ�y��������)��
 *            py                               py
 *           /                                /
 *          y                                x                  
 *         /  \      --(����)-->            /  \                     #
 *        x   ry                           lx   y  
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 * 
 */
static void _rbtree_right_rotate(rbtree_t tree, struct rbnode * y) {
    // ����x�ǵ�ǰ�ڵ�����ӡ�
    struct rbnode * x = y->left;
	struct rbnode * yparent = rb_parent(y);

    // �� ��x���Һ��ӡ� ��Ϊ ��y�����ӡ���
	y->left = x->right;
    // ���"x���Һ���"��Ϊ�յĻ����� ��y�� ��Ϊ ��x���Һ��ӵĸ��ס�
    if (x->right != NULL)
		rb_set_parent(x->right, y);

    // �� ��y�ĸ��ס� ��Ϊ ��x�ĸ��ס�
    rb_set_parent(x, yparent);
    if (yparent == NULL) 
        tree->root = x;				// ��� ��y�ĸ��ס� �ǿսڵ㣬��x��Ϊ���ڵ�
    else {
        if (y == yparent->right)
            yparent->right = x;		// ��� y�������ڵ���Һ��ӣ���x��Ϊ��y�ĸ��ڵ���Һ��ӡ�
        else
            yparent->left = x;		// (y�������ڵ������) ��x��Ϊ��x�ĸ��ڵ�����ӡ�
    }

    // �� ��y�� ��Ϊ ��x���Һ��ӡ�
    x->right = y;
    // �� ��y�ĸ��ڵ㡱 ��Ϊ ��x��
	rb_set_parent(y, x);
}

/*
 * �����������������
 *
 * ���������в���ڵ�֮��(ʧȥƽ��)���ٵ��øú�����
 * Ŀ���ǽ������������һ�ź������
 *
 * ����˵����
 *     tree ������ĸ�
 *     node ����Ľ��        // ��Ӧ���㷨���ۡ��е�z
 */
static void _rbtree_insert_fixup(rbtree_t tree, struct rbnode * node) {
    struct rbnode * parent, * gparent, * uncle;

    // �������ڵ���ڣ����Ҹ��ڵ����ɫ�Ǻ�ɫ��
    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        gparent = rb_parent(parent);

        //�������ڵ㡱�ǡ��游�ڵ�����ӡ�
        if (parent == gparent->left) {
            // Case 1����������ڵ��Ǻ�ɫ
            uncle = gparent->right;
            if (uncle && rb_is_red(uncle)) {
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(gparent);
                node = gparent;
                continue;
            }

            // Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ���
            if (parent->right == node) {
                _rbtree_left_rotate(tree, parent);
                uncle = parent;
                parent = node;
                node = uncle;
            }

            // Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ������ӡ�
            rb_set_black(parent);
            rb_set_red(gparent);
            _rbtree_right_rotate(tree, gparent);
        } 
        else { //����z�ĸ��ڵ㡱�ǡ�z���游�ڵ���Һ��ӡ�
            // Case 1����������ڵ��Ǻ�ɫ
            uncle = gparent->left;
            if (uncle && rb_is_red(uncle)) {
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(gparent);
                node = gparent;
                continue;
            }

            // Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ�������
            if (parent->left == node) {
                _rbtree_right_rotate(tree, parent);
                uncle = parent;
                parent = node;
                node = uncle;
            }

            // Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ��ӡ�
            rb_set_black(parent);
            rb_set_red(gparent);
            _rbtree_left_rotate(tree, gparent);
        }
    }

    // �����ڵ���Ϊ��ɫ
    rb_set_black(tree->root);
}

/*
 * ����һ�����, ����� new(pack)
 * tree		: �����ͷ���
 * pack		: ������Ľ�㵱cmp(x, pack) �ҽ��
 */
void 
rb_insert(rbtree_t tree, void * pack) {
	cmp_f cmp;
	struct rbnode * node, * x, * y;
	if((!tree) || (!pack) || !(node = _rb_new(tree, pack))) {
		CERR("rb_insert param is empty! tree = %p, pack = %p.\n", tree, pack);
		return;	
	}
	
	cmp = tree->cmp;
	// ��ʼ�߲��빤��
	y = NULL;
	x = tree->root;

	// 1. �����������һ�Ŷ�������������ڵ���ӵ�����������С���С����
	while (x != NULL) {
		y = x;
		if (cmp(x, node) > 0)
			x = x->left;
		else
			x = x->right;
	}
	rb_set_parent(node, y);

	if (y != NULL) {
		if (cmp(y, node) > 0)
			y->left = node;             // ���2������node��������ֵ�� < ��y��������ֵ������node��Ϊ��y�����ӡ�
		else
			y->right = node;            // ���3��(��node��������ֵ�� >= ��y��������ֵ��)��node��Ϊ��y���Һ��ӡ� 
	}
	else
		tree->root = node;              // ���1����y�ǿսڵ㣬��node��Ϊ��

	// 2. ���ýڵ����ɫΪ��ɫ
	rb_set_red(node);

	// 3. ������������Ϊһ�Ŷ��������
	_rbtree_insert_fixup(tree, node);
}

/*
 * �����ɾ����������
 *
 * �ڴӺ������ɾ������ڵ�֮��(�����ʧȥƽ��)���ٵ��øú�����
 * Ŀ���ǽ������������һ�ź������
 *
 * ����˵����
 *     tree ������ĸ�
 *     node �������Ľڵ�
 */
static void _rbtree_delete_fixup(rbtree_t tree, struct rbnode * node, struct rbnode * parent) {
    struct rbnode * other;

    while ((!node || rb_is_black(node)) && node != tree->root) {
        if (parent->left == node) {
            other = parent->right;
            if (rb_is_red(other)) {
                // Case 1: x���ֵ�w�Ǻ�ɫ��  
                rb_set_black(other);
                rb_set_red(parent);
                _rbtree_left_rotate(tree, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                // Case 2: x���ֵ�w�Ǻ�ɫ����w����������Ҳ���Ǻ�ɫ��  
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else {
                if (!other->right || rb_is_black(other->right)) {
                    // Case 3: x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ��  
                    rb_set_black(other->left);
                    rb_set_red(other);
                    _rbtree_right_rotate(tree, other);
                    other = parent->right;
                }
                // Case 4: x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ��
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                _rbtree_left_rotate(tree, parent);
                node = tree->root;
                break;
            }
        }
        else {
            other = parent->left;
            if (rb_is_red(other)) {
                // Case 1: x���ֵ�w�Ǻ�ɫ��  
                rb_set_black(other);
                rb_set_red(parent);
                _rbtree_right_rotate(tree, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                // Case 2: x���ֵ�w�Ǻ�ɫ����w����������Ҳ���Ǻ�ɫ��  
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else {
                if (!other->left || rb_is_black(other->left)) {
                    // Case 3: x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ��  
                    rb_set_black(other->right);
                    rb_set_red(other);
                    _rbtree_left_rotate(tree, other);
                    other = parent->left;
                }
                // Case 4: x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ��
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                _rbtree_right_rotate(tree, parent);
                node = tree->root;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

/*
 * ɾ��rb_get�õ��Ľ��
 * root		: ��������
 * pack		: ��cmp(x, pack) �ҽ��
 */
void 
rb_remove(rbtree_t tree, void * pack) {
	struct rbnode * child, * parent, * node = NULL;
	int color;
	
	if ((!tree) || !(node = (struct rbnode *)pack)) {
		CERR("rb_remove check is error, tree = %p, node = %p.", tree, node);
		return;
	}

	// ��ɾ���ڵ��"���Һ��Ӷ���Ϊ��"�������
	if (NULL != node->left && node->right != NULL) {
		// ��ɾ�ڵ�ĺ�̽ڵ㡣(��Ϊ"ȡ���ڵ�")
		// ������ȡ��"��ɾ�ڵ�"��λ�ã�Ȼ���ٽ�"��ɾ�ڵ�"ȥ����
		struct rbnode * replace = node;

		// ��ȡ��̽ڵ�
		replace = replace->right;
		while (replace->left != NULL)
			replace = replace->left;

		// "node�ڵ�"���Ǹ��ڵ�(ֻ�и��ڵ㲻���ڸ��ڵ�)
		if ((parent = rb_parent(node))) {
			if (parent->left == node)
				parent->left = replace;
			else
				parent->right = replace;
		} 
		else 
			// "node�ڵ�"�Ǹ��ڵ㣬���¸��ڵ㡣
			tree->root = replace;

		// child��"ȡ���ڵ�"���Һ��ӣ�Ҳ����Ҫ"�����Ľڵ�"��
		// "ȡ���ڵ�"�϶����������ӣ���Ϊ����һ����̽ڵ㡣
		child = replace->right;
		parent = rb_parent(replace);
		// ����"ȡ���ڵ�"����ɫ
		color = rb_color(replace);

		// "��ɾ���ڵ�"��"���ĺ�̽ڵ�ĸ��ڵ�"
		if (parent == node)
			parent = replace; 
		else {
			// child��Ϊ��
			if (child)
				rb_set_parent(child, parent);
			parent->left = child;

			replace->right = node->right;
			rb_set_parent(node->right, replace);
		}
		
		rb_set_parent(replace, rb_parent(node));
		rb_set_color(replace, rb_color(node));
		replace->left = node->left;
		rb_set_parent(node->left, replace);

		if (color) // ��ɫ������µ�����ϵ
			_rbtree_delete_fixup(tree, child, parent);
		// ������ٲ���
		if(tree->die)
			tree->die(node);
		return ;
	}

	if (node->left !=NULL)
		child = node->left;
	else 
		child = node->right;

	parent = rb_parent(node);
	// ����"ȡ���ڵ�"����ɫ
	color = rb_color(node);

	if (child)
		rb_set_parent(child, parent);

	// "node�ڵ�"���Ǹ��ڵ�
	if (parent) {
		if (parent->left == node)
			parent->left = child;
		else
			parent->right = child;
	}
	else
		tree->root = child;

	if (!color)
		_rbtree_delete_fixup(tree, child, parent);
	if(tree->die)
		tree->die(node);
}

/*
 * �õ��������ƥ��Ľ��
 * root		: ƥ��Ľ����Ϣ
 * pack		: ��ǰ��ƥ����, cmp(x, pack)���ҽ�㴦��
 */
void * 
rb_get(rbtree_t tree, void * pack) {
	cmp_f cmp;
	struct rbnode * node;
	if((!tree) || !pack) {
		CERR("rb_get param is empty! tree = %p, pack = %p.\n", tree, pack);
		return NULL;	
	}
	
	cmp = tree->cmp;
	node = tree->root;
	while(node) {
		int ct = cmp(node, pack);
		if(ct == 0)
			return node;
		node = ct > 0 ? node->left : node->right;
	}

	return NULL;
}

// �������ɾ������
static void _rb_die(struct rbnode * root, die_f die) {
	if(NULL == root)
		return;
	_rb_die(root->left, die);
	_rb_die(root->right, die);
	die(root);
}

/*
 * ������Ŷ�����
 * root		: ��ǰ��������
 */
void
rb_die(rbtree_t tree) {
	if(!tree || !tree->root || !tree->die)
		return;

	// �����ݹ�ɾ��
	_rb_die(tree->root, tree->die);

	// �����������ڴ�
	tree->root = NULL;
	free(tree);
}