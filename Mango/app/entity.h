#pragma once

#include "../mango/mango.h"




class MangoApp;

class Entity : public Mango::Entity3D
{
public:
	Entity() = default;
	virtual ~Entity() {}

	virtual void OnInit() {}
	virtual void OnRelease() {}
	virtual void OnRender(Mango::MangoCore* mango_core, float lerp) {}
	virtual void OnFrameUpdate(Mango::MangoCore* mango_core, float lerp) {} // this is before OnRender()
	virtual void OnUpdate() {}

	void SetMangoApp(MangoApp* app) { m_app = app; }
	MangoApp* GetMangoApp() { return m_app; }

private:
	MangoApp* m_app = reinterpret_cast<MangoApp*>(0xDEADBEEF);
};