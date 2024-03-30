#include "olcUTIL_Geometry2D.h"
#define OLC_IGNORE_VEC2D
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace olc;
using namespace olc::utils;

#define PI 3.14159

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

	float map(float x, float in_min, float in_max, float out_min, float out_max) {
	  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

public:
	bool OnUserCreate() override
	{


		// Called once at the start, so create things here
		return true;
	}

	float aimingAngle=0.f;
	float sweepAngle=PI/6;

	float angle_difference(float angle_1, float angle_2)
	{
		angle_1 = fmod(angle_1, 2 * PI);
		angle_2 = fmod(angle_2, 2 * PI);
		float angle_diff = angle_1 - angle_2;

		while (angle_diff > PI)
			angle_diff -= 2 * PI;
		while (angle_diff < -PI)
			angle_diff += 2 * PI;

		return -angle_diff;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		Clear(VERY_DARK_BLUE);

		DrawCircle(GetScreenSize()/2,24);

		if(GetKey(SPACE).bPressed){
			aimingAngle=geom2d::line<float>(GetScreenSize()/2,GetMousePos()).vector().polar().y;
		}


		if(GetMouseWheel()>0)sweepAngle+=PI/64;
		if(GetMouseWheel()<0)sweepAngle-=PI/64;

		DrawPolygonDecal(nullptr,{GetScreenSize()/2,vf2d{1000,aimingAngle-sweepAngle}.cart()+GetScreenSize()/2,vf2d{1000,aimingAngle+sweepAngle}.cart()+GetScreenSize()/2},{{},{},{}},Pixel{40,40,0,128});
		DrawLineDecal(GetScreenSize()/2,vf2d{1000,aimingAngle}.cart()+GetScreenSize()/2,RED);
		DrawLineDecal(GetScreenSize()/2,vf2d{1000,aimingAngle-sweepAngle}.cart()+GetScreenSize()/2,YELLOW);
		DrawLineDecal(GetScreenSize()/2,vf2d{1000,aimingAngle+sweepAngle}.cart()+GetScreenSize()/2,YELLOW);



		float controllerX = map(GetMouseX(),0,ScreenWidth(),-1,1);
		float controllerY = map(GetMouseY(),1,ScreenHeight(),-1,1);

		DrawStringDecal({0,0},"Controller X: "+std::to_string(controllerX));
		DrawStringDecal({0,12},"Controller Y: "+std::to_string(controllerY));

		vf2d turretVector{controllerX,controllerY};
		if(turretVector.mag()>1.f)turretVector=turretVector.norm();
		float turretAngle=atan2(turretVector.y,turretVector.x);

		DrawStringDecal({0,32},"Aiming Angle: "+std::to_string(aimingAngle));
		DrawStringDecal({0,44},"Turret Angle: "+std::to_string(turretAngle));

		float angleRatio=sweepAngle/(PI/2);

		float angle_diff=angle_difference(aimingAngle,turretAngle)*angleRatio;

		DrawStringDecal({0,56},"Angle Diff: "+std::to_string(angle_diff),abs(angle_diff)>sweepAngle?RED:WHITE);

		angle_diff=std::clamp(angle_diff,-sweepAngle,sweepAngle);
		DrawStringDecal({0,68},"Adjusted Angle Diff: "+std::to_string(angle_diff),GREEN);

		turretAngle=aimingAngle+angle_diff;

		DrawLineDecal(GetScreenSize()/2,vf2d{1000,turretAngle}.cart()+GetScreenSize()/2,GREEN);

		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}