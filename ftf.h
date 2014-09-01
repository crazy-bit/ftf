/**
 * @file	ftf.h
 * @brief	函数测试框架，可以用于程序API的测试组件，可以用于程序的调试
 * 	工具。其支持的功能如下：
 * 	 1 - 提供类似于shell的输入提示和命令的自动匹配
 * 	 2 - 提供命令的记忆(上下键)
 * 	 3 - 提供命令的自动匹配，命令可以由多个侵害分割字符串组成，如"address to ip IP"
 *
 * 	 函数测试框架中命令由命令和参数两部分组成，命令部分由框架完成完全匹配，
 * 	 参数部分以argc=参数个数，argv=第一个参数地址的形式式传入给调用的函数处理
 *
 * -----------------------------------------------------------------
 * 使用示例：[具体示例参见api.c]
 *  向函数测试框架中添加一个测试函数test Str，输出输入的第一个字符串
 *	DEFUN(test_cmd_t,
 *		test_cmd_n,
 *		"test STR",
 *		"Usage: test STR\n"
 *		"	Parameters: STR - string to print\n")
 *	{
 *		printf("Str=%s\n", argv[0]);
 *		return 0;
 *	}
 * 
 *  int main()
 *  {
 * 		install_element(&test_cmd_n);
 * 		ftf_run();
 * 		return 0;
 * 	}
 * -----------------------------------------------------------------
 *
 * @author	yoyo
 * @date	2012-03-26
 */

#ifndef _FTF_H
#define _FTF_H

#ifdef __cplusplus
extern "C" {
#endif
	
// maximum number of command to remember
#define HISTORY_MAX_SIZE 20
// size of input buffer size
#define FTF_BUFFER_SIZE	1024
	
/**
 * A virtual tty used by FFT
 *
 * @param prompt prompt string, such as ftf, then you see 'ftf>'
 * @param buffer buffer to store user input
 * @param buf_len buffer length
 * @param used_len buffer actually used length
 * @param cur_pos current cursor point in which position in buffer
 * @param c latest input key word
 * @param history to record user input command
 * @param hpos history current position
 * @param hindex history end index
 * */
struct ftf_vty {
	char prompt[24];
	char buffer[FTF_BUFFER_SIZE];
	int buf_len;
	int used_len;
	int cur_pos;
	char c;
	
	char *history[HISTORY_MAX_SIZE];
	int hpos;
	int hindex;
};
	
/**
 * struct to store command string
 *
 * @param size total size of vector
 * @param used_size already allocated size
 * @param data point to data 
 */
typedef struct ftf_vector {
	int size;
	int used_size;
	void **data;
} ftf_vector_t;

/**
 * A struct cmd_elem relative to One command
 *
 * @param string command string, such as 'show vlan'
 * @param doc command refenrence, each command word has one
 * @param func excute when command called
 * @param para_vec command string in para_vec form
 * @param para_size command parameter number
 */
struct cmd_elem {
	char *string;
	char *doc;
	int (*func)(struct cmd_elem *, struct ftf_vty *, int , char **);
	ftf_vector_t *para_vec;
	int para_num;
};

/**
 *  Test Function API Definition
 *
 *  @func_name 测试函数的名字，需要唯一，一般以_t结尾
 *  @cmd_name 测试函数的结构体，需要唯一，即加入install_element中的参数，一般以_n结尾
 *  @cmd_str 命令，只包含命令部分，参数部分不用
 *  @help_str 对命令的解释，在\t键时会对命令进行提示
 */
#define DEFUN(func_name, cmd_name, cmd_str, help_str)	\
	int func_name(struct cmd_elem *, struct ftf_vty *, int, char **);	\
	struct cmd_elem cmd_name = 	\
	{	\
		cmd_str,	\
		help_str,	\
		func_name 	\
	};	\
	int func_name(struct cmd_elem *self, struct ftf_vty *vty, int argc, char **argv)


/**
 * 向函数测试框架中添加测试函数，函数的定义如DEFUN宏定义所示
 *
 * @param cmd 新加入的要测试的函数
 */
void install_element(struct cmd_elem *cmd);

/**
 * 函数测试框架运行函数
 * */
int run();

#ifdef __cplusplus
}
#endif

#endif

