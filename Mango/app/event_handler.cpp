#include "event_handler.h"

#include "mango_app.h"


void EventHandler::OnKeyPress(int key, std::string key_name, bool repeat)
{
	
}
void EventHandler::OnKeyRelease(int key, std::string key_name)
{

}

void EventHandler::OnMouseMove(float xpos, float ypos)
{

}
void EventHandler::OnMouseButtonPress(int button, bool repeat)
{

}
void EventHandler::OnMouseButtonRelease(int button)
{

}

void EventHandler::OnWindowResize(int width, int height)
{
	DBG_LOG("Resize [%i, %i]", width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);

	m_mango_app->GetFramebuffer()->Release();
	m_mango_app->GetFramebuffer()->Setup({ width, height });

	m_mango_app->GetMangoCore()->GetRenderer2D().Resize(width, height);
	m_mango_app->GetMangoCore()->GetRenderer3D().Resize(width, height);
}