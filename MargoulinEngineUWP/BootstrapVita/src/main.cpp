#include <psp2/kernel/processmgr.h>

#include <MUtils/String.hpp>
#include <MUtils/Maths/Vector.hpp>
#include <MargoulinEngine/Engine.hpp>

int main(int argc, char *argv[]) 
{
	Engine* engine = nullptr;
	engine = Engine::GetInstance();
	engine->Initialize(nullptr);
	
	while (true)
	{
		if (engine->Update())
			engine->Draw();
	}
	engine->Shutdown();

	sceKernelExitProcess(0);
    return 0;
}
