#include <schead.h>
#include <sclog.h>
#include <sccsv.h>
#include <tstr.h>

// ��csvת����json�ļ����, �ɹ�����0, �����״̬��<0
int csvtojson(const char* path);

int main_xlstojson(int argc, char* argv[]) {
	int i = 0, rt;

	// �򵥲������
	if(argc < 2) {
		CERR("uage: %s [*.csv]", argv[0]);
		return 0;
	}
	
	// sccsv ʹ���� sclog ��Ҫ���� sclog
	sl_start();
	// ��ʼ��������
	while(++i<argc){
		rt = csvtojson(argv[i]);
		if(0 == rt) 
			printf("%s ת���ɹ�\n", argv[i]);
		else
			printf("%s ת��ʧ��, rt = %d\n", argv[i], rt);
	}
	
	return 0;
}

// �õ�����json�ļ�������, ��Ҫ�Լ�free
static char* _csvtojsonpath(const char* path) {
	char *tarp;
	int len = strlen(path);
	// �жϺ�׺��
	if(tstr_icmp(path+len-4, ".csv")){
		CERR("path is %s need *.csv", path);
		return NULL;
	}
	// ���������ڴ���д���
	if((tarp = malloc(len+2))==NULL) {
		CERR("malloc is error!");
		return NULL;
	}
	
	// �������ս��
	memcpy(tarp, path, len - 3);
	// ���json��׺��
	tarp[len-3] = 'j';
	tarp[len-2] = 's';
	tarp[len-1] = 'o';
	tarp[len] = 'n';
	tarp[len + 1] = '\0';
	return tarp;
}

// csv read -> json write
static void _csvtojson(sccsv_t csv, FILE* json) {
	// ��һ��, ��һ�ж��ǲ������
	int c, r;
	// �ڶ��� �����Ƕ�������������
	int clen = csv->clen - 1, rlen = csv->rlen - 1;
	// ��ȷ�������к���
	while (rlen > 2) {
		if (*sccsv_get(csv, rlen, 1))
			break;
		--rlen;
	}
	while (clen > 1) {
		if (*sccsv_get(csv, 0, clen))
			break;
		--clen;
	}

	// ������Ǹ�����
	fputs("[\n", json);
	for (r = 2; r <= rlen; ++r) {
		// ��������
		fputs("\t{\n", json);

		// �����ǰ����������
		for (c = 1; c <= clen; ++c) {
			fprintf(json, "\t\t\"%s\":%s", sccsv_get(csv, 1, c),sccsv_get(csv, r, c));
			fputs(c == clen ? "\n" : ",\n", json);
		}

		// ���µ�json�﷨֧�ֶ�� ','
		fputs(r == rlen ? "\t}\n" : "\t},\n", json);
	}

	fputs("]", json);
}

// ��csvת����json�ļ����, �ɹ�����0, �����״̬��<0
int 
csvtojson(const char* path) {
	char* tarp;
	FILE* json;
	sccsv_t csv;
	
	if(!path || !*path) {
		CERR("path is null!");
		return RT_ErrorParam;
	}
	
	// �����жϺ�׺��
	if((tarp = _csvtojsonpath(path)) == NULL ) {
		CERR("path = %s is error!", path);
		return RT_ErrorParam;
	}
	// ���￪ʼ���ļ�, ���ж�
	if((csv = sccsv_new(path)) == NULL) {
		free(tarp);
		CERR("sccsv_new %s is error!", path);
		return RT_ErrorFopen;
	}
	if((json = fopen(tarp, "wb")) == NULL ) {
		sccsv_delete(csv);
		free(tarp);
		CERR("fopen %s wb is error!", tarp);
		return RT_ErrorFopen;
	}
	
	// ������һ��ǰϷ�����˿�ʼת����
	_csvtojson(csv, json);
	
	fclose(json);
	sccsv_delete(csv);
	free(tarp);
	return RT_SuccessBase;
}