/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <View.h>
#include "ShojiView.h"

#define SHOJI_TEMPLATE_VIEW_TYPE "application/x.vnd-sen-labs.Shoji.templateView"
#define SHOJI_TEMPLATE_PATH "shoji/templates"
#define SHOJI_TEMPLATE_NAME "template.view"

class ShojiTemplateView : public ShojiView
{
    public:
                    ShojiTemplateView(const char* mimeType);
        virtual	   ~ShojiTemplateView();
        const char* GetType();
        status_t    Initialize();
        status_t    Populate(const BMessage *mimeAttrInfo, const BMessage* attrs);

    protected:
        status_t    LookupView(const char* mimeType, BView* view);
        void        PopulateControl(BView *view, const void** data);
};
