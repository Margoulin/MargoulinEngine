#include <psp2/kernel/processmgr.h>

#include <psp2/ctrl.h>
#include <MUtils/String.hpp>
#include <MUtils/Maths/Vector.hpp>
#include <MargoulinEngine/Engine.hpp>

int main(int argc, char *argv[]) 
{
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData pad;
	memset(&pad, 0, sizeof(pad));

	Engine* engine = nullptr;
	engine = Engine::GetInstance();
	engine->Initialize(nullptr);
	
	while (true)
	{
		sceCtrlPeekBufferPositive(0, &pad, 1);
		if (pad.buttons & SCE_CTRL_CROSS)
			break;
		if (engine->Update())
			engine->Draw();
	}
	engine->Shutdown();

	sceKernelExitProcess(0);
    return 0;
}
