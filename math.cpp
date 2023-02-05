#include "math.h"
#include <queue>
#include <iostream>

//Calculate points between 0 and 1 bases on Coefieant input.
float Math500::math::Coeffieants(int c)
{
	float steps = 1.0f/(static_cast<float>(c));
	return steps;
}

//plots Points using a given Coeffieant step.
void Math500::math::plotpoints(ImPlotDragToolFlags flags, float step)
{
    plots.clear();

    for (float i = 0.0; i < 1.0; i += step) {

        if (i == 0.0) {
            plots.push_back(ImPlotPoint(0, 1)); 
        }
        else {
            plots.push_back(ImPlotPoint(i, 1));
        }
    }

    plots.push_back(ImPlotPoint(1, 1));
   

}

//Calculate BB for a strightLine
void Math500::math::P1_BB(int c, ImPlotPoint B[], ImVector<ImPlotPoint> plots)
{
    for (int i = 0; i < 100; ++i) 
    {
        float t = static_cast<float>(i) /99.0f;
        float bpoint = 0;
        float xpoint = 0;
        if (i == 99)
        {
            int l = 0;
        }
        for (int j = 0; j <= c ; ++j)
        {
            float choose = fact(c) / (fact(j) * fact(c - j));
            bpoint += choose * pow((1.0f - t), c - j) * pow(t, j) * plots[j].y;
            xpoint += choose * pow((1.0f - t), c - j) * pow(t, j) * plots[j].x;
        }

		B[i] = ImPlotPoint(xpoint,bpoint);
	}
}

void Math500::math::P1_DeCasteljau(int c, ImPlotPoint B[], ImVector<ImPlotPoint> plots)
{
    for (int i = 0; i < 100; i++)
    {
        ImVector<ImPlotPoint> coff = plots;

        float t = static_cast<float>(i) / 99.0f;
        for (int j = 0; j < c; j++)
        {
            for (int k = 0; k < c - j - 1; k++)
            {
                coff[k].y = coff[k].y * (1.0f - t) + coff[k + 1].y * t;
                coff[k].x = coff[k].x * (1.0f - t) + coff[k + 1].x * t;
            }
        }
        B[i] = ImPlotPoint(coff[0].x, coff[0].y);
    }

}

void Math500::math::P2_NLI_Form(float t, ImVector<ImPlotPoint> b,bool shells)
{

    plots = b;
    int d = b.size();
    std::string name = "##shell";
    if(shells)
    {
	    for (int j = 0; j < d; j++)
	    {
	    	for (int k = 0; k < d - j - 1; k++)
	    	{
	    		plots[k].x = plots[k].x * (1.0f - t) + plots[k + 1].x * t;
	    		plots[k].y = plots[k].y * (1.0f - t) + plots[k + 1].y * t;
	    	}

	    	ImPlot::SetNextLineStyle(ImVec4(0, 0, 8, 4), 2);
	    	ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
	    	name += std::to_string(j);
	    	ImPlot::PlotLine(name.c_str(), &plots[0].x, &plots[0].y, plots.size()-j, 0, 0, sizeof(ImPlotPoint));
	    }
    }

    ImPlotPoint nli[100];
    P1_DeCasteljau(d, nli, b);
    ImPlot::SetNextLineStyle(ImVec4(1, 0.9f, 2, 1), 2);
    ImPlot::PlotLine("##NLI", &nli[0].x, &nli[0].y, 100, 0, 0, sizeof(ImPlotPoint));
}

void Math500::math::P2_BB_Form(float t, ImVector<ImPlotPoint> b)
{
    int d = b.size() - 1;

    ImPlotPoint bb[100];
    P1_BB(d, bb, b);
    ImPlot::SetNextLineStyle(ImVec4(1, 0.9f, 2, 1), 2);
    ImPlot::PlotLine("##BB", &bb[0].x, &bb[0].y, 100, 0, 0, sizeof(ImPlotPoint));
}

void Math500::math::P2_MidPoint_Form(ImVector<ImPlotPoint> b)
{
    plots = b;
    std::queue<ImVector<ImPlotPoint>> theQue;
    theQue.push(b);

    float t = 0.5;
    constexpr int loop = 5;
    int k = 0;
	int d = b.size();

    while (k != loop)
    {
        auto size = theQue.size();
        for(int i = 0; i < size;i++)
	    {
		    auto points = theQue.front();
	    	theQue.pop();

	    	ImVector<ImPlotPoint> first;

	    	for (int j = 0; j < d; j++)
	    	{
	    		first.push_back(points.front());
	    		for (int k = 0; k < d - j - 1; k++)
	    		{
	    			points[k].x = points[k].x * (1.0f - t) + points[k + 1].x * t;
	    			points[k].y = points[k].y * (1.0f - t) + points[k + 1].y * t;
	    		}
	    	}

	    	theQue.push(first);
	    	theQue.push(points);
	    }

        k++;
    }

    ImVector<ImPlotPoint> finalPlot;
    while(!theQue.empty())
    {
        auto p = theQue.front();
        for (auto& point : p)
        {
            finalPlot.push_back(point);
        }
        theQue.pop();
    }

    ImPlot::SetNextLineStyle(ImVec4(0, 0.9f, 2, 1), 2);
    ImPlot::PlotLine("##MidP", &finalPlot[0].x, &finalPlot[0].y, finalPlot.size(), 0, 0, sizeof(ImPlotPoint));

}

float Math500::math::fact(int n)
{
    if (n == 0)
        return 1;
    float res = 1;
    for (int i = 2; i <= n; i++)
        res = res * i;
    return res;
}

ImVector<ImPlotPoint>& Math500::math::getPoints()
{
    return plots;
}

