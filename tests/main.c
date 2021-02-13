#include "main.h"

UTEST_STATE;   // <-- note there is no ()! 

int main(int argc, const char* const argv[])
{
	int rc = iwlog_init();
	return utest_main(argc, argv);
}