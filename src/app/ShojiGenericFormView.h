/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <GroupView.h>

#include "ShojiView.h"

class ShojiGenericFormView : public ShojiView
{
    public:
                    ShojiGenericFormView();
        virtual	   ~ShojiGenericFormView();
        status_t    Populate(const BMessage *mimeAttrInfo, const BMessage* attrs);

    protected:
        BView*      CreateDataView(const char* name, type_code typeCode, bool editable, const void* data);
};
