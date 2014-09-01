<p>
	<strong>HomePage:&nbsp;http://crazy-bit.github.io/ftf/</strong>
</p>
<p>
	函数测试框架，可以用于程序API的测试组件，可以用于程序的调试工具。其支持的功能如下：
</p>
&nbsp; &nbsp;1 - 提供类似于shell的输入提示和命令的自动匹配<br />
&nbsp; &nbsp;2 - 提供命令的记忆(上下键<br />
&nbsp; &nbsp;3 - 提供命令的自动匹配，命令可以由多个侵害分割字符串组成，如&quot;address to ip IP&quot;<br />
<br />
<br />
函数测试框架中命令由命令和参数两部分组成，命令部分由框架完成完全匹配，参数部分以argc=参数个数，argv=第一个参数地址的形式式传入给调用的函数处理<br />
<br />
<br />
-----------------------------------------------------------------<br />
使用示例：[具体示例参见api.c]<br />
向函数测试框架中添加一个测试函数test Str，输出输入的第一个字符串<br />
DEFUN(test_cmd_t,<br />
&nbsp; &nbsp; test_cmd_n,<br />
&nbsp; &nbsp; &quot;test STR&quot;,<br />
&nbsp; &nbsp; &quot;Usage: test STR\n&quot;<br />
&nbsp; &nbsp; &quot;<span style="white-space:pre"> </span>Parameters: STR - string to print\n&quot;)<br />
{<br />
&nbsp; &nbsp; printf(&quot;Str=%s\n&quot;, argv[0]);<br />
&nbsp; &nbsp; return 0;<br />
}<br />
<br />
<br />
int main()<br />
{<br />
&nbsp; &nbsp; install_element(&amp;test_cmd_n);<br />
&nbsp; &nbsp; ftf_run();<br />
&nbsp; &nbsp; return 0;<br />
}<br />
-----------------------------------------------------------------
