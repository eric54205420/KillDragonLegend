#include <schead.h>
#include <scconf.h>

// д����,��������,һ����
int main_scconf(int argc, char* argv[])
{
	const char* value;
	mconf_start();

	//�򵥲��� ���ö�ȡ����
	value = mconf_get("heoo");
	puts(value);

	value = mconf_get("heoo2");
	if (value)
		puts(value);
	else
		puts("heoo2������");

	value = mconf_get("yexu");
	puts(value);

	value = mconf_get("end");
	puts(value);

	system("pause");
	return 0;
}