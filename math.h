#pragma once

#include <ImFrame.h>

namespace Math500
{
	class math 
	{
	public:
		float Coeffieants(int c);

		void  plotpoints(ImPlotDragToolFlags flags, float step);

		void  P1_BB(int c, ImPlotPoint b[], ImVector<ImPlotPoint> plots);

		void  P1_DeCasteljau(int c, ImPlotPoint b[], ImVector<ImPlotPoint> plots);

		void P2_NLI_Form(float t, ImVector<ImPlotPoint> b, bool shells);

		void P2_BB_Form(float t, ImVector<ImPlotPoint> b);

		void P2_MidPoint_Form(ImVector<ImPlotPoint> b);

		void P3_Newton_Polynomial(ImVector<ImPlotPoint> b);

		float fact(int n);

		ImVector<ImPlotPoint>& getPoints();

	private:
		ImVector<ImPlotPoint> plots;

	};

}


