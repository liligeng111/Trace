//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <FL/fl_file_chooser.H>		// FLTK file chooser

#include "TraceGLWindow.h"


struct TraceUISlider
{
	TraceUISlider() {};
	TraceUISlider(const char* name, float minimum, float maximum, float stepsize, float value) {
		strcpy(m_name, name);
		m_minimum = minimum; m_maximum = maximum; m_stepsize = stepsize; m_value = value;
	}
	TraceUISlider& operator=(const TraceUISlider& o) {
		if (this != &o) {
			strcpy(m_name, o.m_name);
			m_minimum = o.m_minimum; m_maximum = o.m_maximum; m_stepsize = o.m_stepsize; m_value = o.m_value;
		}
		return *this;
	}
	char  m_name[128];
	float m_minimum;
	float m_maximum;
	float m_stepsize;
	float m_value;
};

struct TraceUIButton
{
	TraceUIButton() {};
	TraceUIButton(const char* name, bool value) {
		strcpy(m_name, name); m_value = value;
	}
	TraceUIButton& operator=(const TraceUIButton& o) {
		if (this != &o) {
			strcpy(m_name, o.m_name); m_value = o.m_value;
		}
		return *this;
	}
	char m_name[128];
	bool m_value;
};

typedef enum {
	ANTIALIAS_B,
	JITTER_B,
	HBV_B,
	NUM_BUTTON
} BUTTON_ENUM;

typedef enum {
	ATTENU_CONSTANT,
	ATTENU_LINEAR,
	ATTENU_QUAD,
	AMBIENT_LIGHT,
	INTENSITY_SCALE,
	DISTANCE_SCALE,
	NUM_SLIDER
} SLIDER_ENUM;

static TraceUIButton ButtonList[NUM_BUTTON] = 
{
	TraceUIButton("Antialias Enabled", false),
	TraceUIButton("Jitter Enabled", false),
	TraceUIButton("HBV Acceleration Enabled", true)
};

static TraceUISlider SliderList[NUM_SLIDER] = {
	// name, min, max, step, value
	TraceUISlider("Attenuation, Constant", 0, 1, 0.01, 0.25),
	TraceUISlider("Attenuation, Linear", 0, 1, 0.01, 0.25),
	TraceUISlider("Attenuation, Quadratic", 0, 1, 0.01, 0.50),
	TraceUISlider("Ambient Light, Quadratic", 0, 1, 0.01, 0.20),
	TraceUISlider("Intensity Scale", 1, 10, 1, 1),
	TraceUISlider("Distance Scale (Log10)", -0.99, 3, 0.01, 1.87)
};

class TraceUI {
public:
	TraceUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	Fl_Slider*			m_sizeSlider;
	Fl_Slider*			m_depthSlider;

	Fl_Button*			m_renderButton;
	Fl_Button*			m_stopButton;

	TraceGLWindow*		m_traceGlWindow;

	Fl_Slider* m_sliderList[NUM_SLIDER];
	Fl_Check_Button* m_checkButtonList[NUM_BUTTON];

	float get_slidervalue(int n) {
		return m_sliderList[n]->value();
	}

	bool get_buttonvalue(int n) {
		return m_checkButtonList->value();
	}


	// member functions
	void show();

	void		setRayTracer(RayTracer *tracer);

	int			getSize();

private:
	RayTracer*	raytracer;

	int			m_nSize;

// static class members
	static Fl_Menu_Item menuitems[];

	static TraceUI* whoami(Fl_Menu_* o);

	static void cb_load_scene(Fl_Menu_* o, void* v);
	static void cb_save_image(Fl_Menu_* o, void* v);
	static void cb_exit(Fl_Menu_* o, void* v);
	static void cb_about(Fl_Menu_* o, void* v);

	static void cb_exit2(Fl_Widget* o, void* v);

	static void cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_depthSlides(Fl_Widget* o, void* v);

	static void cb_render(Fl_Widget* o, void* v);
	static void cb_stop(Fl_Widget* o, void* v);
};


struct para
{
	int N;
	int height;
	int width;
	RayTracer* tracer;
	int id;
};

#endif
