// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CUDADLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// CUDADLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

//下面创建一个CUDA Kernel 并把它做成DLL
#include<vector>
using namespace std;

#ifdef CUDADLL_EXPORTS
#define CUDADLL_API __declspec(dllexport)
#else
#define CUDADLL_API __declspec(dllimport)
#endif

//extern "C" CUDADLL_API int vectorAdd(int c[], int a[], int b[], int size);
extern "C"  void DeviceInf(void);


//这个函数利用CUDA计算 输入光线组 与 STL面元组的交点，然后返回：1，是否相交，2，距交点传播距离，3，与第几个STL相交，4，交点位置x,y,z
extern "C" void RunReflectionLine(int _NumPoints, float* _psourcex, float* _psourcey, float*  _psourcez,
	float* _pdirx, float*  _pdiry, float*  _pdirz,
	bool* &_intersected, float* &_prot, int* &_STLIndex,
	float* &_inter_x, float* &_inter_y, float* &_inter_z,
	int _NumSTL, float* _stlp1x, float* _stlp1y, float* _stlp1z,
	float* _stlp2x, float* _stlp2y, float* _stlp2z,
	float* _stlp3x, float* _stlp3y, float* _stlp3z);

/*
// 此类是从 CUDADLL.dll 导出的
class CUDADLL_API CCUDADLL {
public:
	CCUDADLL(void);
	// TODO:  在此添加您的方法。
};

extern CUDADLL_API int nCUDADLL;

CUDADLL_API int fnCUDADLL(void);
*/