/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Alert.h>
#include <CheckBox.h>
#include <ControlLook.h>
#include <Entry.h>
#include <Errors.h>
#include "ShioDynamicView.h"
#include <StringView.h>
#include <TextControl.h>

/*
 * generic dynamic view to use when no MIME specific view is available for the source file.
 * Builds a view containing fields and controls according to the attribute type of the source file attributes.
 */
ShioDynamicView::ShioDynamicView(const BMessage *props)
: BGroupView(B_VERTICAL, 0)
{
	SetFlags(Flags() | B_NAVIGABLE);
	SetName("Shio Dynamic View");
    GroupLayout()->SetInsets(be_control_look->DefaultLabelSpacing());

	// Set view color to standard background grey
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	SetFont(be_plain_font);

    Populate(props);
}

ShioDynamicView::~ShioDynamicView()
{
}

status_t ShioDynamicView::Populate(const BMessage *props)
{
    const void* data;
    char*       name;
    ssize_t     size;
    type_code   type;
    status_t    result;

    int32 count = props->CountNames(B_ANY_TYPE);
    int32 countFieldItems;

    for (int32 i = 0; i < count; i++) {
        status_t result = props->GetInfo(B_ANY_TYPE, i, &name, &type, &countFieldItems);
		if (result != B_OK)
			continue;

        if (countFieldItems != 1) {
            BString error("Could not process message data ");
            error.Append(name).Append("\nMultiple items per field not supported.");

            BAlert alert("Error setting up view", error.String(), "OK");
            alert.SetFlags(alert.Flags() | B_STOP_ALERT | B_CLOSE_ON_ESCAPE);
            alert.Go();
            return B_ERROR;
        }

        result = props->FindData(name, type, &data, &size);
        if (result != B_OK) {
            BString error("Could not read data ");
            error.Append(name);
            BAlert alert("Error setting up view", error.String(), "OK");
            alert.SetFlags(alert.Flags() | B_STOP_ALERT | B_CLOSE_ON_ESCAPE);
            alert.Go();
            return B_ERROR;
        }

        BView* propView = CreateDataView(name, type, data);
        GroupLayout()->AddView(propView);
    }

    return B_OK;
}

BView* ShioDynamicView::CreateDataView(const char* name, type_code typeCode, const void* data)
{
    // container view
    BString containerLabel(name);
    containerLabel.Append("_container");

    BGroupView* containerView = new BGroupView(B_HORIZONTAL, 240.0);    // todo: calculate from all fields
	containerView->SetName(containerLabel.String());
	// Set view color to standard background grey
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

    // add data label
    BString label(name);
    label.Append("_label");
/*
    BStringView* labelView = new BStringView(label.String(), reinterpret_cast<const char*>(name));
    labelView->ResizeToPreferred();
    containerView->GroupLayout()->AddView(labelView);
*/
    // add data field
    BView* dataView = NULL;

    switch(typeCode) {
        case B_BOOL_TYPE: {
            dataView = new BCheckBox(name, new BMessage()); /* todo: */
            bool state = reinterpret_cast<const bool*>(data);
            reinterpret_cast<BCheckBox*>(dataView)->SetValue(
                state ? B_CONTROL_ON : B_CONTROL_OFF);   // wtf? no bool parameter?
            break;
        }
        case B_DOUBLE_TYPE: {
            const double* number = reinterpret_cast<const double*>(data);
            BString value;
            value << *number;
            dataView = new BTextControl(name, value.String(), new BMessage() /* todo: */);
            break;
        }
        case B_FLOAT_TYPE: {
            const float* number = reinterpret_cast<const float*>(data);
            BString value;
            value << *number;
            dataView = new BTextControl(name, value.String(), new BMessage() /* todo: */);
            break;
        }
        case B_INT8_TYPE: {
            const int8* number = reinterpret_cast<const int8*>(data);
            BString value;
            value << *number;
            dataView = new BTextControl(name, value.String(), new BMessage() /* todo: */);
            break;
        }
        case B_INT16_TYPE: {
            const int16* number = reinterpret_cast<const int16*>(data);
            BString value;
            value << *number;
            dataView = new BTextControl(name, value.String(), new BMessage() /* todo: */);
            break;
        }
        case B_INT32_TYPE: {
            const int32* number = reinterpret_cast<const int32*>(data);
            BString value;
            value << *number;
            dataView = new BTextControl(name, value.String(), new BMessage() /* todo: */);
            break;
        }
        case B_STRING_TYPE:
            dataView = new BTextControl(name, reinterpret_cast<const char*>(data), new BMessage() /* todo: */);
            break;

        default:
            BString error("(field type of attribute '");
            error.Append(name).Append("' is not supported)");

            dataView = new BTextControl(name, error.String(), new BMessage());
            break;
    }
    // todo: handle displayable and editable flags

    dataView->ResizeToPreferred();
    containerView->GroupLayout()->AddView(dataView);
    containerView->ResizeToPreferred();

    return containerView;
}
