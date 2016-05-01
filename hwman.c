#include <stdio.h>
#include <string.h>

int main(void)
{
	const int lid_open = file_contains_string("/proc/acpi/button/lid/LID/state", "open");
	const int ac_power = !file_contains_string("/sys/class/power_supply/BAT0/status", "Discharging");


	if(getuid())
	{
		printf("Root permission is required to run this program.\n");
		return 1;
	}
	
	if(!lid_open)
	{
		if(!application_running("xtrlock"))
			system("DISPLAY=\":0\" xtrlock -b &");
		if(!ac_power)
			system("s2ram");
	}
	
	return 0;
}

int file_contains_string(const char* file_path, const char* string)
{
	FILE *ptr_file = fopen(file_path, "r");
	if(!ptr_file)
		return 0;
	
	char buffer[1000];
	
	while(fgets(buffer, 1000, ptr_file) != NULL)
	{
		char *line = buffer;
		if(strstr(buffer, string))
		{
			fclose(ptr_file);
			return 1;
		}
	}
	fclose(ptr_file);
	
	return 0;
}

int application_running(const char* cmd)
{
	system("ps -a|grep %cmd > /tmp/pid_data");
	return file_contains_string("/tmp/pid_data", cmd);
}
