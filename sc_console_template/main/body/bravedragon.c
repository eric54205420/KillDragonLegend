#include <bravedragon.h>
#include <cfgdata.h>
#include <scconf.h>

#ifdef __GNUC__  // ����������GCC������ʵ��

#	define BDCOLOR(v) \
		system("\033[" #v ";30m ")

#elif _MSC_VER // ����������Visual Studio������ʵ��

#	define BDCOLOR(v) \
		system("color " #v)

#else	
#	error "error : Currently only supports the Visual Studio and GCC!"
#endif

#define _INT_PAUSETIME	(1000)

// ������˵����ͷ��
static int _bd_head(void) {
	static bool _isstart = false; // true;

	// ֻ�е�һ�ν����ʱ��Ქ�Ŷ���
	if (_isstart) {
		_isstart = false;
		CONSOLECLS();
		BDCOLOR(31);
		puts("********************************************************************************\n");
		bd_animates("                              �� ��ӭ���롾������˵����\n", 50, _INT_PAUSETIME);
		puts("\n********************************************************************************");
		SLEEPMS(_INT_PAUSETIME);
	}

	CONSOLECLS();
	BDCOLOR(30);
	puts("********************************************************************************\n\n");
	puts("                                    ��������˵��\n");
	puts("                               [A]--�µ�����");
	puts("                               [B]--�ɵĻ���");
	puts("                               [C]--����ɽ��");
	puts("\n\n********************************************************************************");

	return toupper(sh_getch());
}

// ������˵�˳����
static bool _bd_end(void) {
	int ch;
	CONSOLECLS();
	printf("��ȷ��Ҫ�˳���Ϸ��<y/N>");
	ch = sh_getch();
	return ch == 'y' || ch == 'Y';
}

//��ɫ����ɫ�����ȼ����������������ٶȣ����ˣ���ң������жϣ� ��yesp=99���ж��û��Ѵ��ڣ������飬�������飬Ѫ����ħ��ֵ�� 
struct player {
	char name[_INT_KNAME];			// ��ɫ����
	unsigned short level;			// ��ɫ�ȼ�
	short attack;					// ������
	short defense;					// ������
	short acceleration;				// �ٶ� 
	short luck;						// ����
	int gold;						// ���ӵ�еĽ�ɫ��Ϣ
	int exp;						// ��ҵ�ǰ����
	int exped;						// ���������Ҫ�ľ���
	int hp;							// ��ҵ�ǰѪ��
	int hpmax;						// ������Ѫ��
	short mp;						// ��ҵ�ǰ����
	short mpmax;					// �����������
};

// ���ݼ��, ������ֱ���˳�
static void _bd_check(struct player * play) {
	if (play->level >= 101 
		|| play->gold > 200000
		|| play->acceleration > 1000 
		|| play->exp < 0
		|| play->exped > 100000) {
		bd_errs("���������쳣�����ܴ�������BUG���׻��ߵ�����ȡ�쳣����Ϸ���Զ�����,�밴�չ��������Ϸ!", "��BUG���桿", "����", 0);
		exit(EXIT_FAILURE);
	}
}

static void _bd_menuf_heroinfo(struct player * play) {
	char ch;
	bd_animatesa("", "����ɫ��Ϣ��", true, false, 0, 0);
	printf("\n\n������%s", play->name);
	printf("\n\n�ȼ�:%d (%d/%d)", play->level, play->exp, play->exped);
	printf("\nѪ��:%d/%d    ħ��:%d/%d", play->hp, play->hpmax, play->mp, play->mpmax);
	printf(" \n������:%d \n����:%d \n�ٶ�:%d", play->attack, play->defense, play->acceleration);
	printf(" \n����:%d \n���:%d  ", play->luck, play->gold);
	printf("\n\n[W]--װ��         [Y]--��Ʒ     ");
	printf("\n\n[K]--����         [Q]--����     ");

	for (;;) {
		ch = toupper(sh_getch());
		switch (ch) {
		case 'W': // ��Ҫ�Լ�ȥʵ��װ��ϵͳ

			break;
		case 'Y': // ����չʾ

			break;

		case 'K': // ��Ʒչʾ

			break;

		case 'Q': // ����������
			return;
		}
	}
}

static void _bd_menuf(struct player * play) {
	int ch;
	const char * help;
	_bd_check(play);
	BDCOLOR(30);

	help = mconf_get("bravedragon_help");
	for (;;) {

		// ����ͷ����Ϣ
		CONSOLECLS();
		printf("\n                            ��������˵��\n");
		printf("------------------------------------------------------------------------");
		printf("\n\n��ʿ��%s (%d��)", play->name, play->level);
		printf("\n\n[A]--������Ϣ       [B]--����              [C]--�̵�");
		printf("\n\n[D]--��ս����       [E]--����              [F]--����");
		printf("\n\n[G]--С��Ϸ         [H]--����");

		ch = sh_getch();
		ch = toupper(ch);

		switch (ch) {
		case 'A':

			break;
		case 'B':
			break;
		case 'C':
			break;
		case 'D':
			break;
		case 'E':
			break;
		case 'F':
			break;
		case 'G':
			break;

		case 'H': // ��Ϸ����ĳ��
			bd_animatesa(help, "����Ϸ������", true, false, 0, 0);
			sh_getch();
			break;
		}
	}
}

// �µ�����
static void _bd_newgame(struct player * play) {
	static bool _iswelcome = true;
	char name[_INT_KNAME + 1];

	CONSOLECLS();

	// �����µ���Ϸ��
	if (_iswelcome) {
		_iswelcome = false;
		bd_animatesa(mconf_get("bravedragon_main"), mconf_get("bravedragon_title"),
			true, false, 0, _INT_PAUSETIME << 1);
	}

	CONSOLECLS();
	bd_animatesa("��ӭ�㣡��ʿ���������������֣�", "����ʿע�᡿", 1, 0, 0, 0);
	// ��������
	if (fgets(name, _INT_KNAME, stdin) == NULL) {
		bd_errs("��ʿ, �����Ҹ㰡!!!!!!", "�����桿", "��ͨ", 0);
		SLEEPMS(_INT_KNAME);
		return;
	}

	// ȥ��\n
	name[strlen(name) - 1] = '\0';

	// ��ʼ�������ݶ���
	strcpy(play->name, name);
	play->level = 1;					// ��ɫ�ȼ�
	play->attack = 5;					// ������
	play->defense = 8;					// ������
	play->acceleration = 3;				// �ٶ� 
	play->luck = 5;						// ����
	play->gold = 1000;					// ���ӵ�еĽ�ɫ��Ϣ
	play->exp = 0;						// ��ҵ�ǰ����
	play->exped = play->level * 50;		// ���������Ҫ�ľ���
	play->hp = 100;						// ��ҵ�ǰѪ��
	play->hpmax = 100;					// ������Ѫ��
	play->mp = 50;						// ��ҵ�ǰ����
	play->mpmax = 50;					// �����������

	// ���濪ʼ���µ�Ŀ¼����
	_bd_menuf(play);
}

// ������˵��������
void 
bd_main(void) {
	// ��Ҷ�����Ϣ
	struct player hero;

	// ����������Ϣ
	data_start();

	for (;;) {
		// ���Ŷ���, ���õ�������� ��д�����ַ�
		char ch = _bd_head();

		switch (ch) {
		case 'A':						// �µ�����
			_bd_newgame(&hero);
			break;	
		case 'B':						// �ɵĻ���
			break;
		case 'C':						// ����ɽ��
			if (_bd_end())
				goto _end;
			break;
		}
	}

_end:
	// �ͷ�������Ϣ
	data_close();
}

// ����ı�str, ÿ��ӡһ���ַ�ͣ��tn����, ���ͣ��tb����
void 
bd_animates(const char * str, int tn, int tb) {
	char c;
	while ((c = *str++)) {
		putchar(c);
		SLEEPMS(tn);
	}

	SLEEPMS(tb);
}

// �������������Ϣ, errs:������Ϣ, title����, levs����ȼ�, iscls�Ƿ����ͷ������
void 
bd_errs(const char * errs, const char * title,
		const char * levs, bool iscls) {
	int i = 0, c;

	if (iscls)
		CONSOLECLS();

	printf("\n\n\a                    %s\n", title);
	printf("********************************************************************************\n");
	printf("\n\n������ȼ�����%s\n\n", levs);
	printf("������ԭ�򡿣�\n ");

	while ((c = *errs++)) {
		putchar(c);
	}

	sh_getch();
}

// ���Ƹ�ǿ�Ĵ�ӡ����
void 
bd_animatesa(const char * str, const char * title,
			 bool isclsh, bool isclsb, int tn, int tb) {
	int c, i = 0;

	if (isclsh)
		CONSOLECLS();

	printf("                    %s\n", title);
	while ((c = *str++)) {
		putchar(c);
		SLEEPMS(tn);
	}

	if (isclsb)
		CONSOLECLS();
	SLEEPMS(tb);
}

// �����ӡ����, �������ű���
void 
bd_butis(const char * str, const char * title) {
	CONSOLECLS();
	printf("\n                              %s\n", title);
	for (int i = 0; i < 80; ++i)
		printf("%s", str);
	putchar('\n');
}

// ��ӡ��������Ϣ
void 
bd_mms(char str[], int len) {
	putchar('\n');
	for (int i = 0; i < len; ++i)
		printf("%s", str);
}
