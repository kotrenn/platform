#include "playerinterface.h"
#include "resourcemanager.h"

PlayerInterface::PlayerInterface(Level *level, InputDevice *device)
	:PlayerActor(level, device)
{
}

PlayerInterface::~PlayerInterface()
{
}

void PlayerInterface::draw_interface(Surface *dst) const
{
	PlayerActor::draw_interface(dst);

	return;

	// health bar
	int health_w = 20;
	int health_h = dst->get_h() * 0.8;
	int health_x = dst->get_w() - 2 * health_w;
	int health_y = 0.1 * dst->get_h();
	int buffer = 2;

	Uint32 black = mapRGB(0, 0, 0);
	Uint32 red = mapRGB(255, 0, 0);
	//Uint32 red = mapRGB(0, 0, 255);

	float p = (1.0 * m_health) / m_health_max;

	dst->draw_rect(black,
	               Rect(health_x - buffer,
	                    health_y - buffer,
	                    health_w + 2 * buffer,
	                    health_h + 2 * buffer),
	               buffer);
	dst->draw_rect_3d(red,
	                  Rect(health_x,
	                       (1 - p) * health_h + health_y,
	                       health_w,
	                       p * health_h));
}
