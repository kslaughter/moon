/*
 * Automatically generated, do not edit this file directly
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>

#include "cbinding.h"

#include "plugin.h"
/**
 * PluginInstance
 **/
void *
plugin_instance_evaluate (PluginInstance *instance, const char *code)
{
	if (instance == NULL)
		return NULL;
	
	return instance->Evaluate (code);
}


gint32
plugin_instance_get_actual_height (PluginInstance *instance)
{
	if (instance == NULL)
		// Need to find a proper way to get the default value for the specified type and return that if instance is NULL.
		return (gint32) 0;
	
	return instance->GetActualHeight ();
}


gint32
plugin_instance_get_actual_width (PluginInstance *instance)
{
	if (instance == NULL)
		// Need to find a proper way to get the default value for the specified type and return that if instance is NULL.
		return (gint32) 0;
	
	return instance->GetActualWidth ();
}


bool
plugin_instance_get_allow_html_popup_window (PluginInstance *instance)
{
	if (instance == NULL)
		return false;
	
	return instance->GetAllowHtmlPopupWindow ();
}


void *
plugin_instance_get_browser_host (PluginInstance *instance)
{
	if (instance == NULL)
		return NULL;
	
	return instance->GetBrowserHost ();
}


bool
plugin_instance_get_enable_frame_rate_counter (PluginInstance *instance)
{
	if (instance == NULL)
		return false;
	
	return instance->GetEnableFrameRateCounter ();
}


bool
plugin_instance_get_enable_html_access (PluginInstance *instance)
{
	if (instance == NULL)
		return false;
	
	return instance->GetEnableHtmlAccess ();
}


bool
plugin_instance_get_enable_redraw_regions (PluginInstance *instance)
{
	if (instance == NULL)
		return false;
	
	return instance->GetEnableRedrawRegions ();
}


const char *
plugin_instance_get_init_params (PluginInstance *instance)
{
	if (instance == NULL)
		return NULL;
	
	return instance->GetInitParams ();
}


int
plugin_instance_get_max_frame_rate (PluginInstance *instance)
{
	if (instance == NULL)
		// Need to find a proper way to get the default value for the specified type and return that if instance is NULL.
		return (int) 0;
	
	return instance->GetMaxFrameRate ();
}


const char *
plugin_instance_get_source (PluginInstance *instance)
{
	if (instance == NULL)
		return NULL;
	
	return instance->GetSource ();
}


const char *
plugin_instance_get_source_location (PluginInstance *instance)
{
	if (instance == NULL)
		return NULL;
	
	return instance->GetSourceLocation ();
}


const char *
plugin_instance_get_source_location_original (PluginInstance *instance)
{
	if (instance == NULL)
		return NULL;
	
	return instance->GetSourceLocationOriginal ();
}


const char *
plugin_instance_get_source_original (PluginInstance *instance)
{
	if (instance == NULL)
		return NULL;
	
	return instance->GetSourceOriginal ();
}


Surface *
plugin_instance_get_surface (PluginInstance *instance)
{
	if (instance == NULL)
		return NULL;
	
	return instance->GetSurface ();
}


bool
plugin_instance_get_windowless (PluginInstance *instance)
{
	if (instance == NULL)
		return false;
	
	return instance->GetWindowless ();
}


void
plugin_instance_report_exception (PluginInstance *instance, char *msg, char *details, char* *stack_trace, int num_frames)
{
	if (instance == NULL)
		return;
	
	instance->ReportException (msg, details, stack_trace, num_frames);
}


void
plugin_instance_set_enable_frame_rate_counter (PluginInstance *instance, bool value)
{
	if (instance == NULL)
		return;
	
	instance->SetEnableFrameRateCounter (value);
}


void
plugin_instance_set_enable_redraw_regions (PluginInstance *instance, bool value)
{
	if (instance == NULL)
		return;
	
	instance->SetEnableRedrawRegions (value);
}


void
plugin_instance_set_max_frame_rate (PluginInstance *instance, int value)
{
	if (instance == NULL)
		return;
	
	instance->SetMaxFrameRate (value);
}


