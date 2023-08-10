#ifdef WINDOWS

#include "selftest_local.h"

const char *demo_loop_1 =
"setChannel 10 0\r\n"
"again:\r\n"
"    addChannel 10 1\r\n"
"    if $CH10<3 then goto again\r\n"
"    setChannel 11 234\r\n";


const char *demo_loop_2 =
"setChannel 20 3\r\n"
"setChannel 22 0\r\n"
"again:\r\n"
"    addChannel 20 -1\r\n"
"    addChannel 22 1\r\n"
"    if $CH20>0 then goto again\r\n"
"    setChannel 21 123\r\n";

const char *demo_loop_3 =
"setChannel 20 15\r\n"
"setChannel 0 1\r\n"
"again:\r\n"
"    addChannel 20 -1\r\n"
"    delay_s 1\r\n"
"    if $CH20>0 then goto again\r\n"
"    setChannel 0 0\r\n";

void Test_Scripting_Loop1() {
	char buffer[64];

	// reset whole device
	SIM_ClearOBK(0);
	CMD_ExecuteCommand("lfs_format", 0);

	// put file in LittleFS
	Test_FakeHTTPClientPacket_POST("api/lfs/demo_loop_1.txt", demo_loop_1);
	// get this file 
	Test_FakeHTTPClientPacket_GET("api/lfs/demo_loop_1.txt");
	SELFTEST_ASSERT_HTML_REPLY(demo_loop_1);

	CMD_ExecuteCommand("startScript demo_loop_1.txt", 0);
	SELFTEST_ASSERT_INTEGER(CMD_GetCountActiveScriptThreads(), 1);
	// should execute and quit within few frames
	// (we have a limit of scripting executed per frame)
	Sim_RunFrames(15, false);
	SELFTEST_ASSERT_INTEGER(CMD_GetCountActiveScriptThreads(), 0);
	SELFTEST_ASSERT_CHANNEL(10, 3);
	SELFTEST_ASSERT_CHANNEL(11, 234);
	//system("pause");
}
void Test_Scripting_Loop2() {
	char buffer[64];

	// reset whole device
	SIM_ClearOBK(0);
	CMD_ExecuteCommand("lfs_format", 0);

	// put file in LittleFS
	Test_FakeHTTPClientPacket_POST("api/lfs/demo_loop_2.txt", demo_loop_2);
	// get this file 
	Test_FakeHTTPClientPacket_GET("api/lfs/demo_loop_2.txt");
	SELFTEST_ASSERT_HTML_REPLY(demo_loop_2);

	CMD_ExecuteCommand("startScript demo_loop_2.txt", 0);
	SELFTEST_ASSERT_INTEGER(CMD_GetCountActiveScriptThreads(), 1);
	// should execute and quit within few frames
	// (we have a limit of scripting executed per frame)
	Sim_RunFrames(25, false);
	SELFTEST_ASSERT_INTEGER(CMD_GetCountActiveScriptThreads(), 0);
	SELFTEST_ASSERT_CHANNEL(20, 0);
	SELFTEST_ASSERT_CHANNEL(21, 123);
	SELFTEST_ASSERT_CHANNEL(22, 3);
	//system("pause");
}
void Test_Scripting_Loop3() {
	char buffer[64];

	// reset whole device
	SIM_ClearOBK(0);
	CMD_ExecuteCommand("lfs_format", 0);

	// put file in LittleFS
	Test_FakeHTTPClientPacket_POST("api/lfs/demo_loop_3.txt", demo_loop_3);
	// get this file 
	Test_FakeHTTPClientPacket_GET("api/lfs/demo_loop_3.txt");
	SELFTEST_ASSERT_HTML_REPLY(demo_loop_3);

	CMD_ExecuteCommand("startScript demo_loop_3.txt", 0);
	SELFTEST_ASSERT_INTEGER(CMD_GetCountActiveScriptThreads(), 1);
	// should execute and quit within few frames
	// (we have a limit of scripting executed per frame)
	for (int i = 0; i < 14; i++) {
		Sim_RunSeconds(1, false);
		SELFTEST_ASSERT_INTEGER(CMD_GetCountActiveScriptThreads(), 1);
		SELFTEST_ASSERT_CHANNEL(0, 1);
	}
	for (int i = 0; i < 2; i++) {
		Sim_RunSeconds(1, false);
	}
	for (int i = 0; i < 3; i++) {
		Sim_RunSeconds(1, false);
		SELFTEST_ASSERT_INTEGER(CMD_GetCountActiveScriptThreads(), 0);
	}
	SELFTEST_ASSERT_CHANNEL(0, 0);
	SELFTEST_ASSERT_CHANNEL(20, 0);
	//system("pause");
}
void Test_Scripting() {
	Test_Scripting_Loop1();
	Test_Scripting_Loop2();
	Test_Scripting_Loop3();
}

#endif
