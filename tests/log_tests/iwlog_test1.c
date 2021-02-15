//
/**************************************************************************************************
 * IOWOW library
 *
 * MIT License
 *
 * Copyright (c) 2012-2021 Softmotions Ltd <info@softmotions.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *************************************************************************************************/


#include "../main.h"
 //#include <stdio.h>
 //#include <stdlib.h>
 //#include <unistd.h>

//int init_suite(void) {
//	int rc = iwlog_init();
//	return rc;
//}
//
//int clean_suite(void) {
//	return 0;
//}

typedef struct { FILE* fp; char name[_MAX_PATH]; } temp_file;
static temp_file my_temp_file(void)
{
	temp_file rezult_;

#undef name_template
#define name_template "fnXXXXXX"

	_snprintf_s(rezult_.name, _MAX_PATH, _MAX_PATH, "%s", name_template);

	/* Get the size of the string and add one for the null terminator.*/
	size_t sizeInChars = strlen(rezult_.name) + 1;
	/* Attempt to find a unique filename: */
	int err = _mktemp_s(rezult_.name, sizeInChars);
	if (err != 0) {
		perror("Problem creating the temp file name");
		exit(EXIT_FAILURE);
	}

	FILE* fp = NULL;
	if (fopen_s(&fp, rezult_.name, "w") == 0) {
		// printf("Unique filename is %s\n", rezult_.name);
	}
	else {
		printf("Cannot open %s\n", rezult_.name);
		fclose(fp);
		perror("exiting");
		exit(EXIT_FAILURE);
	}

	rezult_.fp = fp;
	return rezult_;
#undef name_template
}

UTEST(iwlog, test1) {
	uint32_t ec = (0xfffffffdU & 0x3fffffffU);
	uint64_t rc = 0xfafafafaULL;
	rc = iwrc_set_errno(rc, ec);
	uint32_t ec2 = iwrc_strip_errno(&rc);
	ASSERT_EQ(ec, ec2);
	ASSERT_EQ(rc, 0xfafafafaULL);
}

UTEST(iwlog, test2) {
	IWLOG_DEFAULT_OPTS opts = { 0 };
	int rv = 0;
	size_t sz;
	temp_file tfile = my_temp_file();
	FILE* out = tfile.fp;

	fprintf(stderr, "Redirecting log to: %s" IW_LINE_SEP, tfile.name);

	opts.out = out;
	iwlog_set_logfn_opts(&opts);

	iwlog_info2("7fa79c75beac413d83f35ffb6bf571b9");
	iwlog_error("7e94f7214af64513b30ab4df3f62714a%s", "C");
	iwlog_ecode_warn(IW_ERROR_READONLY, "c94645c3b107433497ef295b1c00dcff%d", 12);

	errno = ENOENT;
	iwrc ecode = iwrc_set_errno(IW_ERROR_ERRNO, errno);
	rv = iwlog(IWLOG_DEBUG, ecode, NULL, 0, "ERRNO Message");
	ASSERT_EQ(rv, 0);
	errno = 0;

	fclose(out);
	out = fopen(tfile.name, "r");
	ASSERT_TRUE(out);

	char buf[1024] = { 0 };
	memset(buf, 0, 1024);
	sz = fread(buf, 1, 1024, out);
	ASSERT_TRUE(sz);
	fprintf(stderr, "%s\n\n" IW_LINE_SEP, buf);

	ASSERT_TRUE(strstr(buf, "7fa79c75beac413d83f35ffb6bf571b9"));
	ASSERT_TRUE(strstr(buf, "7e94f7214af64513b30ab4df3f62714aC"));
	ASSERT_TRUE(strstr(buf,
		"DEBUG 70001|2|0|Error with expected errno "
		"status set. (IW_ERROR_ERRNO)|"));
	ASSERT_TRUE(strstr(buf, "ERRNO Message"));
	ASSERT_TRUE(strstr(buf, "ERROR iwlog_test1.c:"));
	ASSERT_TRUE(strstr(buf, "70004|0|0|Resource is readonly. (IW_ERROR_READONLY)|"));
	ASSERT_TRUE(strstr(buf, "c94645c3b107433497ef295b1c00dcff12"));

	fclose(out);
}

//int log_tests(int argc, const char* const* argv) {
//	CU_pSuite pSuite = NULL;
//
//	/* Initialize the CUnit test registry */
//	if (CUE_SUCCESS != CU_initialize_registry()) {
//		return CU_get_error();
//	}
//
//	/* Add a suite to the registry */
//	pSuite = CU_add_suite("iwlog_test1", init_suite, clean_suite);
//
//	if (NULL == pSuite) {
//		CU_cleanup_registry();
//		return CU_get_error();
//	}
//
//	/* Add the tests to the suite */
//	if ((NULL == CU_add_test(pSuite, "iwlog_test1", iwlog_test1))
//		|| (NULL == CU_add_test(pSuite, "iwlog_test2", iwlog_test2))) {
//		CU_cleanup_registry();
//		return CU_get_error();
//	}
//
//	/* Run all tests using the CUnit Basic interface */
//	CU_basic_set_mode(CU_BRM_VERBOSE);
//	CU_basic_run_tests();
//	int ret = CU_get_error() || CU_get_number_of_failures();
//	CU_cleanup_registry();
//	return ret;
//}
