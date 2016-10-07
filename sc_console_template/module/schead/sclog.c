#include "sclog.h"
#include "scatom.h"
#include "pthread.h"
#include <stdarg.h>

//-------------------------------------------------------------------------------------------|
// �ڶ����� ����־��Ϣ�������get��set,������������Ϣ���ʵ��
//-------------------------------------------------------------------------------------------|

//�����ض���� ����Ӧ�� �� "mkdir -p \"" _STR_SCLOG_PATH "\" >" _STR_TOOUT " 2>" _STR_TOERR
#define _STR_TOOUT "__out__"
#define _STR_TOERR "__err__"
#define _STR_LOGID _STR_SCLOG_DIR "/__lgd__" // ����logid,�־û�

static struct {
	pthread_key_t	key;	//ȫ���߳�˽�б���
	pthread_once_t	once;	//ȫ�ֳ�ʼ���õ�����
	unsigned		logid;	//Ĭ�ϵ�ȫ��logid, Ψһ��ʶ
	FILE *			log;	//log�ļ�ָ��
	FILE *			wf;		//wf�ļ�ָ��
} _slmain = { (pthread_key_t)0, PTHREAD_ONCE_INIT, 0, NULL, NULL };

//�ڲ��򵥵��ͷź���,������pthread_key_create ��ֹ�߳���Դй¶
static void _slinfo_destroy(void * slinfo) {
	free(slinfo);
}

static void _gkey(void) {
	pthread_key_create(&_slmain.key, _slinfo_destroy);
}

struct slinfo {
	unsigned		logid;					//�����logid,Ψһid
	char			reqip[_INT_LITTLE];		//����ip
	char			times[_INT_LITTLE];		//��ǰʱ�䴮
	struct timeval	timev;					//����ʱ��,����ֵ,ͳһ�ú���
	char			mod[_INT_LITTLE];		//��ǰ�̵߳�ģ������,���ܳ���_INT_LITTLE - 1
};

/**
*	�̵߳�˽�����ݳ�ʼ��
**
** mod   : ��ǰ�߳�����
** reqip : �����ip
** logid : �����Ψһ��ʶid, Ĭ��0
** return :	_RT_OK ��ʾ����,_RF_EM�ڴ�������
**/
int
sl_init(const char mod[_INT_LITTLE], const char reqip[_INT_LITTLE], unsigned logid) {
	struct slinfo * pl;

	//��֤ _gkeyֻ��ִ��һ��
	pthread_once(&_slmain.once, _gkey);

	if ((pl = pthread_getspecific(_slmain.key)) == NULL) {
		//���¹���
		if ((pl = malloc(sizeof(struct slinfo))) == NULL)
			return RT_ErrorMalloc;
	}

	gettimeofday(&pl->timev, NULL);
	//������־logid, ������, û��Ĭ��ԭ������
	pl->logid = logid ? logid : ATOM_ADD_FETCH(_slmain.logid, 1);
	strncpy(pl->mod, mod, _INT_LITTLE); //����һЩ����
	strncpy(pl->reqip, reqip, _INT_LITTLE);

	//����˽�б���
	pthread_setspecific(_slmain.key, pl);

	return RT_SuccessBase;
}

/**
*	��ȡ��־��Ϣ���Ψһ��logid
**/
unsigned
sl_getlogid(void) {
	struct slinfo * pl = pthread_getspecific(_slmain.key);
	if (NULL == pl) //����0��ʾû���Ҽ�
		return 0u;
	return pl->logid;
}

/**
*	��ȡ��־��Ϣ�������ip��,����NULL��ʾû�г�ʼ��
**/
const char*
sl_getreqip(void) {
	struct slinfo * pl = pthread_getspecific(_slmain.key);
	if (NULL == pl) //����NULL��ʾû���Ҽ�
		return NULL;
	return pl->reqip;
}

/**
*	��ȡ��־��Ϣ�������,����NULL��ʾû�г�ʼ��
**/
const char*
sl_getmod(void)
{
	struct slinfo * pl = pthread_getspecific(_slmain.key);
	if (NULL == pl) //����NULL��ʾû���Ҽ�
		return NULL;
	return pl->mod;
}


//-------------------------------------------------------------------------------------------|
// �������� ����־ϵͳ������������ӿڲ���
//-------------------------------------------------------------------------------------------|

