/*
The MIT License (MIT)

Copyright (c) 2021 James Boer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#include <ImFrame.h>
#include "math.h"

#ifdef IMFRAME_WINDOWS
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace Math500
{
	class MainApp : public ImFrame::ImApp
	{
	public:
		MainApp(GLFWwindow * window) : ImFrame::ImApp(window) {}
		virtual ~MainApp() {}

        
		void OnUpdate() override {
            ImGui::BulletText("Welcome to my Plotting UI");
            ImGui::SetWindowSize(ImVec2(500,500), ImGuiCond_None);
            static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;

            //Setting up the drop down
            static int p_menu = 1;
            static float slider_i = 0.5;
            static bool shells = true;


            ImGui::Combo("Type", &p_menu, "Nested Linear Interpolation\0Bernstein Polynomials\0Midpoint Subdivision\0\0");
            ImGui::SliderFloat("t value", &slider_i, 0, 1, "%f", flags);
            
            //Drawing of Graph/ Plots
            static ImVector<ImPlotPoint> b;
            
            ImGui::Checkbox("Shells",&shells);

            if (ImGui::Button("Clear"))
                b.clear();

            if (ImPlot::BeginPlot("Project1", ImVec2(-1,-1), 0)) {
                ImPlot::SetupAxes("t-values", "y-values");
                ImPlot::SetupAxesLimits(0, 1, -3, 3);
                ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 1.0, 1.0);
                ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, 6.0, 6.0);

                

                if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl) {
                    ImPlotPoint pt = ImPlot::GetPlotMousePos();
                    b.push_back(pt);
                }
                
                
                if (!b.empty())
                {
                    int Id = 0;
                    for (auto it = b.begin(); it != b.end(); it++)
                    {

                        ImPlot::DragPoint(Id, &it->x, &it->y, ImVec4(1, 0.5f, 1, 1), 4);
                        Id++;
                        if (it->y > 3.0)
                        {
                            it->y = 3.0;
                        }
                        if (it->y < -3.0)
                        {
                            it->y = -3.0;
                        }
                    }
                }
                if (b.size() >= 2)
                {
                	if (p_menu == 0) {

                        calculate.P2_NLI_Form(slider_i, b, shells);
                	}
                	if (p_menu == 1) {
                        calculate.P2_BB_Form(slider_i, b);
                	}
                	if (p_menu == 2) {
                        calculate.P2_MidPoint_Form(b);
                        slider_i = 0.5;
                	}

                
                	ImPlot::SetNextLineStyle(ImVec4(0, 0.9f, 0, 1), 2);
                	ImPlot::PlotLine("##bez", &b[0].x, &b[0].y, b.size(), 0, 0, sizeof(ImPlotPoint));
            
                	
	            }

				ImPlot::EndPlot();
            }

		}
    private:
        int status = -1;
        Math500::math calculate;

	};
}

// ImFrame main function and app creation
IMFRAME_MAIN("Math500cf", "Project One", Math500::MainApp)

