/*
*	created by liyun 2017/10/20
*   function 描述下这个类用来干什么
*   version 1.0
*/

#ifndef TRACELIGHT_H  // 全大写
#define TRACELIGHT_H

namespace calculation { // 命令空间 避免命名重复

	class TraceLight //类名保持和头文件名字相同
	{
	public:
		TraceLight() {
			// 所有变量初始化 指针NULL
		};

		// 若有继承 一定要加virtual
		~TraceLight() {
			// 若有分配了内存记得释放
			// if(p) {
			//   delete p;
			//   p = NULL;
			// }
		};   

		// 若有分配内存 最好重写赋值构造和重载 = 
		TraceLight(const TraceLight& traceLight) {
			
		}
		TraceLight operator = (const TraceLight& traceLight) {

		}

		/*
		* 简单函数功能介绍 + 参数说明 以及返回参数说明
		*/
		void fun() {};

	private:

		int* p;
	};

}

#endif // TRACELIGHT_H