/**
*	��־�ر�ʱ��ִ��,����ӿ�,�رմ򿪵��ļ����
**/
static void _sl_end(void)
{
	FILE* lid;
	void* pl;

	// �ڼ򵥵ط�������ȫ����ֵ��,�ں��ĵط����㷨�Ż��Ĳ����ȹ�
	if (NULL == _slmain.log)
		return;

	//���õ�ǰϵͳ���ļ��ṹ��
	fclose(_slmain.log);
	fclose(_slmain.wf);

	//д���ļ�
	lid = fopen(_STR_LOGID, "wb");
	if (NULL != lid) {
		fprintf(lid, "%u", _slmain.logid);
		fclose(lid);
	}

	BZERO(_slmain);

	//�����ͷ�˽�б���,��ʵ������ �൱��һ���߳��ǲ������!���ǲ�ͬ���������ڵ�
	pl = pthread_getspecific(_slmain.key);
	_slinfo_destroy(pl);
	pthread_setspecific(_slmain.key, NULL);
}

/**
*	��־ϵͳ�״�ʹ�ó�ʼ��,�ҶԶ�ӳ��־�ļ�·��,����ָ��·��
**/
void
sl_start(void)
{
	FILE *lid;

	//����ִֻ��һ��
	if (NULL == _slmain.log) {
		//�ȶ༶����Ŀ¼,���ײ�������ʵ�ֿ�ƽ̨,system����ֵ�Ǻܸ���,Ĭ�ϳɹ�!
		system("mkdir -p \"" _STR_SCLOG_DIR "\" >" _STR_TOOUT " 2>" _STR_TOERR);
		rmdir("-p");
		remove(_STR_TOOUT);
		remove(_STR_TOERR);
	}

	if (NULL == _slmain.log) {
		_slmain.log = fopen(_STR_SCLOG_DIR "/" _STR_SCLOG_LOG, "a+");
		if (NULL == _slmain.log)
			CERR_EXIT("__slmain.log fopen %s error!", _STR_SCLOG_LOG);
	}
	//������ wf �ļ�
	if (NULL == _slmain.wf) {
		_slmain.wf = fopen(_STR_SCLOG_DIR "/" _STR_SCLOG_WFLOG, "a+");
		if (!_slmain.wf) {
			fclose(_slmain.log); //��ʵ�ⶼû�б�Ҫ,ͼ���İ�
			CERR_EXIT("__slmain.log fopen %s error!", _STR_SCLOG_WFLOG);
		}
	}

	//��ȡ�ļ�����,��ȡ�ļ�����,�־û�
	if ((lid = fopen(_STR_LOGID, "rb")) != NULL)
		fscanf(lid, "%u", &_slmain.logid);

	//���ж��Ƿ��г�ʼ���ı�Ҫ
	if (_slmain.log && _slmain.wf) {
		//������Ե�������һ���̻߳����,������־������ ���ģ���������ά��,���չ����
		sl_init("main thread", "0.0.0.0", 0);

		//ע���˳�����
		atexit(_sl_end);
	}
}

/**
*	��ȡ��־��Ϣ���ʱ�䴮,����NULL��ʾû�г�ʼ��
**/
static const char* _sl_gettimes(void) {
	struct timeval et; //��¼ʱ��
	unsigned td;

	struct slinfo* pl = pthread_getspecific(_slmain.key);
	if (NULL == pl) //����NULL��ʾû���Ҽ�
		return NULL;

	gettimeofday(&et, NULL);
	//ͬһ��΢���
	td = 1000000 * (et.tv_sec - pl->timev.tv_sec) + et.tv_usec - pl->timev.tv_usec;
	snprintf(pl->times, LEN(pl->times), "%u", td);

	return pl->times;
}

int
sl_printf(const char* format, ...)
{
	int len;
	va_list ap;
	char logs[_INT_LOG]; //�������һ���õ����,����c ��֧�� int a[n];
	stime_t tstr;

	if (NULL == _slmain.log) {
		CERR("%s fopen %s | %s error!", _STR_SCLOG_DIR, _STR_SCLOG_LOG, _STR_SCLOG_WFLOG);
		return RT_ErrorFopen;
	}

	//��ʼ��ʱ�����
	stu_getntstr(tstr);
	len = snprintf(logs, LEN(logs), "[%s %s]", tstr, _sl_gettimes());
	va_start(ap, format);
	vsnprintf(logs + len, LEN(logs) - len, format, ap);
	va_end(ap);

	// д��ͨ�ļ� log
	fputs(logs, _slmain.log); //��������ȥ����,fputs�����̰߳�ȫ��

							  // д�����ļ� wf
	if (format[1] == 'F' || format[1] == 'W') //��ΪFATAL��WARNING��ҪЩд�뵽�����ļ���
		fputs(logs, _slmain.wf);

	return RT_SuccessBase;
}